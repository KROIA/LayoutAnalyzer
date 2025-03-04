#include "Objects/PrintLayer.h"
#include "SFML/Graphics/Image.hpp"
#include "Toolbox.h"
#include "GlobalSettings.h"


namespace LayoutAnalyzer
{
	OBJECT_IMPL(PrintLayer);

    
	PrintLayer::PrintLayer()
		: GameObject("Layer")
	{
		m_painter = new LargePixelPainter();
		addComponent(m_painter);
		m_pathfinderResult = new LargePixelPainter();
		addComponent(m_pathfinderResult);


		// Create filter
		m_filter = new ViaFilter();
		addComponent(m_filter);
    
	    //m_filter->loadFilterMaskFromImage("..\\Filtermask\\ViaFiltermask.png");
        m_filter->setScale(1.f / 255);
		
		

		// Create point painter
		m_pointPainter = new QSFML::Components::PointPainter();
		addComponent(m_pointPainter);
        m_pointPainter->setVerteciesCount(4);
		m_pointPainter->setColor(sf::Color::Red);
		m_pointPainter->setRadius(GlobalSettings::getViaRadius());

		

		m_pathFinder = new ConnectionPathFinder();
		addComponent(m_pathFinder);
		m_pathFinder->setPixelPainter(m_painter);
		m_pathFinder->setPathfinderResult(m_pathfinderResult);


		m_mousePressEvent = new QSFML::Components::MousePressEvent();
        m_mousePressEvent->setTriggerButton(sf::Mouse::Right);
		addComponent(m_mousePressEvent);
        QObject::connect(m_mousePressEvent, &QSFML::Components::MousePressEvent::fallingEdge, [this] {
            sf::Vector2u pixelPos = sf::Vector2u(getMouseWorldPosition());
			if (pixelPos.x >= m_painter->getPixelCount().x || pixelPos.y >= m_painter->getPixelCount().y)
				return;
            switch (Toolbox::getCurrentTool())
            {
            case Toolbox::Tool::pathFinder:
                m_pathFinder->startPathFinder(pixelPos);
				break;
            case Toolbox::Tool::silkscreenColorPicker:
				GlobalSettings::setSilkscreenColor(m_painter->getPixel(pixelPos));
                break;
            case Toolbox::Tool::viaColorPicker:
                GlobalSettings::setViaColor(m_painter->getPixel(pixelPos));
                break;
            case Toolbox::Tool::backgroundColorPicker:
            {
                sf::Color color = m_painter->getPixel(pixelPos);
				m_painter->replacePixel(color, sf::Color::Transparent);
                m_painter->updateTexture();
                break;
            }
			case Toolbox::Tool::viaDeleter:
				removeViaNear(pixelPos);
				break;
            case Toolbox::Tool::viaSetter:
                addVia(pixelPos);
                break;
            
            }
			
            });
	}

	PrintLayer::~PrintLayer()
	{
	}
    void PrintLayer::enableInteractions(bool enable)
    {
		m_mousePressEvent->setEnabled(enable);
    }

    bool PrintLayer::loadLayer(const std::string& imagePath)
    {
        if (!m_painter->loadFromImage(imagePath))
        {
            Logger::logError("Failed to load image: " + imagePath);
            return false;
        }
        else
        {
            Logger::logInfo("Loaded image: " + imagePath);
			//m_painter->replacePixel(sf::Color(), sf::Color::Transparent);
            //m_painter->updateTexture();
			m_pathfinderResult->setPixelCount(m_painter->getPixelCount());
            m_filter->setImage(m_painter->getPixels(), m_painter->getPixelCount());
        }        
        return true;
    }
    void PrintLayer::loadFilter(const std::string& imagePath)
    {
        if (!m_filter->loadFilterMaskFromImage(imagePath))
        {
            Logger::logError("Failed to load filter image: " + imagePath);
        }
        else
        {
            Logger::logInfo("Loaded filter image: " + imagePath);
			GlobalSettings::setViaRadius(m_filter->getMaskSize().x / 2.f);
        }
    }
    void PrintLayer::applyFilter()
    {
		m_filter->clearCache();
        m_viaCoords = m_filter->getFilteredPixelCoords();
		updateViaDispay();
    }    
    void PrintLayer::stopPathFinder()
    {
		m_pathFinder->stopPathFinder();
    }
    void PrintLayer::removeViaNear(const sf::Vector2u& pos)
    {
		std::vector<sf::Vector2u> newViaCoords;
		float distanceThreshold = GlobalSettings::getViaRadius();
		for (const sf::Vector2u& coord : m_viaCoords)
		{
			float distance = std::sqrt((coord.x - pos.x) * (coord.x - pos.x) + (coord.y - pos.y) * (coord.y - pos.y));
			if (distance > distanceThreshold)
			{
				newViaCoords.push_back(coord);
			}
		}
		m_viaCoords = newViaCoords;
		updateViaDispay();
    }
    void PrintLayer::addVia(const sf::Vector2u& pos)
    {
		m_viaCoords.push_back(pos);
		updateViaDispay();
    }
    void PrintLayer::updateViaDispay()
    {
        std::vector<sf::Vector2f> filteredCoordsFloat;
        for (const sf::Vector2u& coord : m_viaCoords)
        {
            filteredCoordsFloat.push_back(sf::Vector2f(coord.x, coord.y));
        }
        m_pointPainter->setPoints(filteredCoordsFloat);
        m_pointPainter->setRadius(GlobalSettings::getViaRadius());
    }
}