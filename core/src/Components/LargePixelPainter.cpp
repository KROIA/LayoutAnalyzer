#include "Components/LargePixelPainter.h"
#include "lodepng.h"

namespace LayoutAnalyzer
{
    COMPONENT_IMPL(LargePixelPainter);
 

    LargePixelPainter::LargePixelPainter(const std::string& name)
        : Drawable(name)
        , m_pixelCount(0, 0)
        , m_pixelSize(1.0f)
        , m_pixels(nullptr)
    {

    }
    LargePixelPainter::LargePixelPainter(const LargePixelPainter& other)
        : Drawable(other)
        , m_pixelCount(other.m_pixelCount)
        , m_pixelSize(other.m_pixelSize)
        , m_pixels(new sf::Uint8[other.m_pixelCount.x * other.m_pixelCount.y * 4])
    {
        memcpy(m_pixels, other.m_pixels, m_pixelCount.x * m_pixelCount.y * 4);
    }
    LargePixelPainter::~LargePixelPainter()
    {
        delete[] m_pixels;
    }

    bool LargePixelPainter::loadFromImage(const std::string& imagePath)
    {
        if (!loadLargeImageToSfImage(imagePath)) {
            Logger::logError("Failed to load image: " + imagePath);
            return false;
        }
        createChunks();
        return true;
    }

    void LargePixelPainter::setPixelSize(float size)
    {
        m_pixelSize = size;
    }
    float LargePixelPainter::getPixelSize() const
    {
        return m_pixelSize;
    }

    void LargePixelPainter::setPixelCount(const sf::Vector2u& count)
    {
        if (count == m_pixelCount)
            return;
        sf::Uint8* newPixels = new sf::Uint8[count.x * count.y * 4];
        if (m_pixels)
        {
            for (unsigned int i = 0; i < count.y; ++i)
            {
                for (unsigned int j = 0; j < count.x; ++j)
                {
                    if (i < m_pixelCount.y && j < m_pixelCount.x)
                    {
                        newPixels[(i * count.x + j) * 4 + 0] = m_pixels[(i * m_pixelCount.x + j) * 4 + 0];
                        newPixels[(i * count.x + j) * 4 + 1] = m_pixels[(i * m_pixelCount.x + j) * 4 + 1];
                        newPixels[(i * count.x + j) * 4 + 2] = m_pixels[(i * m_pixelCount.x + j) * 4 + 2];
                        newPixels[(i * count.x + j) * 4 + 3] = m_pixels[(i * m_pixelCount.x + j) * 4 + 3];
                    }
                    else
                    {
                        newPixels[(i * count.x + j) * 4 + 0] = 0;
                        newPixels[(i * count.x + j) * 4 + 1] = 0;
                        newPixels[(i * count.x + j) * 4 + 2] = 0;
                        newPixels[(i * count.x + j) * 4 + 3] = 0;
                    }
                }
            }
            delete[] m_pixels;
        }
        m_pixelCount = count;
        m_pixels = newPixels;
    }
    const sf::Vector2u& LargePixelPainter::getPixelCount() const
    {
        return m_pixelCount;
    }

    void LargePixelPainter::setPixel(const sf::Vector2u& pos, const sf::Color& c)
    {
        if (pos.x < m_pixelCount.x && pos.y < m_pixelCount.y)
        {
            size_t index = (pos.y * m_pixelCount.x + pos.x) * 4;
            m_pixels[index + 0] = c.r;
            m_pixels[index + 1] = c.g;
            m_pixels[index + 2] = c.b;
            m_pixels[index + 3] = c.a;
        }
    }
    sf::Color LargePixelPainter::getPixel(const sf::Vector2u& pos) const
    {
        if (pos.x < m_pixelCount.x && pos.y < m_pixelCount.y)
        {
            size_t index = (pos.y * m_pixelCount.x + pos.x) * 4;
            return sf::Color(m_pixels[index + 0],
                             m_pixels[index + 1],
                             m_pixels[index + 2],
                             m_pixels[index + 3]);
        }
        return sf::Color::Black;
    }

    void LargePixelPainter::clear()
    {
        memset(m_pixels, 0, m_pixelCount.x * m_pixelCount.y * 4);
    }

