#pragma once
#include "LayoutAnalyzer_base.h"
#include "Components/LargePixelPainter.h"
#include "Components/ViaFilter.h"


namespace LayoutAnalyzer
{
	class LAYOUT_ANALYZER_EXPORT PrintLayer : public QSFML::Objects::GameObject
	{
		public:
		OBJECT_DECL(PrintLayer);
		PrintLayer();
		~PrintLayer();


		void loadLayer(const std::string& imagePath);
		void loadFilter(const std::string& imagePath);
		void applyFilter();

		private:

		LargePixelPainter *m_painter = nullptr;
		ViaFilter* m_filter = nullptr;
		QSFML::Components::PointPainter* m_pointPainter = nullptr;
	};
}