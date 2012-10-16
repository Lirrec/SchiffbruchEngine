#include "Animation.h"

#include <SFML/Graphics.hpp>

#include "Engine.h"

#include "ResourceManager.h"
#include "event/EventQueue.h"
#include "Geom.h"

#include <cmath>
#include <iostream>
#include <string>


// ########################################################################################################
// ########################################################################################################
// ################################				Animation				###################################
// ########################################################################################################
// ########################################################################################################

Animation::Animation( ImageSet& _A) : AnimData(_A)
{
	std::shared_ptr<sf::Texture> txt = Engine::GetResMgr()->GetTexture(AnimData.ImageName);
	if (txt) {
		Sprite.setTexture(*txt);
	}

	Reset();
//	Engine::out() << "Animation(" << this << ")" << std::endl;
};

void Animation::Reset()
{
	playing = false;
	reverse = false;
	looping = false;
	Screen_Position = Geom::Point (0,0);
	sendEndEvent = false;
	CurFrame = 0;
	CurFramePos = AnimData.StartPos;
	RangeToPlay = AnimData.FrameCount;
	Sprite.setTextureRect( Geom::toSFRect( AnimData.FramePosToPixels(CurFramePos) ) );
	LastUpdate = Module::Get()->GetModuleTime();
}

void Animation::SetScreenPosition( Geom::Point p )
{
	Screen_Position = p;
	Sprite.setPosition( Screen_Position.x(), Screen_Position.y() );
}

void Animation::SetFrame(int index)
{
	CurFrame = index;
	CurFramePos = AnimData.CalcFramePos( index );
}

void Animation::Play(unsigned int from, unsigned int to)
{
	//if (from < 1) from = 1;

	if (from == to || from < 1 || to > AnimData.NumFrames)
	{
		Engine::out() << "Animation::Play() # Can't play Animation, requested index out of range!" << std::endl;
		return;
	}
	if (to == 0) to = AnimData.NumFrames;

	RangeToPlay = {from, to};
	reverse = (from > to);
	playing = true;
	CurFrame = from;
	CurFramePos = AnimData.CalcFramePos( CurFrame );
}

void Animation::PlayRandomized()
{
	int from = rand() % AnimData.NumFrames;
	from ++; // this yields a frame-number from 1 to numFrames+1

	Play( from, 0);
	// this should give an random offest off maximum 1.f ( 1 second )
	// maxes updates look much more naturally
	LastUpdate = Module::Get()->GetModuleTime() - sf::seconds(std::rand() / static_cast<double>(RAND_MAX));
}


void Animation::TogglePlay()
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

bool Animation::IsPlaying()
{
	return playing;
}

bool Animation::IsReverse()
{
	return reverse;
}

bool Animation::IsLooping()
{
	return looping;
}

void Animation::SetLooping(bool loop)
{
	looping = loop;
}

void Animation::SetReverse(bool reverse)
{
	this->reverse = true;
}

void Animation::Advance()
{
	// Finished
	if ((CurFrame == RangeToPlay.y() && !looping) || !playing )
	{
		Finish();
		Engine::out() << "Anim " << AnimData.Name << " finished." << std::endl;
		Reset();
		return;
	}
	// we're looping, reset to first frame
	else if (CurFrame == RangeToPlay.y())
	{
		CurFrame = RangeToPlay.x();
	}
	else
	{
		CurFrame++;
	}

	CurFramePos = AnimData.CalcFramePos( CurFrame );
}

void Animation::RAdvance()
{
	// Finished
	if ((CurFrame == RangeToPlay.x() && !looping) || !playing )
	{
		Finish();
		Reset();
		return;
	}
	// we're looping, reset to last frame
	else if (CurFrame == RangeToPlay.x())
	{
		CurFrame = RangeToPlay.y();
	}
	else
	{
		CurFrame--;
	}

	CurFramePos = AnimData.CalcFramePos( CurFrame );
}

void Animation::Finish()
{
	if (sendEndEvent)
	{
		Event e = Event("EVT_ANIM_END", GetID());
		Module::Get()->QueueEvent(e);
	}
}


void Animation::updateSprite()
{
	Geom::Rect subrect = AnimData.FramePosToPixels(CurFramePos);
	Sprite.setTextureRect(Geom::toSFRect(subrect));
}

void Animation::Update( sf::Time GameTime )
{
	sf::Time diff = GameTime - LastUpdate;
	sf::Time FrameTime = sf::seconds(1.0f/AnimData.FramesPerSecond);

	if (diff > FrameTime)
	{
//		Engine::out() << "Diff: " << diff << std::endl;
//		Engine::out() << "FrameTime: " << FrameTime << std::endl;
//		Engine::out() << "AnimData: " << AnimData.ImageName << std::endl;
//		Engine::out() << "AnimData.fps: " << AnimData.FramesPerSecond << std::endl;

		while (diff > FrameTime)
		{
			//Engine::out() << "Diff: " << diff << std::endl;
			reverse?RAdvance():Advance();
			diff -= FrameTime;
		}

		updateSprite();
	}


	LastUpdate = GameTime - diff;
}

sf::Sprite& Animation::GetSprite()
{
	return Sprite;
}

void Animation::HandleEvent( Event& e )
{
//	if ( e.Is("EVT_TICK") && playing)
//	{
//		Update( Module::Get()->GetModuleTime() );
//	}
}

