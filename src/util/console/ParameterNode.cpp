#include "sbe/util/console/ParameterNode.hpp"

namespace sbe
{

	ParameterNode::ParameterNode()
			: Node("paramnode") {

	}

	ParameterNode::~ParameterNode() {

	}

	bool ParameterNode::Is(const std::string& s) {
		return V ? V(s) : true;
	}

	bool ParameterNode::startsWith(const std::string& prefix) {
		return V ? V(prefix) : true;
	}


}
