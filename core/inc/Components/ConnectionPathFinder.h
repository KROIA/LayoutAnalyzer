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
        ConnectionPathFinder();
		~ConnectionPathFinder();

		void setPixelPainter(LargePixelPainter* painter)
		{
			if (!painter)
				stopThread();
			m_pixelPainter = painter;
		}
		void setPathfinderResult(LargePixelPainter* painter)
		{
			if (!painter)
				stopThread();
			m_pathfinderResult = painter;
		}
        void startPathFinder(const sf::Vector2u& start);
		void stopPathFinder() {
			stopThread();
		}

    protected:

		void update() override;

	private:
		void stopThread();
		void pathFinderThread();

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

    };
}