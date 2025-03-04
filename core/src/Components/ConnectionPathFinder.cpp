#include "Components/ConnectionPathFinder.h"
#include "GlobalSettings.h"
#include <queue>

namespace LayoutAnalyzer
{
	ConnectionPathFinder::ConnectionPathFinder()
		: Component("ConnectionPathFinder")
		, m_thread(nullptr)
		, m_searching(false)
	{
	}

	ConnectionPathFinder::~ConnectionPathFinder()
	{
	}

	void ConnectionPathFinder::startPathFinder(const sf::Vector2u& start)
	{
		if (m_pixelPainter == nullptr || m_pathfinderResult == nullptr)
			return;
		sf::Vector2u pixelCount = m_pixelPainter->getPixelCount();
		if (start.x >= pixelCount.x || start.y >= pixelCount.y)
			return;

		stopThread();

		m_start = start;
		m_searching = true;
		m_startColor = m_pixelPainter->getPixel(start);
		m_connectionColor = m_startColor;
		m_connectionColor.r = std::min(255, m_connectionColor.r + 100);
		m_connectionColor.g = std::min(255, m_connectionColor.g + 100);
		m_connectionColor.b = std::min(255, m_connectionColor.b + 100);
		m_connectionColor.a = 255;

		m_pathfinderResult->clearPixels(sf::Color::Transparent);
		m_pathfinderResult->setPixel(start, m_connectionColor);

		m_textureUptdateCounter = false;
		m_pathFinished = false;
		m_thread = new std::thread(&ConnectionPathFinder::pathFinderThread, this);

	}


	void ConnectionPathFinder::update()
	{
		if (!m_searching || !m_pixelPainter || !m_pathfinderResult)
			return;

		m_textureUptdateCounter++;
		//sf::Vector2u pixelCount = m_pixelPainter->getPixelCount();



		if(m_textureUptdateCounter > 100 || m_pathFinished == true)
		{
			if (m_pathFinished)
			{
				m_pathFinished = false;
				m_searching = false;
			}
			m_textureUptdateCounter = 0;
			m_pathfinderResult->updateTexture();
		}
	}

	void ConnectionPathFinder::stopThread()
	{
		if (m_thread)
		{
			m_stopThread = true;
			m_thread->join();
			delete m_thread;
			m_stopThread = false;
			m_thread = nullptr;
		}
		m_searching = false;
	}
	void ConnectionPathFinder::pathFinderThread()
	{
		sf::Vector2u pixelCount = m_pixelPainter->getPixelCount();
		int width = pixelCount.x;
		int height = pixelCount.y;
		std::queue<std::pair<int, int>> pixels;
		pixels.push({ m_start.x, m_start.y });

		std::vector<std::vector<bool>> visited(pixelCount.x, std::vector<bool>(pixelCount.y, false));
		visited[m_start.x][m_start.y] = true;  // Mark as visited

		sf::Color printColor = GlobalSettings::getSilkscreenColor();
		sf::Color viaColor = GlobalSettings::getViaColor();

		while (!m_stopThread && !pixels.empty())
		{
			auto [x, y] = pixels.front();
			pixels.pop();

			// Change the color
			m_pathfinderResult->setPixel(sf::Vector2u(x,y), m_connectionColor);

			// Check all 4 neighboring pixels (left, right, up, down)
			const std::pair<int, int> directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

			for (auto [dx, dy] : directions) {
				int newX = x + dx;
				int newY = y + dy;

				// Bounds check
				if (newX >= 0 && newY >= 0 && newX < width && newY < height) {
					sf::Color c = m_pixelPainter->getPixel(sf::Vector2u(newX, newY));
					if (!visited[newX][newY])
					{
						if (c == m_startColor || c == viaColor) {
							pixels.push({ newX, newY });
							visited[newX][newY] = true; // Mark as visited immediately
						}
						else if(c == printColor)
						{
							for (int i = 0; i < 10; ++i)
							{
								newX = x + dx * i;
								newY = y + dy * i;
								if (newX >= 0 && newY >= 0 && newX < width && newY < height)
								{
									sf::Color c2 = m_pixelPainter->getPixel(sf::Vector2u(newX, newY));
									if (!visited[newX][newY])
									{
										if (c2 == m_startColor || c == viaColor) {
											pixels.push({ newX, newY });
											visited[newX][newY] = true; // Mark as visited immediately
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (pixels.empty())
			m_pathFinished = true;
	}
}