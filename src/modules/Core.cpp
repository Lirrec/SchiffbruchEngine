#include "Core.hpp"

#include "../event/EventCore.hpp"

EventCore* Core::EvtCore;

Core::Core()
{
	EvtCore = new EventCore() ;
}

Core::~Core()
{
	delete EvtCore;
}
