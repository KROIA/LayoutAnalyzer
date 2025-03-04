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

    void LargePixelPainter::setPixelCount(const sf::Vector2u& count)
    {
        if (count == m_pixelCount)
            return;
        sf::Uint8* newPixels = new sf::Uint8[count.x * count.y * 4];
        memset(newPixels, 0, count.x * count.y * 4);
        m_pixelCount = count;
        m_pixels = newPixels;
		createChunks();
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
    void LargePixelPainter::clearPixels(const sf::Color& color)
    {
		for (unsigned int i = 0; i < m_pixelCount.x * m_pixelCount.y; ++i)
		{
			m_pixels[i * 4 + 0] = color.r;
			m_pixels[i * 4 + 1] = color.g;
			m_pixels[i * 4 + 2] = color.b;
			m_pixels[i * 4 + 3] = color.a;
		}
        // update all textures
		//updateTexture();
    }
    void LargePixelPainter::replacePixel(const sf::Color& from, const sf::Color& to)
    {
		for (unsigned int i = 0; i < m_pixelCount.x * m_pixelCount.y; ++i)
		{
			if (m_pixels[i * 4 + 0] == from.r &&
				m_pixels[i * 4 + 1] == from.g &&
				m_pixels[i * 4 + 2] == from.b &&
				m_pixels[i * 4 + 3] == from.a)
			{
				m_pixels[i * 4 + 0] = to.r;
				m_pixels[i * 4 + 1] = to.g;
				m_pixels[i * 4 + 2] = to.b;
				m_pixels[i * 4 + 3] = to.a;
			}
		}
		// update all textures
		//updateTexture();

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
    void LargePixelPainter::updateTexture(const sf::Vector2u& pixelPos)
    {
        // Find chunk
		size_t chunkIndex = (pixelPos.y / CHUNK_SIZE) * (m_pixelCount.x / CHUNK_SIZE) + (pixelPos.x / CHUNK_SIZE);
		if (chunkIndex >= textures.size())
			return;
		sf::Image subImage;
		subImage.create(CHUNK_SIZE, CHUNK_SIZE);
		sf::Vector2u chunkPos = textures[chunkIndex].pos;
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			for (int i = 0; i < CHUNK_SIZE; ++i) {
				if (chunkPos.x + i < m_pixelCount.x && chunkPos.y + j < m_pixelCount.y)
				{
					size_t pixelCoord = ((chunkPos.y + j) * m_pixelCount.x + (chunkPos.x + i)) * 4;
					sf::Color color(m_pixels[pixelCoord + 0],
						m_pixels[pixelCoord + 1],
						m_pixels[pixelCoord + 2],
						m_pixels[pixelCoord + 3]);
					subImage.setPixel(i, j, color);
				}
			}
		}
		textures[chunkIndex].texture.update(subImage);
    }
    void LargePixelPainter::updateTexture()
    {
        for (size_t t = 0; t < textures.size(); ++t)
        {
            sf::Image subImage;
            subImage.create(CHUNK_SIZE, CHUNK_SIZE); 
            sf::Vector2u chunkPos = textures[t].pos;
            for (int j = 0; j < CHUNK_SIZE; ++j) {
                for (int i = 0; i < CHUNK_SIZE; ++i) {
                    if (chunkPos.x + i < m_pixelCount.x && chunkPos.y + j < m_pixelCount.y)
                    {
                        size_t pixelCoord = ((chunkPos.y + j) * m_pixelCount.x + (chunkPos.x + i)) * 4;
                        sf::Color color(m_pixels[pixelCoord + 0],
                            m_pixels[pixelCoord + 1],
                            m_pixels[pixelCoord + 2],
                            m_pixels[pixelCoord + 3]);
                        subImage.setPixel(i, j, color);
                    }
                    else
                    {
                        subImage.setPixel(i, j, sf::Color::Transparent);
                    }
                }
            }
            textures[t].texture.update(subImage);
        }

		sprites.clear();
		sprites.reserve((m_pixelCount.x / CHUNK_SIZE) * (m_pixelCount.y / CHUNK_SIZE));
        for (size_t i = 0; i < textures.size(); ++i)
        {
            sf::Sprite sprite(textures[i].texture);
            sprite.setPosition(textures[i].pos.x, textures[i].pos.y);
            sprites.emplace_back(std::move(sprite));
        }
    }

    void LargePixelPainter::clear()
    {
        memset(m_pixels, 0, m_pixelCount.x * m_pixelCount.y * 4);
    }

    void LargePixelPainter::createChunks()
    {
        unsigned int width = m_pixelCount.x;
        unsigned int height = m_pixelCount.y;

		textures.clear();
		sprites.clear();
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

        unsigned error = lodepng::decode(image, width, height, filename, LCT_RGBA, 8);

        if (error) {
            Logger::logError("Failed to load image: " + filename + " error: " + lodepng_error_text(error));
            return false;
        }
        setPixelCount({ width , height });
        memcpy(m_pixels, image.data(), width * height * 4);
        /*for (unsigned x = 0; x < width; x++)
        {
            for (unsigned y = 0; y < height; y++)
            {
                size_t subIndex = ((size_t)y * (size_t)width + (size_t)x);
                size_t index1 = 4 * subIndex;
                size_t index2 = subIndex * 4;
                m_pixels[index2 + 0] = image[index1] ;
                m_pixels[index2 + 1] = image[index1+1];
                m_pixels[index2 + 2] = image[index1+2];
                m_pixels[index2 + 3] = image[index1 + 3];
            }
        }*/
        Logger::logInfo("Image loaded: " + filename);
        return true;
    }
}