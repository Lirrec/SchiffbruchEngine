#ifndef SCHIFFBRUCHENGINE_SIMPLEMODULE_HPP_H
#define SCHIFFBRUCHENGINE_SIMPLEMODULE_HPP_H

#include <sbe/Module.hpp>
#include <tuple>
#include <memory>

namespace sbe
{
	/**
	 * Helper Class for creating Modules.
	 * This class creates a Module which will take its template parameters and instantiate each parameter as a local member in the Module ( in the LocalInit() function).
	 * The instances will be deleted in the DeInit() Callback. This allows to create simple Modules
	 * which only instantiate a few classes where those classes then register for events and essentially are self-sustained.
	 * It is possible to supply callables to execute additional logic in the LocalInit/Init/DeInit phases ( e.g. call some methods on the members to configure them properly ).
	 *
	 * Example ( this would appear inside the sbe::GameBase::ModuleInit function of some game implementation ):
	 * @code
		auto renderer = new sbe::SimpleModule<sbe::Screen, sbe::DebugWindowr>(
			//LocalInit Callback
			[](auto& tuple){
				auto& Scr = std::get<std::unique_ptr<sbe::Screen>>(tuple);
				Scr->getRenderer()->addLayer( {sbe::Screen::sCam()} );
				Scr->getCam()->enableScrolling( false );
				Scr->setClearColor( sf::Color(0,0,0,255) );
				Module::Get()->QueueEvent("TOGGLE_SHOW_DBGWIN", true);
		});
	 * @endcode
	 *
	 * This creates a module which will instantiate a sbe::Screen and on sbe::DebugWindow in its LocalInit() Method. Additionally the first parameter/lambda
	 * will add a new Renderlayer to the Renderer and set some other properties.
	 */
	template <class ...Args>
	class SimpleModule : public sbe::Module
	{
		using MemberTuple = std::tuple<std::unique_ptr<Args>...>;
		using Callback = std::function<void(MemberTuple&)>;
	public:

		SimpleModule() = default;

		SimpleModule( Callback _Init, Callback _DeInit = [](auto& t){}, Callback _LocalInit = [](auto& t){} )
			: Initializer( _Init),
			  DeInitializer(_DeInit),
			  LocalInitializer(_LocalInit)
		{}

		template<class T>
		std::unique_ptr<T> getMember() {
			return std::get<T>(members);
		}

	private:
		virtual void LocalInit() override {
			members = { (std::make_unique<Args>())... };
			if (LocalInitializer) LocalInitializer(members);
		}

		virtual void Init() override {
			if (Initializer) Initializer(members);
		}

		virtual void DeInit() override {
			if (DeInitializer) DeInitializer(members);
			members = {};
		}


	private:
		MemberTuple members;

		Callback Initializer;
		Callback DeInitializer;
		Callback LocalInitializer;

	};
}



#endif //SCHIFFBRUCHENGINE_SIMPLEMODULE_HPP_H
