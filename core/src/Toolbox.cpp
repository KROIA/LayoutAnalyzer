#include "Toolbox.h"

namespace LayoutAnalyzer
{
	Toolbox& Toolbox::getInstance()
	{
		static Toolbox instance;
		return instance;
	}
}