#pragma once
#include "LayoutAnalyzer_base.h"

namespace LayoutAnalyzer
{
	class IFilter
	{
	public:
		IFilter() = default;
		virtual ~IFilter() = default;

		virtual std::vector<sf::Vector2u> getFilteredPixelCoords() const = 0;
	private:

	};
}
