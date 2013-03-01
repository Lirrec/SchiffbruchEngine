#ifndef ANIM_H
#define ANIM_H

#include "sbe/geom/Point.hpp"
#include "sbe/gfx/ImageSet.hpp"

#include <SFML/System/Time.hpp>

#include <string>
#include <memory>

/**
	This is an base class for animations.
	@see AnimatedSprite, AnimatedVertexArray
*/
class Animation
{
	public:

		Animation( ImageSet& _A);
		virtual ~Animation();

		/// Reset the animation. Sets the current frame to 1 and stops.
		void reset( const sf::Time& GameTime );

		void setScreenPosition( Geom::Point p );

		/// Set the animation to a specific Frame
		void setFrame(int index);

		/// Start playing the animation by default all frames
		void play(unsigned int from = 0, unsigned int to = 0);
		/// Starts the animation at a random frame, including a slight random time offset
		/// This is required if you start many animations in very short time, bc they would
		/// otherwise update all at the same time
		void playRandomized( const sf::Time& GameTime );

		/// Pauses/Unpauses the Animation
		void togglePlay();

		/// returns wether the animation is playing right now
		bool isPlaying();
		bool isReverse();
		bool isLooping();

		/// play the frames in reverse oder
		void setReverse(bool reverse);
		void setLooping(bool loop);

		/// Advance the animation to the next frame
		void advance();
		/// Reverse Advance the animation to the current frame (which is the next one when playing reverse)
		void rAdvance();

		virtual void update( const sf::Time& GameTime );
		virtual void updateDrawable() = 0;


	protected:

		/// called once the animation is finished, send an EVT_ANIM_FINISCH event if requested
		void finish();

		const ImageSet& AnimData;
		Geom::Point Screen_Position;

		bool playing;
		bool reverse;
		bool looping;
		bool sendEndEvent;

		Geom::Vec2 CurFramePos;
		int CurFrame;

		Geom::Vec2 RangeToPlay;

		sf::Time LastUpdate;

};


//class AnimatedVertexArray : Animation
//{
//		/// update the associated sprite/vertexarray to show the correct sub-rect/frame of the image
//		void updateArray();
//	private:
//		sf::VertexArray Arr;
//		std::vector<sf::Time> Times;
//};


#endif // ANIM_H
