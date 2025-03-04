#include "GlobalSettings.h"

namespace LayoutAnalyzer
{
	GlobalSettings& GlobalSettings::getInstance()
	{
		static GlobalSettings instance;
		instance.m_viaColor.a = 255;
		return instance;
	}
}