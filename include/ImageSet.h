#ifndef IMAGESET_H
#define IMAGESET_H


#include <SFML/Graphics.hpp>

#include "Geom.h"

#include "Engine.h"

#include <memory>

/**
This Class contains the Data necessary to manage an animation or just a set of
images/tiles.
*/
class ImageSet
{
	public:

		/// def. constructor
		ImageSet();

		/// constructor
		ImageSet( const std::string& _name,
				  const std::string& _imgname,
				  const Geom::Point& _startpos,
				  const Geom::Point& _destpos,
				  const Geom::Vec2&  _fsize,
				  const Geom::Vec2&  _fcount,
				  const int _fps);

		/// calculates the rectangle on the image which contains the given frame
		Geom::Rect FramePosToPixels( const Geom::Vec2 FramePos ) const;

		/// calculate the frame position in the image from a frame index
		Geom::Vec2 CalcFramePos( const int index ) const ;

		/// Creates a sprite showing a specific image of the imageset
		/// @param index the index of the frame the sprite should display
		sf::Sprite CreateSprite(const int index) const;
		std::shared_ptr<sf::Sprite> CreateSpritePtr(const int index) const;

		/// Creates a sprite showing a specific image of the imageset
		/// @param FramePos the Position of the frame the sprite should display
		sf::Sprite CreateSprite(const Geom::Vec2 FramePos) const;
		std::shared_ptr<sf::Sprite> CreateSpritePtr(const Geom::Vec2 FramePos) const;


		//## DATA members ##




		/// the name of this imageset
		std::string Name;

		/// the name of the imagefile which contains this frames data
		std::string ImageName;

		/// Top left corner of the first frame (in pixels)
		Geom::Point StartPos;
		/**
		Top left corner of the destination
		( actual usage of this varies, some imagesets e.g. for ui elements have target destinations)
		*/
		Geom::Point DestPos;

		/// the size of each frame (in pixels)
		Geom::Vec2 FrameSize;

		/// how many frames are on the image (x -> columns, y -> rows)
		Geom::Vec2 FrameCount;

		/// after how many Ticks should the next frame be played? (anim only)
		unsigned int FramesPerSecond;

		unsigned int NumFrames;
};


#endif // IMAGESET_H