    void LargePixelPainter::createChunks()
    {
        unsigned int width = m_pixelCount.x;
        unsigned int height = m_pixelCount.y;

		textures.reserve((width / CHUNK_SIZE) * (height / CHUNK_SIZE));
		sprites.reserve((width / CHUNK_SIZE) * (height / CHUNK_SIZE));

        for (unsigned int y = 0; y < height; y += CHUNK_SIZE) {
            for (unsigned int x = 0; x < width; x += CHUNK_SIZE) {
                TextureData texture;
                texture.texture.create(CHUNK_SIZE, CHUNK_SIZE);
                sf::Image subImage;
                subImage.create(CHUNK_SIZE, CHUNK_SIZE);

                for (int j = 0; j < CHUNK_SIZE; ++j) {
                    for (int i = 0; i < CHUNK_SIZE; ++i) {
                        if (x + i < width && y + j < height)
                        {
							size_t pixelCoord = ((y + j) * width + (x + i)) * 4;
							sf::Color color(m_pixels[pixelCoord + 0],
											m_pixels[pixelCoord + 1],
											m_pixels[pixelCoord + 2],
											m_pixels[pixelCoord + 3]);
                            subImage.setPixel(i, j, color);
                        }
                    }
                }
                texture.texture.update(subImage);
                texture.pos = { x,y };
                textures.emplace_back(std::move(texture));
            }
        }
        for (size_t i = 0; i < textures.size(); ++i)
        {
            sf::Sprite sprite(textures[i].texture);
            sprite.setPosition(textures[i].pos.x, textures[i].pos.y);
            sprites.emplace_back(std::move(sprite));
        }
    }

    void LargePixelPainter::drawComponent(sf::RenderTarget& target,
                                     sf::RenderStates states) const
    {
		LA_UNUSED(states);        
        for (const auto& sprite : sprites) {
            target.draw(sprite);
        }
    }




    bool LargePixelPainter::loadLargeImageToSfImage(const std::string& filename)
    {
        Logger::logInfo("Loading image: " + filename);
        std::vector<unsigned char> image; //the raw pixels
        unsigned width, height;

        unsigned error = lodepng::decode(image, width, height, filename, LCT_RGB, 8);

        if (error) {
            Logger::logError("Failed to load image: " + filename + " error: " + lodepng_error_text(error));
            return false;
        }
        setPixelCount({ width , height });
        //m_vertecies.resize(width * height * 4);
        
        
        //size_t size = width * height;
        for (unsigned x = 0; x < width; x++)
        {
            for (unsigned y = 0; y < height; y++)
            {
                size_t index = 3 * ((size_t)y * (size_t)width + (size_t)x);
                //size_t index1 = 3 * ((size_t)y * (size_t)width + (size_t)x);
                //size_t index2 = 3 * ((size_t)y * (size_t)width + (size_t)(x+1));
                //size_t index3 = 3 * ((size_t)(y+1) * (size_t)width + (size_t)(x+1));
                //size_t index4 = 3 * ((size_t)(y+1) * (size_t)width + (size_t)x);
                //if (index3 >= size)
                //    continue;
                //int r = (int)image[index1] + (int)image[index2] + (int)image[index3] + (int)image[index4];
                //int g = (int)image[index1+1] + (int)image[index2+1] + (int)image[index3+1] + (int)image[index4+1];
                //int b = (int)image[index1+2] + (int)image[index2+2] + (int)image[index3+2] + (int)image[index4+2];
                //sf::Color color(r/4,g/4,b/4);
                sf::Color color(image[index], image[index + 1], image[index + 2]);
                setPixel({ x, y }, color);
                /*m_vertecies[counter].color = color;
                m_vertecies[counter++].position = sf::Vector2f(x, y);
                m_vertecies[counter].color = color;
                m_vertecies[counter++].position = sf::Vector2f(x+1, y);
                m_vertecies[counter].color = color;
                m_vertecies[counter++].position = sf::Vector2f(x+1, y+1);
                m_vertecies[counter].color = color;
                m_vertecies[counter++].position = sf::Vector2f(x, y+1);*/
            }
        }
        Logger::logInfo("Image loaded: " + filename);
        return true;
    }
}