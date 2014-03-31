#include "sbe/Module.hpp"

#include <boost/thread.hpp>

// local includes
#include "event/EventCore.hpp"
#include "modules/Core.hpp"
#include "util/TickControl.hpp"
#include "../src/event/EventQueue.hpp"



namespace sbe
{

	boost::thread_specific_ptr<Module> Module::Instance;
	std::list< Module* > Module::RunningModules;
	boost::mutex Module::ModulesMutex;


	Module::Module()
	{
		//Engine::out() << "Module::Module():  this [" << this <<  "]" << std::endl;
	}

	Module::~Module()
	{
		// make sure this pointer doesnt try do delete 'this' after destructor
		//ThreadModule.release();
	}

	void Module::DebugString( const std::string& name, const std::string& value)
	{
		DbgStringEvent->Data() =  std::pair<std::string, std::string>( name, value);


		if (useEventQueue)
		{
			QueueEvent( *DbgStringEvent, true );
		}
		else
		{
			// XXX: This is hardcoded!!
			if (Name == "EventCore") Core::EvtCore->PostEventToQueue( 1 , *DbgStringEvent);
		}
	}

	//EventQueue&  Module::GetEventQueue()

	void Module::StartModule( const ModuleStartInfo& m )
	{
		this->useEventQueue = m.useEventQueue;

		DbgStringEvent.reset ( new Event("VIEW_DBG_STRING") );

		TC.reset ( new TickControl );
		Name = m.Name;
		quit = false;
		TC->Init( m.desiredTicksPerSecond, m.TickEvt );

		if (useEventQueue)
		{
			EvtQ.reset( new EventQueue() );
			QueueID = Core::EvtCore->RegisterModule (*this);
		}

		MyThread = new boost::thread( boost::bind(&Module::ThreadLocalInit, this) );
		//MyThread->detach();
	}

	EventQueue* Module::GetEventQueue() { return EvtQ.get(); };

	void Module::PostEvent( Event &e )
	{
		EvtQ->PostEvent( e );
	}

	void Module::QueueEvent( const Event& e, bool global)
	{
		EvtQ->QueueEvent( e, global );
	}

	void Module::QueueEvent( const std::string& EvtName, bool global)
	{
		EvtQ->QueueEvent( EvtName, global );
	}


	void Module::SetTPS( int TPS )
	{
		TC->SetTargetTicksPerSecond( TPS );
	}

	void Module::SetTickEvent( std::shared_ptr<Event> TickEvt )
	{
		TC->SetTickEvent( TickEvt );
	}

	void Module::ThreadLocalInit()
	{
		Instance.reset( this );
		ModuleTime.restart();

		Engine::out(Engine::INFO) << "[" << Module::Get()->GetName() << "] New module." << std::endl;

		Init();
		Execute();
		DeInit();

		if (useEventQueue) Core::EvtCore->RemoveModule(QueueID);
		Engine::out(Engine::INFO) << "[" << Module::Get()->GetName() << "] Thread/Module has exited" << std::endl;
		// without this the thread_specific_ptr would call delete on this module
		Instance.release();
	}

	void Module::Execute()
	{
		while (!quit)
		{
			TC->Tick();
		}
	}

} // namespace sbe
