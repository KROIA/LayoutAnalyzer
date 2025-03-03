#pragma once
#include "LayoutAnalyzer_base.h"
#include "Components/LargePixelPainter.h"

namespace LayoutAnalyzer
{
	class LAYOUT_ANALYZER_EXPORT PrintLayer : public QSFML::Objects::GameObject
	{
		public:
		OBJECT_DECL(PrintLayer);
		PrintLayer(const std::string &imagePath);
		~PrintLayer();

		private:

		LargePixelPainter *m_painter = nullptr;
	};
}