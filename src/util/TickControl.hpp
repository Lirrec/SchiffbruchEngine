#ifndef TICKCONTROL_H
#define TICKCONTROL_H

#include <memory>

#include "sbe/event/Event.hpp"
#include <SFML/System/Clock.hpp>


namespace sbe
{

	/**
		This class provides simple functionality for running a tick-based eventloop.
		It tries to call the Eventloop in the given frequency and will sleep if there is time left.
		You may specify a TickEvent which will be sent by default with each eventloop.
		An undefined TickEvent will not be sent.
	*/
	class TickControl
	{
		public:
			/**
				Init method
				@param TPS the desired ticks per second
				@param TickEvt the desired Event to be set each tick
			*/
			void Init( int TPS, std::shared_ptr<Event> TickEvt );

			/**
				Set the desired amount of TicksPerSecond
			*/
			void SetTargetTicksPerSecond( int TPS );

			/**
				Specify which event should be sent as TickEvent.
				An invalid/empty shared_ptr defaults to no Event.
			*/
			void SetTickEvent( std::shared_ptr<Event> TickEvt );

			/**
				Do a tick.
				This usually sends a TickEvent to this threads eventqueue.
			*/
			void Tick();

		private:

			/**
				Logs tick information.
				Sends information about the current fps, ms taken per tick and ms slept per tick via the DEBUG_STRING Event.
			*/
			void LogModuleStats();

			/// Sleep the rest of the current tick
			void YieldTickRest();

			/// This event will be sent locally each tick
			std::shared_ptr<Event> TickEvent;

			/// How many eventqueue ticks per second should this module try to reach
			int 	TicksPerSecond;
			/// How long took us the last tick (ms)
			float	LastTickDuration;
			/// How many ms did we lose through ticks that took longer then planned
			float Lag;

			/// how many ms are spare till we should start the next tick
			float		MsToNextTick;
			/// how long should a tick at maximum take to reach our TPS (ms)
			float	MaxTickDuration;

			/// Counts Frames/Ticks
			int TickCounter;

			/// last time we reported our stats
			sf::Clock LastStatsLog;
			/// counts how long each Tick takes
			sf::Clock Timer;
	};

} // namespace sbe
#endif // TICKCONTROL_H
