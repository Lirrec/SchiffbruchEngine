#include "sbe/geom/Point.hpp"

namespace sbe {
	namespace Geom {
		template class point<int>;
		template class point<point<int>>;
		template class point<float>;
		template class point<point<float>>;
	}
} // namespace sbe
