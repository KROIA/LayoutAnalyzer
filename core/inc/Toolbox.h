#pragma once
#include "LayoutAnalyzer_base.h"

namespace LayoutAnalyzer
{
	class LAYOUT_ANALYZER_EXPORT Toolbox
	{
		static Toolbox& getInstance();
	public:
		enum Tool
		{
			none,
			pathFinder,
			viaSetter,
			viaDeleter,
			silkscreenColorPicker,
			viaColorPicker,
			backgroundColorPicker
		};

		static Tool getCurrentTool() { return getInstance().m_currentTool; }
		static void setCurrentTool(Tool tool) { getInstance().m_currentTool = tool; }

	private:
		Tool m_currentTool = none;
	};
}