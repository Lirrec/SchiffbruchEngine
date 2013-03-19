#ifndef NODE
#define NODE

#include <list>

namespace sbe
{
	/**

	*/
	class Node
	{
		public:

			Node();
			virtual ~Node() {}

		private:
			std::list<Node> Childs;
	};
} // namespace sbe
#endif // NODE
