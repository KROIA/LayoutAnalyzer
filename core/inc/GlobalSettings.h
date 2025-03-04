#pragma once
#include "LayoutAnalyzer_base.h"

namespace LayoutAnalyzer
{
	class LAYOUT_ANALYZER_EXPORT GlobalSettings
	{
		static GlobalSettings& getInstance();
	public:

		static sf::Color getSilkscreenColor() { return getInstance().m_silkscreenColor; }
		static void setSilkscreenColor(const sf::Color& color) { getInstance().m_silkscreenColor = color; }

		static sf::Color getViaColor() { return getInstance().m_viaColor; }
		static void setViaColor(const sf::Color& color) { getInstance().m_viaColor = color; }

		static float getViaRadius() { return getInstance().m_viaRadius; }
		static void setViaRadius(float radius) { getInstance().m_viaRadius = radius; }

		static std::string getFilterMaskPath() { return getInstance().m_filterMaskPath; }
		static void setFilterMaskPath(const std::string& path) { getInstance().m_filterMaskPath = path; }
	private:
		sf::Color m_silkscreenColor;
		sf::Color m_viaColor;
		float m_viaRadius = 10;
		std::string m_filterMaskPath;
	};
}