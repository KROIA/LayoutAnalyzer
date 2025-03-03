#include "Components/ViaFilter.h"
#include <thread>
#include <iostream>

namespace LayoutAnalyzer
{
	ViaFilter::ViaFilter()
		: Component("Filter")
		, m_pixels(nullptr)
	{

	}

	ViaFilter::~ViaFilter()
	{
		delete[] m_pixels;
	}

	std::vector<sf::Vector2u> ViaFilter::getFilteredPixelCoords() const
	{
		std::vector<sf::Vector2u> rawCoords;
		sf::Vector2u maskCenterOffset = m_mask.getSize() / 2u;
		if (m_maskResult.empty())
			appyMask();
		float sensitivity = 0.8f;
		for (size_t y = 0; y < m_pixelCount.y; ++y)
		{
			for (size_t x = 0; x < m_pixelCount.x; ++x)
			{
				sf::Vector2u pos(x, y);
				size_t index = getPixelIndex(pos);
				float grayScaleCorrelation = (m_maskResult[index * 3] + m_maskResult[index * 3 + 1] + m_maskResult[index * 3 + 2])/3.f;
				if (grayScaleCorrelation > sensitivity)
					rawCoords.push_back(pos + maskCenterOffset);
			}
		}
		
		// averate nearby points and reduce to one point
		std::vector<sf::Vector2u> coords;
		std::unordered_map<size_t, bool> used;
		for (size_t i = 0; i < rawCoords.size(); ++i)
		{
			sf::Vector2u pos = rawCoords[i];
			sf::Vector2u sum = pos;
			unsigned int count = 1;
			
			for (size_t j = 0; j < rawCoords.size(); ++j)
			{
				if (i == j || used[j])
					continue;
				sf::Vector2u other = rawCoords[j];
				if (abs((int)pos.x - (int)other.x) < (int)m_mask.getSize().x &&
					abs((int)pos.y - (int)other.y) < (int)m_mask.getSize().y)
				{
					sum += other;
					++count;
					used[j] = true;
				}
			}
			if(!used[i])
				coords.push_back(sum / count);
		}

		return coords;
	}

	void ViaFilter::appyMask() const
	{
		static constexpr bool useThreads = true;

		if constexpr (useThreads)
		{
			size_t threadCount = std::thread::hardware_concurrency();
			std::vector<std::thread> threads;
			m_maskResult.resize(m_pixelCount.x * m_pixelCount.y * 3, 0.0f);
			size_t chunkSize = m_pixelCount.y / threadCount;
			sf::Vector2u maskSize = m_mask.getSize();
			sf::Vector2u imageSize = m_pixelCount;

			std::atomic<size_t> progressCounter;

			for (size_t i = 0; i < threadCount; ++i)
			{
				size_t start = i * chunkSize;
				size_t end = (i + 1) * chunkSize;
				if (i == threadCount - 1)
					end = m_pixelCount.y;
				threads.push_back(std::thread([this, i, start, end, maskSize, &progressCounter]()
					{
						const sf::Vector2u maskSize = m_mask.getSize();
						float normalizer = 1.f / (maskSize.x * maskSize.y);
						size_t startIndex = start < maskSize.y ? 0 : start - maskSize.y;
						size_t endIndex = end + maskSize.y >= m_pixelCount.y ? m_pixelCount.y : end + maskSize.y;
						size_t pixels = m_pixelCount.x * m_pixelCount.y;
						size_t printInterval = 1;
						size_t counter = 0;
						for (size_t y = startIndex; y < endIndex - maskSize.y; ++y)
						{
							for (size_t x = 0; x < m_pixelCount.x - maskSize.x; ++x)
							{
								float r = 0.0f;
								float g = 0.0f;
								float b = 0.0f;

								for (size_t my = 0; my < maskSize.y; ++my)
								{
									for (size_t mx = 0; mx < maskSize.x; ++mx)
									{
										sf::Vector2u pos(mx, my);
										size_t pixelIndex = getPixelIndex(sf::Vector2u(x + mx, y + my));
										
										r += m_mask.correlateRed(pos, m_pixels[pixelIndex*3]);
										g += m_mask.correlateGreen(pos, m_pixels[pixelIndex*3+1]);
										b += m_mask.correlateBlue(pos, m_pixels[pixelIndex*3+2]);
									}
								}

								sf::Vector2u pos(x, y);
								size_t index = getPixelIndex(pos);
								m_maskResult[index * 3] = r * normalizer;
								m_maskResult[index * 3 + 1] = g * normalizer;
								m_maskResult[index * 3 + 2] = b * normalizer;
							}
							progressCounter += m_pixelCount.x - maskSize.x;
							if (i==0 && counter % printInterval == 0)
							{
								float progress = (float)progressCounter / (float)pixels;
								std::cout << "Progress: " << progress * 100 << "%\n";
							}
							counter++;
						}
					}));
			}

			for (auto& thread : threads)
				thread.join();


		}
		else
		{
			const sf::Vector2u maskSize = m_mask.getSize();
			float normalizer = 1.f / (maskSize.x * maskSize.y);
			m_maskResult.resize(m_pixelCount.x * m_pixelCount.y * 3, 0.0f);

			for (size_t y = 0; y < m_pixelCount.y - maskSize.x; ++y)
			{
				for (size_t x = 0; x < m_pixelCount.x - maskSize.y; ++x)
				{
					float r = 0.0f;
					float g = 0.0f;
					float b = 0.0f;

					for (size_t my = 0; my < maskSize.y; ++my)
					{
						for (size_t mx = 0; mx < maskSize.x; ++mx)
						{
							sf::Vector2u pos(mx, my);
							size_t pixelIndex = getPixelIndex(sf::Vector2u(x + mx, y + my));

							r += m_mask.correlateRed(pos, m_pixels[pixelIndex]);
							g += m_mask.correlateGreen(pos, m_pixels[pixelIndex + 1]);
							b += m_mask.correlateBlue(pos, m_pixels[pixelIndex + 2]);
						}
					}

					sf::Vector2u pos(x, y);
					size_t index = getPixelIndex(pos);
					m_maskResult[index * 3] = r * normalizer;
					m_maskResult[index * 3 + 1] = g * normalizer;
					m_maskResult[index * 3 + 2] = b * normalizer;
				}
			}
		}
	}
}