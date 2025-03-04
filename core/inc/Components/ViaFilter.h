#pragma once
#include "LayoutAnalyzer_base.h"
#include "IFilter.h"
#include "FilterMask.h"

namespace LayoutAnalyzer
{
	class LAYOUT_ANALYZER_EXPORT ViaFilter : public QSFML::Components::Component, public IFilter
	{
	public:
		ViaFilter();
		~ViaFilter();

		std::vector<sf::Vector2u> getFilteredPixelCoords() const override;

		bool loadFilterMaskFromImage(const std::string& path)
		{
			bool success = m_mask.loadFromImage(path);
			m_mask.scale(m_scale);
			return success;
		}

		void setImage(const sf::Uint8* pixels, const sf::Vector2u& pixelCount)
		{
			delete[] m_pixels;
			m_pixels = new float[pixelCount.x * pixelCount.y*3];
			for (size_t i = 0; i < pixelCount.x * pixelCount.y; ++i)
			{
				// Reduce RGBA to RGB and scale
				m_pixels[i*3] = pixels[i*4]*m_scale;
				m_pixels[i*3+1] = pixels[i*4+1]*m_scale;
				m_pixels[i*3+2] = pixels[i*4+2]*m_scale;
			}
			m_pixelCount = pixelCount;
			m_maskResult.clear();
		}

		void setFilterMask(const FilterMask& mask)
		{
			m_mask = mask;
			m_maskResult.clear();
		}
		void clearCache()
		{
			m_maskResult.clear();
		}

		void setScale(float scale)
		{
			m_mask.scale(scale/m_scale);
			m_scale = scale;
		}


	private:
		size_t getPixelIndex(const sf::Vector2u& pos) const
		{
			return pos.y * m_pixelCount.x + pos.x;
		}
		/*sf::Color getColor(const sf::Vector2u& pos) const
		{
			size_t index = getPixelIndex(pos);
			return sf::Color(m_pixels[index * 4], m_pixels[index * 4 + 1], m_pixels[index * 4 + 2], m_pixels[index * 4 + 3]);
		}*/
		void appyMask() const;

		float* m_pixels = nullptr;
		float m_scale = 1.f;
		sf::Vector2u m_pixelCount;
		FilterMask m_mask;

		mutable std::vector<float> m_maskResult;
	};
}
