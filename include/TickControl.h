#ifndef TICKCONTROL_H
#define TICKCONTROL_H

#include <memory>

#include "event/Event.h"

/** This class provides simple functionality for a default Tick implementation.
It tries to call the Eventloop in the frequency and will sleep if there is time left.
You may specify a TickEvent which will be sent by default with each eventloop.

*/
class TickControl
{
	public:
		void Init( int TPS, std::shared_ptr<Event> TickEvt );
		void SetTargetTicksPerSecond( int TPS );
		void SetTickEvent( std::shared_ptr<Event> TickEvt );
		void Tick();

	private:

		void LogModuleStats();
		void YieldTickRest();

		/// This event will be sent locally each tick
		std::shared_ptr<Event> TickEvent;

		/// How many eventqueue ticks per second should this module try to reach
		int 	TicksPerSecond;
		/// How long took us the last tick (ms)
		int	LastTickDuration;

		/// how many ms are spare till we should start the next tick
		int		MsToNextTick;
		/// how long should a tick at maximum take to reach our TPS (ms)
		int	MaxTickDuration;

		/// Counts Frames/Ticks
		int TickCounter;

		/// last time we reported our stats
		sf::Clock LastStatsLog;
		/// counts how long each Tick takes
		sf::Clock Timer;
};

#endif // TICKCONTROL_H

