#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "sbe/util/ClassFactory.hpp"
#include "sbe/entity/EntityID.hpp"
#include <boost/any.hpp>
#include <memory>
#include <string>
#include <utility>
#include <sbe/Engine.hpp>

namespace sbe
{
	typedef CopyFactory<boost::any> ComponentFactory;
	typedef std::pair<std::string, std::shared_ptr<ComponentFactory>> ComponentInfo;


	template<class T>
	std::shared_ptr<ComponentFactory> makeComponentFactory()
	{
		return std::dynamic_pointer_cast<ComponentFactory>(std::make_shared<AssignmentConversionFactory<T, boost::any>>());
	}

    namespace operators {
        /**
         * A String literal for easy lookup of component ids
         */
        sbeID operator "" _cId(const char* str, size_t);
    }
} // namespace sbe
#endif // COMPONENT_HPP
