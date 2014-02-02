#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "sbe/util/ClassFactory.hpp"
#include <boost/any.hpp>

namespace sbe
{
	typedef std::pair<std::string, std::shared_ptr<CopyFactory<boost::any>>> ComponentInfo;
} // namespace sbe
#endif // COMPONENT_HPP
