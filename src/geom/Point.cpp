#include "sbe/geom/Point.hpp"

namespace Geom {
	template class point<int>;
	template class point<point<int>>;
	template class point<float>;
	template class point<point<float>>;
}
