#include "TickControl.hpp"

#include <boost/lexical_cast.hpp>

#include "sbe/Module.hpp"


#include <boost/format.hpp>
#include "../event/EventCore.hpp"



namespace sbe
{

	void TickControl::Init(int TPS) {
		TicksPerSecond = TPS;
		LastTickDuration = 0;
		MsToNextTick = 0;
		MaxTickDuration = (1.0/TicksPerSecond)*1000;
		TickCounter = 0;
		Lag = 0;
	}

	void TickControl::SetTargetTicksPerSecond(int TPS) {
		TicksPerSecond = TPS;
		MaxTickDuration = (1.0/TicksPerSecond)*1000;
	}

	void TickControl::StartTick() {
		Timer.restart();
	}

	void TickControl::EndTick() {
		// Post module info to the gameview (debug)
		LogModuleStats();
		TickCounter++;
		// Sleep if theres time left
		YieldTickRest();
	}

	void TickControl::YieldTickRest() {

		LastTickDuration = Timer.getElapsedTime().asMilliseconds();
		if (TicksPerSecond == 0) return;

		// check if we wait till next tick
		if (LastTickDuration < MaxTickDuration)
		{
			MsToNextTick = MaxTickDuration - LastTickDuration;
			if (Lag <= MsToNextTick)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(MsToNextTick - Lag));
				Lag = 0;
			}
			else
				Lag -= MsToNextTick;
		}
		else
		{
			MsToNextTick = 0;
			Lag += LastTickDuration - MaxTickDuration;
			//if (LastTickDuration > MaxTickDuration*2 )
			//	Engine::out(Engine::SPAM) << "[" << Module::Get()->GetName() << "] Slow! " << str(format("[ %.2g/%.2gms, %.2gms Lag]")  % LastTickDuration % MaxTickDuration % Lag ) << std::endl;
		}
	}

	void TickControl::LogModuleStats() {
		using boost::format;
		using boost::io::group;

		if (LastStatsLog.getElapsedTime() > sf::seconds(1))
		{
			Module::Get()->DebugString(
				str(format("Tick [%s]")
		        	% Module::Get()->GetName()),
				str(format(" %.2g+%.2g / %.2g")
					% LastTickDuration
					% MsToNextTick
                    % MaxTickDuration));
			Module::Get()->DebugString(
				str(format("FPS [%s]")
			     	% Module::Get()->GetName()),
				str(format("%.1g")
					% TickCounter));

			Module::Get()->DebugString(
				"Events [%s] " + Module::Get()->GetName(),
				str(format("%d")
				    %(Module::Get()->EventQueueEnabled()
				    	? Module::Get()->GetEventQueue()->GetEventCount()
						: EventCore::getInstance()->GetEventCount())));

			Lag = (LastStatsLog.getElapsedTime() - sf::seconds(1)).asMilliseconds();
			TickCounter = 0;
			LastStatsLog.restart();
		}
	}

} // namespace sbe
