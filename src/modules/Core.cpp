#include "Core.h"

#include "event/EventCore.h"

EventCore* Core::EvtCore;

Core::Core()
{
	EvtCore = new EventCore() ;
}

Core::~Core()
{
	delete EvtCore;
}
