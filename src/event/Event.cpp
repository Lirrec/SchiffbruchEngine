#include "event/Event.h"

#include "boost/lexical_cast.hpp"

#include "Engine.h"

#include "event/EventHelper.h"

#include "event/EventCore.h"

// internal include
#include "../src/modules/Core.h"

#include <algorithm>
#include <utility>
#include <ostream>

const boost::uuids::uuid Event::invalid_source_id = Module::NewUUID();

Event::Event(const std::string& EventName,
			const boost::uuids::uuid& Source
			 )
		 : 	SourceID(Source),
			EventID(Module::NewUUID())
{
	EvtData = 0;

	if (!Core::EvtCore->HasEvent( EventName ))
	{
		Engine::out() << "[EVENT] First Time occurence of new Event: '" << EventName << "'" << std::endl;
		Core::EvtCore->RegisterEventName( EventName );
	}

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
};
