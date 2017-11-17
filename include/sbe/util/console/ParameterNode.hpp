#ifndef PARAMETERNODE_HPP
#define PARAMETERNODE_HPP

#include "Node.hpp"
#include <functional>

namespace sbe
{

	class ParameterNode : public Node
	{
	public:

		/**
			A Validator is used to determine wether a String is valid for a ParameterNode.
			Should return true if the given string is valid.
			This can be used to ensure that a parameter is e.g. a valid integer, an existing config node, etc.
		*/
		typedef std::function<bool(const std::string&)> Validator;

		ParameterNode();

		~ParameterNode();

		void setValidator(Validator Val) { V = Val; }

		bool Is(const std::string& s) override;

		bool startsWith(const std::string& prefix) override;

	private:

		Validator V;
	};
}

#endif // PARAMETERNODE_HPP

