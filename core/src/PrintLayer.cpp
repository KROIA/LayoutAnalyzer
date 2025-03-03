#include "PrintLayer.h"
#include "SFML/Graphics/Image.hpp"


namespace LayoutAnalyzer
{
	OBJECT_IMPL(PrintLayer);

    
	PrintLayer::PrintLayer(const std::string& imagePath)
		: GameObject("Layer")
	{
		m_painter = new LargePixelPainter();
		addComponent(m_painter);

		if (!m_painter->loadFromImage(imagePath))
		{
            Logger::logError("Failed to load image: " + imagePath);
		}
		else
		{
            Logger::logInfo("Loaded image: " + imagePath );
		}

	
		
	}

	PrintLayer::~PrintLayer()
	{
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