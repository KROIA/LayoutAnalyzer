#pragma once
#include "LayoutAnalyzer_base.h"

namespace LayoutAnalyzer
{
	class FilterMask
	{
	public:
		FilterMask() = default;
		FilterMask(const FilterMask& other)
			: m_filterSize(other.m_filterSize)
			, m_filterMaskR(new float[m_filterSize.x * m_filterSize.y])
			, m_filterMaskG(new float[m_filterSize.x * m_filterSize.y])
			, m_filterMaskB(new float[m_filterSize.x * m_filterSize.y])
		{
			memcpy(m_filterMaskR, other.m_filterMaskR, m_filterSize.x * m_filterSize.y * sizeof(float));
			memcpy(m_filterMaskG, other.m_filterMaskG, m_filterSize.x * m_filterSize.y * sizeof(float));
			memcpy(m_filterMaskB, other.m_filterMaskB, m_filterSize.x * m_filterSize.y * sizeof(float));
		}
		virtual ~FilterMask() = default;

		bool loadFromImage(const std::string& path)
		{
			sf::Image img;
			if (img.loadFromFile(path))
			{
				setSize(img.getSize());
				for (size_t y = 0; y < m_filterSize.y; ++y)
				{
					for (size_t x = 0; x < m_filterSize.x; ++x)
					{
						sf::Color color = img.getPixel(x, y);
						sf::Vector2u pos(x, y);
						size_t index = getFilterIndex(pos);
						m_filterMaskR[index] = color.r;
						m_filterMaskG[index] = color.g;
						m_filterMaskB[index] = color.b;
					}
				}
				return true;
			}
			return false;
		}
		void setSize(const sf::Vector2u& size)
		{
			m_filterSize = size;
			delete m_filterMaskR;
			m_filterMaskR = new float[m_filterSize.x * m_filterSize.y];
			delete m_filterMaskG;
			m_filterMaskG = new float[m_filterSize.x * m_filterSize.y];
			delete m_filterMaskB;
			m_filterMaskB = new float[m_filterSize.x * m_filterSize.y];
		}
		const sf::Vector2u& getSize() const
		{
			return m_filterSize;
		}
		void scale(float factor)
		{
			for (size_t i = 0; i < m_filterSize.x * m_filterSize.y; ++i)
			{
				m_filterMaskR[i] *= factor;
				m_filterMaskG[i] *= factor;
				m_filterMaskB[i] *= factor;
			}
		}
		void setFilterMaskValueRed(const sf::Vector2u& pos, float value)
		{
			m_filterMaskR[getFilterIndex(pos)] = value;
		}
		void setFilterMaskValueGreen(const sf::Vector2u& pos, float value)
		{
			m_filterMaskG[getFilterIndex(pos)] = value;
		}
		void setFilterMaskValueBlue(const sf::Vector2u& pos, float value)
		{
			m_filterMaskB[getFilterIndex(pos)] = value;
		}
		void setFilterMaskValue(float value)
		{
			for (size_t i = 0; i < m_filterSize.x * m_filterSize.y; ++i)
			{
				m_filterMaskR[i] = value;
				m_filterMaskG[i] = value;
				m_filterMaskB[i] = value;
			}
		}
		float getFilterMaskValueRed(const sf::Vector2u& pos) const
		{
			return m_filterMaskR[getFilterIndex(pos)];
		}
		float getFilterMaskValueGreen(const sf::Vector2u& pos) const
		{
			return m_filterMaskG[getFilterIndex(pos)];
		}
		float getFilterMaskValueBlue(const sf::Vector2u& pos) const
		{
			return m_filterMaskB[getFilterIndex(pos)];
		}

		float correlateRed(const sf::Vector2u& pos, float red) const
		{
			size_t index = getFilterIndex(pos);
			float filterR = m_filterMaskR[index];
			float div = std::abs(red - filterR);
			return 1 - div;
		}
		float correlateGreen(const sf::Vector2u& pos, float green) const
		{
			size_t index = getFilterIndex(pos);
			float filterG = m_filterMaskG[index];
			float div = std::abs(green - filterG);
			return 1 - div;
		}
		float correlateBlue(const sf::Vector2u& pos, float blue) const
		{
			size_t index = getFilterIndex(pos);
			float filterB = m_filterMaskB[index];
			float div = std::abs(blue - filterB);
			return 1 - div;
		}

	protected:
		size_t getFilterIndex(const sf::Vector2u& pos) const
		{
			return pos.y * m_filterSize.x + pos.x;
		}
	private:
		
		sf::Vector2u m_filterSize;
		float* m_filterMaskR = nullptr;
		float* m_filterMaskG = nullptr;
		float* m_filterMaskB = nullptr;

	};
}
