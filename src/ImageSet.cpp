#include "sbe/ImageSet.hpp"

#include "sbe/ResourceManager.hpp"

ImageSet::ImageSet()
 : 	Name(),
	ImageName(),
	StartPos(),
	DestPos(),
	FrameSize(),
	FrameCount(),
	FramesPerSecond(0),
	NumFrames(0)
{}

ImageSet::ImageSet(const  std::string& _name,
				 const std::string& _imgname,
				 const Geom::Point& _startpos,
				 const Geom::Point& _destpos,
				 const Geom::Vec2&  _fsize,
				 const Geom::Vec2&  _fcount,
				 const int 			_fps)
: 	Name(_name),
	ImageName(_imgname),
	StartPos(_startpos),
	DestPos(_destpos),
	FrameSize(_fsize),
	FrameCount(_fcount),
	FramesPerSecond(_fps)
{
	NumFrames = FrameCount.x() * FrameCount.y();

}




Geom::Vec2 ImageSet::CalcFramePos(const int index) const
{
	Geom::Vec2 FPos;

	int tmp = (int)std::floor((float)index / (float)FrameCount.x());

	FPos.SetX( index % FrameCount.x());
	FPos.SetY( tmp );
	return FPos;
}


Geom::Rect ImageSet::FramePosToPixels(const Geom::Vec2 FramePos) const
{
	Geom::Rect re;
	Geom::Point topleft, bottomright;

	topleft.SetX(StartPos.x() + FramePos.x() * FrameSize.x());
	topleft.SetY(StartPos.y() + FramePos.y() * FrameSize.y());

	bottomright.SetX(StartPos.x() + (FramePos.x() +1) * FrameSize.x());
	bottomright.SetY(StartPos.y() + (FramePos.y() +1) * FrameSize.y());


	re.SetX(topleft);
	re.SetY(bottomright);

	return re;
}

std::shared_ptr<sf::Sprite> ImageSet::CreateSpritePtr(const int index)
{
	//Engine::out() << "CreateSprite: " << index << std::endl;
	return CreateSpritePtr(CalcFramePos(index));
}

sf::Sprite ImageSet::CreateSprite(const int index)
{
	//Engine::out() << "CreateSprite: " << index << std::endl;
	return CreateSprite(CalcFramePos(index));
}

sf::Sprite ImageSet::CreateSprite(const Geom::Vec2 FramePos)
{
	//Engine::out() << "CreateSprite: " << FramePos << std::endl;
	sf::Sprite re;
	std::shared_ptr<sf::Image> img = Engine::GetResMgr()->get<sf::Image>(ImageName);

	if ( updateTexture() )
	{
		re.setTexture(*Tex);
		re.setTextureRect( Geom::toSFRect( FramePosToPixels(FramePos) ) );
	}
	else
	{
		Engine::out() << "ImageSet::CreateSprite: Unable to create Sprite, image not found: " << ImageName << "!" << std::endl;
	}

	return re;
}

std::shared_ptr<sf::Sprite> ImageSet::CreateSpritePtr(const Geom::Vec2 FramePos)
{
	//Engine::out() << "CreateSprite: " << FramePos << std::endl;
	std::shared_ptr<sf::Sprite> re( new sf::Sprite );

	if ( updateTexture() )
	{
		re->setTexture(*Tex);
		re->setTextureRect( Geom::toSFRect( FramePosToPixels(FramePos) ) );
	}
	else
	{
		Engine::out() << "ImageSet::CreateSprite: Unable to create Sprite, image not found: " << ImageName << "!" << std::endl;
	}

	return re;
}

bool ImageSet::updateTexture( bool force )
{
	if (!Tex || force)
	{
		std::shared_ptr<sf::Image> img = Engine::GetResMgr()->get<sf::Image>(ImageName);

		if (!img)
		{
			Engine::out() << "[ImageSet] Unable to update internal texture, Image with name " << ImageName << " doesn't exist anymore!" << std::endl;
			return false;
		}

		if (!Tex) Tex.reset ( new sf::Texture() );

		Tex->loadFromImage( *img );
	}

	return true;
}

void ImageSet::CreateQuad( const int index, sf::VertexArray& vA, sf::FloatRect& Pos, const int ArrayIndex)
{
	CreateQuad( CalcFramePos(index), vA, Pos, ArrayIndex);
}

void ImageSet::CreateQuad( const Geom::Vec2 FramePos , sf::VertexArray& vA, sf::FloatRect& Pos, const int ArrayIndex)
{
	Geom::Rect coords = FramePosToPixels(FramePos);

	sf::Vertex vs[4];
	vs[0].position = sf::Vector2f( Pos.left, Pos.top  );		// top-left
	vs[1].position = sf::Vector2f( Pos.left+Pos.width, Pos.top ); // top-right
	vs[2].position = sf::Vector2f( Pos.left+Pos.width, Pos.top+Pos.height ); // bottom-right
	vs[3].position = sf::Vector2f( Pos.left, Pos.top+Pos.height );			  // bottom-left

	vs[0].texCoords = sf::Vector2f( coords.x().x(), coords.x().y() );	// top-left
	vs[1].texCoords = sf::Vector2f( coords.y().x(), coords.x().y() ); // top-right
	vs[2].texCoords = sf::Vector2f( coords.y().x(), coords.y().y() ); // bottom-right
	vs[3].texCoords = sf::Vector2f( coords.x().x(), coords.y().y() ); // bottom-left

	if ( ArrayIndex == -1 )
	{
		vA.append( vs[0] );
		vA.append( vs[1] );
		vA.append( vs[2] );
		vA.append( vs[3] );
	}
	else
	{
		vA[ ArrayIndex ] = vs[0];
		vA[ ArrayIndex + 1 ] = vs[1];
		vA[ ArrayIndex + 2 ] = vs[2];
		vA[ ArrayIndex + 3 ] = vs[3];
	}
}

