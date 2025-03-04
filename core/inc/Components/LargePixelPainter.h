#pragma once
#include "LayoutAnalyzer_base.h"

namespace LayoutAnalyzer
{
    class LAYOUT_ANALYZER_EXPORT LargePixelPainter : public QSFML::Components::Drawable
    {
        public:
        LargePixelPainter(const std::string& name = "LargePixelPainter");
        LargePixelPainter(const LargePixelPainter& other);
        COMPONENT_DECL(LargePixelPainter);
        ~LargePixelPainter();

        bool loadFromImage(const std::string& imagePath);

        //void setPixelSize(float size);
        //float getPixelSize() const;

        void setPixelCount(const sf::Vector2u& count);
        const sf::Vector2u& getPixelCount() const;

        void setPixel(const sf::Vector2u& pos, const sf::Color& c);
		void replacePixel(const sf::Color& from, const sf::Color& to);
        void clearPixels(const sf::Color &color);
        sf::Color getPixel(const sf::Vector2u& pos) const;
        sf::Uint8* getPixels()
        { return m_pixels; }
        void updateTexture(const sf::Vector2u &pixelPos);
        void updateTexture();

        void clear();

        void drawComponent(sf::RenderTarget& target,
                           sf::RenderStates states) const override;

        private:
        void createChunks();
        bool loadLargeImageToSfImage(const std::string& filename);
        sf::Vector2u m_pixelCount;
        float m_pixelSize;

        sf::Uint8* m_pixels;

        const int CHUNK_SIZE = GL_MAX_TEXTURE_SIZE; // Adjust to fit within texture limits
        struct TextureData
        {
            sf::Texture texture;
            sf::Vector2u pos;
        };
        std::vector<TextureData> textures;
        std::vector<sf::Sprite> sprites;
    };
}