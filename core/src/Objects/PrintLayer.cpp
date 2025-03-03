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



    /*bool loadLargeImageToSfImage(const std::string& filename, QSFML::Components::PixelPainter* outputImage) {
        int width, height, channels;

        // Open file using fopen_s (safe alternative to fopen)
        FILE* file = nullptr;
        if (fopen_s(&file, filename.c_str(), "rb") != 0 || !file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        // Get image dimensions without loading the full image
        stbi_info_from_file(file, &width, &height, &channels);
        fclose(file);  // Close file after retrieving info

        std::cout << "Image dimensions: " << width << "x" << height << ", Channels: " << channels << std::endl;

        // Create an empty sf::Image
        outputImage->setPixelCount({ (unsigned)width, (unsigned)height });

        // Chunk size to avoid excessive memory usage
        const int CHUNK_SIZE = 4096;  // Load image in 4K x 4K chunks

        // Read the image in chunks
        for (int y = 0; y < height; y += CHUNK_SIZE) {
            for (int x = 0; x < width; x += CHUNK_SIZE) {
                int w = std::min(CHUNK_SIZE, width - x);
                int h = std::min(CHUNK_SIZE, height - y);

                // Load a chunk of the image
                unsigned char* data = stbi_load(filename.c_str(), &w, &h, &channels, 4);
                if (!data) {
                    std::cerr << "Failed to load chunk at (" << x << ", " << y << ")" << std::endl;
                    return false;
                }

                // Copy pixels into sf::Image
                for (int j = 0; j < h; j++) {
                    for (int i = 0; i < w; i++) {
                        int index = (j * w + i) * 4; // RGBA index
                        sf::Color color(data[index], data[index + 1], data[index + 2], data[index + 3]);
                        outputImage->setPixel({ (unsigned)(x + i), (unsigned)(y + j) }, color);
                    }
                }

                // Free memory after copying
                stbi_image_free(data);

                std::cout << "Loaded chunk: (" << x << ", " << y << ") Size: " << w << "x" << h << std::endl;
            }
        }

        return true;
    }*/


    
}