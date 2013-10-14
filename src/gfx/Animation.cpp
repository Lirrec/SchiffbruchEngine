#include "sbe/gfx/Animation.hpp"

#include "sbe/event/Event.hpp"
#include "sbe/Engine.hpp"
#include "sbe/Module.hpp"
#include "sbe/geom/Point.hpp"



#include <cmath>
#include <iostream>
#include <string>

namespace sbe
{

	Animation::Animation( ImageSet& _A) : AnimData( _A )
	{

		reset( sf::Time() );
	//	Engine::out() << "Animation(" << this << ")" << std::endl;
	}

	void Animation::setImageSet( ImageSet& A)
	{
		AnimData = A;

		if ( A.NumFrames == 0 )
		{
			Engine::out() << "Animation: got invalid ImageSet '" << A.Name << "'" << std::endl;
		}
	}

	void Animation::reset( const sf::Time& GameTime )
	{
		playing = false;
		reverse = false;
		looping = false;
		Screen_Position = Geom::Point (0,0);
		sendEndEvent = false;
		CurFrame = 0;
		CurFramePos = AnimData.StartPos;
		RangeToPlay = AnimData.FrameCount;
		LastUpdate = GameTime;
	}

	void Animation::setScreenPosition( Geom::Point p )
	{
		Screen_Position = p;
	}

	void Animation::setFrame(int index)
	{
		CurFrame = index;
		CurFramePos = AnimData.CalcFramePos( index );
	}

	void Animation::play(unsigned int from, unsigned int to)
	{
		//if (from < 1) from = 1;

		if (from == to || from < 1 || to > AnimData.NumFrames || AnimData.NumFrames <= 0)
		{
			Engine::out(Engine::ERROR) << "Animation::Play() # Can't play Animation, requested index out of range!" << std::endl;
			return;
		}
		if (to == 0) to = AnimData.NumFrames;

		RangeToPlay = {from, to};
		reverse = (from > to);
		playing = true;
		CurFrame = from;
		CurFramePos = AnimData.CalcFramePos( CurFrame );
	}

	void Animation::playRandomized( const sf::Time& GameTime )
	{
		if ( AnimData.NumFrames <= 0 )
		{
			Engine::out(Engine::ERROR) << "Animation::Play() # Can't play Animation '" << AnimData.Name << "', no Frames!" << std::endl;
			return;
		}

		int from = rand() % AnimData.NumFrames;
		from ++; // this yields a frame-number from 1 to numFrames+1

		play( from, 0);
		// this should give an random offest off maximum 1.f ( 1 second )
		// maxes updates look much more naturally
		LastUpdate = GameTime - sf::seconds((float)std::rand() / static_cast<float>(RAND_MAX));
	}


	void Animation::togglePlay()
	{
		if (playing)
		{
			playing = false;
		}
		else
		{
			playing = true;
		}
	}

	bool Animation::isPlaying()
	{
		return playing;
	}

	bool Animation::isReverse()
	{
		return reverse;
	}

	bool Animation::isLooping()
	{
		return looping;
	}

	void Animation::setLooping(bool loop)
	{
		looping = loop;
	}

	void Animation::setReverse(bool _reverse)
	{
		this->reverse = _reverse;
	}

	void Animation::advance()
	{
		// Finished
		if ((CurFrame == RangeToPlay.y && !looping) || !playing )
		{
			finish();
			Engine::out(Engine::SPAM) << "Anim " << AnimData.Name << " finished." << std::endl;
			reset( sf::Time() );
			return;
		}
		// we're looping, reset to first frame
		else if (CurFrame == RangeToPlay.y)
		{
			CurFrame = RangeToPlay.x;
		}
		else
		{
			CurFrame++;
		}

		CurFramePos = AnimData.CalcFramePos( CurFrame );
	}

	void Animation::rAdvance()
	{
		// Finished
		if ((CurFrame == RangeToPlay.x && !looping) || !playing )
		{
			finish();
			reset( sf::Time() );
			return;
		}
		// we're looping, reset to last frame
		else if (CurFrame == RangeToPlay.x)
		{
			CurFrame = RangeToPlay.y;
		}
		else
		{
			CurFrame--;
		}

		CurFramePos = AnimData.CalcFramePos( CurFrame );
	}

	void Animation::finish()
	{
		if (sendEndEvent)
		{
			Event e = Event("EVT_ANIM_END");
			Module::Get()->QueueEvent(e);
		}
	}



	void Animation::update( const sf::Time& GameTime )
	{
		if (AnimData.FramesPerSecond == 0 || AnimData.NumFrames == 0 || !playing) return;

		sf::Time diff = GameTime - LastUpdate;
		sf::Time FrameTime = sf::seconds(1.0f/(float)AnimData.FramesPerSecond);

		if (diff > FrameTime)
		{
	//		Engine::out() << "Diff: " << diff << std::endl;
	//		Engine::out() << "FrameTime: " << FrameTime << std::endl;
	//		Engine::out() << "AnimData: " << AnimData.ImageName << std::endl;
	//		Engine::out() << "AnimData.fps: " << AnimData.FramesPerSecond << std::endl;

			int oldframe = CurFrame;

			while (diff > FrameTime)
			{
				//Engine::out() << "Franetime: " << FrameTime.asMilliseconds() << " - Diff: " << diff.asMilliseconds() << std::endl;
				reverse?rAdvance():advance();
				diff -= FrameTime;
			}

			if ( oldframe != CurFrame )
				updateDrawable();
		}


		LastUpdate = GameTime - diff;
	}
} // namespace sbe
