#ifndef TICKCONTROL_H
#define TICKCONTROL_H

#include <memory>

#include <SFML/System/Clock.hpp>

namespace sbe
{

	/**
		This class provides simple functionality for running a tick-based eventloop.
		It tries to limit ticks to a given frequency and will sleep if there is time left.

	 	Simple call StartTick() and EndTick() before and after your Tick logic and the TickControl will provide FrameStatistics and will sleep if the desired framerate is reached
	*/
	class TickControl
	{
	public:
		/**
			Init method
			@param TPS the desired ticks per second
		*/
		void Init(int TPS);

		/**
			Set the desired amount of TicksPerSecond
		*/
		void SetTargetTicksPerSecond(int TPS);

		/**
			Do a tick.
			This usually sends a TickEvent to this threads eventqueue.
		*/
		void StartTick();

		/**
		 * End a Tick, logs stats and sleeps if the Tick needed less time than available at the current Framerate
		 */
		void EndTick();

	private:

		/**
			Logs tick information.
			Sends information about the current fps, ms taken per tick and ms slept per tick via the DEBUG_STRING Event.
		*/
		void LogModuleStats();

		/// Sleep the rest of the current tick
		void YieldTickRest();

		/// How many eventqueue ticks per second should this module try to reach
		int TicksPerSecond;
		/// How long took us the last tick (ms)
		float LastTickDuration;
		/// How many ms did we lose through ticks that took longer then planned
		float Lag;

		/// how many ms are spare till we should start the next tick
		float MsToNextTick;
		/// how long should a tick at maximum take to reach our TPS (ms)
		float MaxTickDuration;

		/// Counts Frames/Ticks
		int TickCounter;

		/// last time we reported our stats
		sf::Clock LastStatsLog;
		/// counts how long each Tick takes
		sf::Clock Timer;
	};

} // namespace sbe
#endif // TICKCONTROL_H
