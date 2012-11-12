#ifndef ANIM_H
#define ANIM_H

#include <SFML/Graphics.hpp>

#include "Geom.hpp"
#include "event/EventUser.hpp"
#include "ImageSet.hpp"

#include <string>
#include <memory>

/**
This is an instance of an animation, a 'view' on an animatedImage
*/
class Animation : public EventUser
{
	public:

		Animation( ImageSet& _A);

		/// Reset the animation. Sets the current frame to 1 and stops.
		void Reset();

		void SetScreenPosition( Geom::Point p );

		/// Set the animation to a specific Frame
		void SetFrame(int index);

		/// Start playing the animation by default all frames
		void Play(unsigned int from = 0, unsigned int to = 0);
		/// Starts the animation at a random frame, including a slight random time offset
		/// This is required if you start many animations in very short time, bc they would
		/// otherwise update all at the same time
		void PlayRandomized();

		/// Pauses/Unpauses the Animation
		void TogglePlay();

		/// returns wether the animation is playing right now
		bool IsPlaying();
		bool IsReverse();
		bool IsLooping();

		/// play the frames in reverse oder
		void SetReverse(bool reverse);
		void SetLooping(bool loop);

		/// Advance the animation to the next frame
		void Advance();
		/// Reverse Advance the animation to the current frame (which is the next one when playing reverse)
		void RAdvance();

		/// For listening to ticks
		virtual void HandleEvent( Event& );

		void Update( sf::Time GameTime );
		sf::Sprite& GetSprite();


	private:

		/// udate the associated sprite to show the correct sub-rect/frame of the image
		void updateSprite();

		const ImageSet& AnimData;
		sf::Sprite Sprite;

		Geom::Point Screen_Position;

		bool playing;
		bool reverse;
		bool looping;
		bool sendEndEvent;

		Geom::Vec2 CurFramePos;
		int CurFrame;

		Geom::Vec2 RangeToPlay;

		sf::Time LastUpdate;

		// small helpers

		/// called once the animation is finished, send an EVT_ANIM_FINISCH event if requested
		void Finish();

};


#endif // ANIM_H
