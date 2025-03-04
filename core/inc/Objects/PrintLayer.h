#pragma once
#include "LayoutAnalyzer_base.h"
#include "Components/LargePixelPainter.h"
#include "Components/ViaFilter.h"
#include "Components/ConnectionPathFinder.h"


namespace LayoutAnalyzer
{
	class LAYOUT_ANALYZER_EXPORT PrintLayer : public QSFML::Objects::GameObject
	{
		public:
		OBJECT_DECL(PrintLayer);
		PrintLayer();
		~PrintLayer();

		void enableInteractions(bool enable);


		bool loadLayer(const std::string& imagePath);
		bool saveDataToFile(const std::string& path);
		bool loadDataFromFile(const std::string& path);
		const std::string& getLayerImagePath() const { return m_layerImagePath; }
		void loadFilter(const std::string& imagePath);
		void applyFilter();
		void stopPathFinder();
		void propagatePathFinderResult(const ConnectionPathFinder::PathFinderResult& result);

		private:
		void removeViaNear(const sf::Vector2u &pos);
		void addVia(const sf::Vector2u& pos);
		void updateViaDispay();

		LargePixelPainter *m_painter = nullptr;
		LargePixelPainter *m_pathfinderResult = nullptr;
		ViaFilter* m_filter = nullptr;
		ConnectionPathFinder* m_pathFinder = nullptr;
		QSFML::Components::MousePressEvent* m_mousePressEvent = nullptr;
		QSFML::Components::PointPainter* m_pointPainter = nullptr;

		std::vector<sf::Vector2u> m_viaCoords;
		std::string m_layerImagePath;
	};
}