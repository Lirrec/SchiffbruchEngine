#ifndef ANIM_H
#define ANIM_H

#include "sbe/geom/Point.hpp"
#include "sbe/gfx/ImageSet.hpp"

#include <SFML/System/Time.hpp>

#include <string>
#include <memory>

namespace sbe
{
	/**
		A base class for animations.
		It supports restarting, pausing, setting the current frame, playing forwards/backwards etc.
		To get a smooth Animation update() should be called in regular intervalls with a timer which represents the time passed ingame.
		The frame numbering starts at zero.
		@see AnimatedSprite, AnimatedVertexArray
	*/
	class Animation
	{
		public:

			Animation( ImageSet& _A);
			virtual ~Animation() {}

			/// Reset the animation. Sets the current frame to 0 and stops.
			void reset( const sf::Time& GameTime );

			/// set the underlying ImageSet ( lazy construction )
			void setImageSet( ImageSet& A);

			void setScreenPosition( glm::ipoint2 p );

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

			/// is the animation playing right now
			bool isPlaying();
			/// is the Animation playing reversed
			bool isReverse();
			/// is the Animation looping
			bool isLooping();

			/// play the frames in reverse oder
			void setReverse(bool reverse);
			/// enable or disable looping ( restarting after the last frame )
			void setLooping(bool loop);

			/// Advance the animation to the next frame
			void advance();
			/// Reverse Advance the animation to the current frame (which is the next one when playing reverse)
			void rAdvance();

			/**
				Update the animation.
				@param GameTime should contain the time passed since the start of the game
			*/
			virtual void update( const sf::Time& GameTime );

			/**
				Update the drawable managed by a Animation.
				Has to be implemented by Classes deriving from Animation.
			*/
			virtual void updateDrawable() = 0;


		protected:

			/// called once the animation is finished, send an EVT_ANIM_FINISCH event if requested
			void finish();

			ImageSet& AnimData;
			glm::ipoint2 Screen_Position;

			bool playing;
			bool reverse;
			bool looping;
			bool sendEndEvent;

			glm::ivec2 CurFramePos;
			int CurFrame;

			glm::ivec2 RangeToPlay;

			sf::Time LastUpdate;

	};
} // namespace sbe


//class AnimatedVertexArray : Animation
//{
//		/// update the associated sprite/vertexarray to show the correct sub-rect/frame of the image
//		void updateArray();
//	private:
//		sf::VertexArray Arr;
//		std::vector<sf::Time> Times;
//};


#endif // ANIM_H
