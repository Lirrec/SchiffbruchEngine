#ifndef IMAGESET_H
#define IMAGESET_H


#include <SFML/Graphics.hpp>

#include "sbe/geom/Point.hpp"

#include "sbe/Engine.hpp"

#include <memory>

namespace sbe
{
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

			/// calculates the rectangle on the image which contains the given frame ( 2d-framepos )
			Geom::Rect CalcTexCoords( const Geom::Vec2 FramePos ) const;
			/// calculates the rectangle on the image which contains the given frame ( frame-index )
			Geom::Rect CalcTexCoords( const int index ) const;

			/// calculate the frame position in the image from a frame index
			Geom::Vec2 CalcFramePos( const int index ) const ;

			/// Creates a sprite showing a specific image of the imageset
			/// @param index the index of the frame the sprite should display
			sf::Sprite CreateSprite(const int index);
			std::shared_ptr<sf::Sprite> CreateSpritePtr(const int index);

			/// Creates a sprite showing a specific image of the imageset
			/// @param FramePos the Position of the frame the sprite should display
			sf::Sprite CreateSprite(const Geom::Vec2 FramePos);
			std::shared_ptr<sf::Sprite> CreateSpritePtr(const Geom::Vec2 FramePos);

			/**
				Add a given Image from the ImageSet to a VertexArray.
				@param index index of the image to add to the vertexarray
				@param vA the VertexArray which is modified
				@param Pos pos
				@param ArrayIndex the position in the vertexarray which sould be used ( -1/default will append to the array, other values will overwrite existing vertices from the given position on)
			*/
			void CreateQuad( const int index , sf::VertexArray& vA, const sf::FloatRect& Pos, const int ArrayIndex = -1, const sf::Color& _color = sf::Color::Black);

			/**
				Add a given Image from the ImageSet to a VertexArray.
				@param FramePos The position of the image/frame on the ImageSet to add to the vertexarray
				@param vA the VertexArray which is modified
				@param Pos pos
				@param ArrayIndex the position in the vertexarray which sould be used ( -1/default will append to the array, other values will overwrite existing vertices from the given position on)
			*/
			void CreateQuad( const Geom::Vec2 FramePos, sf::VertexArray& vA, const sf::FloatRect& Pos, const int ArrayIndex = -1, const sf::Color& _color = sf::Color::Black);

			/**
				Updates the internal pointer to sf::Texture, call to update sprites after the image was changed
				@param force set true to force an update ( default behaviour is to just load the texture once )
				@return false on error
			*/
			bool updateTexture( bool force=false );

			/// access the texture managed by the ImageSet
			std::shared_ptr<sf::Texture> getTexture();

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

			/// the number of frames in the ImageSet, usuallly calculated from FrameCount
			unsigned int NumFrames;

		private:

			std::shared_ptr<sf::Texture> Tex;
	};

} // namespace sbe
#endif // IMAGESET_H
