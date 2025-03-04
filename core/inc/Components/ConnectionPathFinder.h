#pragma once
#include "LayoutAnalyzer_base.h"
#include "LargePixelPainter.h"

namespace LayoutAnalyzer
{
	class LAYOUT_ANALYZER_EXPORT ConnectionPathFinder :
		public QSFML::Components::Component,
		public QSFML::Utilities::Updatable
	{
	public:
		struct PathFinderResult
		{
			std::vector<sf::Vector2u> pixelPositions;
			std::vector<sf::Vector2u> viaPositions;
		};
        ConnectionPathFinder();
		~ConnectionPathFinder();

		void setPixelPainter(LargePixelPainter* painter)
		{
			if (!painter)
				stopThread();
			m_pixelPainter = painter;
		}
		void setPathfinderResultPainter(LargePixelPainter* painter)
		{
			if (!painter)
				stopThread();
			m_pathfinderResult = painter;
		}
        void startPathFinder(const sf::Vector2u& start);
		void startPathFinderFromVia(const sf::Vector2u& via);
		void stopPathFinder() {
			stopThread();
		}

		void setViaLocations(const std::vector<sf::Vector2u>& locations)
		{
			m_viaLocations = locations;
		}
		const PathFinderResult& getResult() const
		{
			return m_result;
		}
    protected:

		void update() override;

	private:
		void stopThread();
		void pathFinderThreadFromConnectionWire();
		void pathFinderThreadFromVia();

        LargePixelPainter* m_pixelPainter = nullptr;
		LargePixelPainter* m_pathfinderResult = nullptr;
        bool m_searching;
		sf::Vector2u m_start;
		sf::Color m_connectionColor;
		sf::Color m_startColor;

		std::thread* m_thread;
		std::atomic<bool> m_stopThread;
		std::atomic<bool> m_pathFinished;
		int m_textureUptdateCounter = 0;

		std::vector<sf::Vector2u> m_viaLocations;


		PathFinderResult m_result;
    };
}