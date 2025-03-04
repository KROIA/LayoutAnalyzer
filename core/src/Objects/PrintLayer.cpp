#include "Objects/PrintLayer.h"
#include "SFML/Graphics/Image.hpp"


namespace LayoutAnalyzer
{
	OBJECT_IMPL(PrintLayer);

    
	PrintLayer::PrintLayer()
		: GameObject("Layer")
	{
		m_painter = new LargePixelPainter();
		addComponent(m_painter);


		// Create filter
		m_filter = new ViaFilter();
		addComponent(m_filter);
    
	    //m_filter->loadFilterMaskFromImage("..\\Filtermask\\ViaFiltermask.png");
        m_filter->setScale(1.f / 255);
		
		

		// Create point painter
		m_pointPainter = new QSFML::Components::PointPainter();
		addComponent(m_pointPainter);
		m_pointPainter->setColor(sf::Color::Red);
		m_pointPainter->setRadius(1.0f);

		
	}

	PrintLayer::~PrintLayer()
	{
	}

    void PrintLayer::loadLayer(const std::string& imagePath)
    {
        if (!m_painter->loadFromImage(imagePath))
        {
            Logger::logError("Failed to load image: " + imagePath);
        }
        else
        {
            Logger::logInfo("Loaded image: " + imagePath);
            m_filter->setImage(m_painter->getPixels(), m_painter->getPixelCount());
        }        
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
        }
    }
    void PrintLayer::applyFilter()
    {
		m_filter->clearCache();
        std::vector<sf::Vector2u> filteredCoords = m_filter->getFilteredPixelCoords();
        std::vector<sf::Vector2f> filteredCoordsFloat;
        for (const sf::Vector2u& coord : filteredCoords)
        {
            filteredCoordsFloat.push_back(sf::Vector2f(coord.x, coord.y));
        }
        m_pointPainter->setPoints(filteredCoordsFloat);
    }    
}