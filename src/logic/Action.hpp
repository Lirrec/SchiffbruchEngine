#ifndef ACTION_H
#define ACTION_H

#include <string>
#include "sbe/event/EventUser.hpp"

namespace sbe
{

	/**

		Short overview of the Action System:

		Actions manage the gamelogic.

		//No more than one Action from the "main" logic should be active at any given time.
		I haven't decided this yet, it's probably a good idea to allow multiple simultaneos actions


		Actions are mostly simple state machines. They are pretty dynamic as states are managed as std::function objects.
		They can be defined e.g. by c++11 lambda functions.

		Actions include some management classes/states (implementation yet to be defined) , including Triggers and Rewards.
		Both are pre-defined and reusable states for simple reoccuring tasks.

		Triggers are predefined states which wait for simple conditions and activate other states.
		Wether they trigger is evaluated once per logic-tick.
		Multiple triggers should be allowed to be active at any time, so an action can be triggered by different 'events'
		Additionally one should enable/disable them from states.

		Rewards are predefined states which complete "game-management related" actions
		Management of triggers and rewards.




		Planned Triggers:

		- SFMLEvent-Triggered:
			Mouse in/out, click,  XXX something like drag&drop?

		- Event-Triggered:

		-- Game related triggers start here --


		Rewards:

		- Event which is sent
		- Sound which is played, possibly with a 2d position
		- play Animation on an entity

		-- Game related Rewards start here

		- Loot-Reward: adds items to the players inventory, item can be specified with min/max values
		- Health/Hunger/thirst-Reward: changes the players health/hunger/thirst values
		- Time-Reward: modifies the in-game time of day.

		- object-Modification: e.g. remove berries from bush
		- Object-Removal: remove tree after chucking
		- Object-Creation: add buildings

	*/

	typedef std::function< void(std::string laststate) > StateFunction;

	class Action : EventUser
	{
		public:
			// Starts the first state
			void Execute();

			void IsRunning();

			// State functions, add new states
			void AddState ( const std::string& name, StateFunction state_function );

		private:

			// - Functions intended to be called from the states

			/// switch to a named state
			void SwitchState( std::string newstate );
			/// switch to the next state in queue
			void NextState();
			/// switch to the previous state in queue
			void PreviousState();

			/// the name of this action
			std::string name;

			/// TODO: change this to a numeric identifier for faster lookup?
			std::string current_state;

			std::map< std::string, StateFunction > States;

	};


	/// Binds the execution of an action to a specific keypress
	//class KeyBinder : SFMLEventListener
	//{
	//	public:
	//
	//		KeyBinder(Action& _a) : a(_a)
	//		{
	//
	//		}
	//
	//		virtual bool HandleEvent( sf::Event& )
	//		{
	//
	//			a.Execute();
	//			return true;
	//		}
	//
	//	private:
	//		Action& a;
	//};

} // namespace sbe
#endif // ACTION_H
