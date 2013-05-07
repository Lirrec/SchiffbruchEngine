#include "sbe/event/Event.hpp"

#include <boost/lexical_cast.hpp>

#include "sbe/Engine.hpp"

#include "sbe/event/EventHelper.hpp"

// local includes
#include "EventCore.hpp"
#include "../modules/Core.hpp"

#include <algorithm>
#include <utility>
#include <ostream>

namespace sbe
{

	Event::Event(const std::string& EventName )
	{
		init ( EventName, 0 );
	}

	Event::Event( const std::string& EventName, const boost::any& _Data )
	{
		init ( EventName, _Data );
	}

	void Event::init( const std::string& EventName, const boost::any& _Data)
	{
		if (!Core::EvtCore->HasEvent( EventName ))
			Core::EvtCore->RegisterEventName( EventName );
		EvtData = _Data;
		Type = Core::EvtCore->GetEventHash( EventName );
	}

	std::string Event::getDebugName()
	{
		std::string re("");
		re = Core::EvtCore->GetEventName( Type );
		re += " - ";
		re += boost::lexical_cast<std::string>(Type);
		return re;
	}


	bool Event::Is(const std::string& Name) {
		 return Type == Core::EvtCore->GetEventHash( Name );
	}

	bool Event::Is(const std::string& Name, const std::type_info &type )
	{
		if (!Is(Name)) return false;
		if ( EvtData.type() != type )
		{
			Engine::out(Engine::ERROR) << "[Event] " << Name << " has wrong Datatype!" <<std::endl;
			return false;
		}

		return true;
	}
} // namespace sbe
