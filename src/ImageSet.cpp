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
{};

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

std::shared_ptr<sf::Sprite> ImageSet::CreateSpritePtr(const int index) const
{
	//Engine::out() << "CreateSprite: " << index << std::endl;
	return CreateSpritePtr(CalcFramePos(index));
}

sf::Sprite ImageSet::CreateSprite(const int index) const
{
	//Engine::out() << "CreateSprite: " << index << std::endl;
	return CreateSprite(CalcFramePos(index));
}

sf::Sprite ImageSet::CreateSprite(const Geom::Vec2 FramePos) const
{
	//Engine::out() << "CreateSprite: " << FramePos << std::endl;
	sf::Sprite re;
	std::shared_ptr<sf::Texture> txt = Engine::GetResMgr()->get<sf::Texture>(ImageName);

	if (txt)
	{
		re.setTexture(*txt);
		re.setTextureRect( Geom::toSFRect( FramePosToPixels(FramePos) ) );
	}
	else
	{
		Engine::out() << "ImageSet::CreateSprite: Unable to create Sprite, image not found: " << ImageName << "!" << std::endl;
	}

	return re;
}

std::shared_ptr<sf::Sprite> ImageSet::CreateSpritePtr(const Geom::Vec2 FramePos) const
{
	//Engine::out() << "CreateSprite: " << FramePos << std::endl;
	std::shared_ptr<sf::Sprite> re( new sf::Sprite );
	std::shared_ptr<sf::Texture> txt = Engine::GetResMgr()->get<sf::Texture>(ImageName);

	if (txt)
	{
		re->setTexture(*txt);
		re->setTextureRect( Geom::toSFRect( FramePosToPixels(FramePos) ) );
	}
	else
	{
		Engine::out() << "ImageSet::CreateSprite: Unable to create Sprite, image not found: " << ImageName << "!" << std::endl;
	}

	return re;
}
