#include "sbe/gfx/ImageSet.hpp"

#include "sbe/geom/SfmlHelpers.hpp"


#include "sbe/ResourceManager.hpp"
#include "sbe/gfx/VertexUtils.hpp"

namespace sbe
{
	ImageSet::ImageSet()
			: Name(),
			  ImageName(),
			  StartPos(),
			  DestPos(),
			  FrameSize(),
			  FrameCount(),
			  FramesPerSecond(0),
			  NumFrames(0) { }

	ImageSet::ImageSet(const std::string& _name,
					   const std::string& _imgname,
					   const glm::ipoint2& _startpos,
					   const glm::ipoint2& _destpos,
					   const glm::ivec2& _fsize,
					   const glm::ivec2& _fcount,
					   const int _fps)
			: Name(_name),
			  ImageName(_imgname),
			  StartPos(_startpos),
			  DestPos(_destpos),
			  FrameSize(_fsize),
			  FrameCount(_fcount),
			  FramesPerSecond(_fps) {
		NumFrames = FrameCount.x*FrameCount.y;

	}


	void ImageSet::calcNumFrames() {
		NumFrames = FrameCount.x*FrameCount.y;
	}

	glm::ivec2 ImageSet::CalcFramePos(const int index) const {
		glm::ivec2 FPos;
		FPos.x = index%FrameCount.x;
		FPos.y = (int) ((float) index/(float) FrameCount.x);
		return FPos;
	}

	geom::irect ImageSet::CalcTexCoords(const int index) const {
		return CalcTexCoords(CalcFramePos(index));
	}

	geom::irect ImageSet::CalcTexCoords(const glm::ivec2 FramePos) const {
		geom::irect re;
		glm::ipoint2 topleft, bottomright;

		topleft.x = StartPos.x + FramePos.x*FrameSize.x;
		topleft.y = StartPos.y + FramePos.y*FrameSize.y;

		bottomright.x = StartPos.x + (FramePos.x + 1)*FrameSize.x;
		bottomright.y = StartPos.y + (FramePos.y + 1)*FrameSize.y;


		re.x = topleft;
		re.y = bottomright;

		return re;
	}

	std::shared_ptr<sf::Sprite> ImageSet::CreateSpritePtr(const int index) {
		//Engine::out() << "CreateSprite: " << index << std::endl;
		return CreateSpritePtr(CalcFramePos(index));
	}

	sf::Sprite ImageSet::CreateSprite(const int index) {
		//Engine::out() << "CreateSprite: " << index << std::endl;
		return CreateSprite(CalcFramePos(index));
	}

	sf::Sprite ImageSet::CreateSprite(const glm::ivec2 FramePos) {
		//Engine::out() << "CreateSprite: " << FramePos << std::endl;
		sf::Sprite re;

		if (updateTexture())
		{
			re.setTexture(*Tex);
			re.setTextureRect(geom::toSFRect(CalcTexCoords(FramePos)));
		}
		else
		{
			Engine::out(Engine::ERROR) << "ImageSet::CreateSprite: Unable to create Sprite, image not found: " << ImageName << "!" << std::endl;
		}

		return re;
	}

	std::shared_ptr<sf::Sprite> ImageSet::CreateSpritePtr(const glm::ivec2 FramePos) {
		//Engine::out() << "CreateSprite: " << FramePos << std::endl;
		std::shared_ptr<sf::Sprite> re(new sf::Sprite);

		if (updateTexture())
		{
			re->setTexture(*Tex);
			re->setTextureRect(geom::toSFRect(CalcTexCoords(FramePos)));
		}
		else
		{
			Engine::out(Engine::ERROR) << "ImageSet::CreateSprite: Unable to create Sprite, image not found: " << ImageName << "!" << std::endl;
		}

		return re;
	}

	bool ImageSet::updateTexture(bool force) {
		if (!Tex || force)
		{
			std::shared_ptr<sf::Image> img = Engine::GetResMgr()->get<sf::Image>(ImageName);

			if (!img)
			{
				Engine::out(Engine::ERROR) << "[ImageSet] Unable to update internal texture, Image with name " << ImageName << " doesn't exist anymore!" << std::endl;
				return false;
			}

			if (!Tex) Tex.reset(new sf::Texture());

			if (Tex->loadFromImage(*img))
			{
				Engine::GetResMgr()->add<sf::Texture>(Tex, ImageName);
				Engine::out(Engine::INFO) << "[ImageSet] Created texture for ImageSet " << Name << ", from Image with name " << ImageName << std::endl;
			}
			else
			{
				Engine::out(Engine::ERROR) << "[ImageSet] ERROR creating texture for ImageSet " << Name << ", from Image with name " << ImageName << std::endl;
				Tex.reset();
			}
		}

		return true;
	}

	std::shared_ptr<sf::Texture> ImageSet::getTexture() {
		updateTexture();
		return Tex;
	}


	void ImageSet::CreateQuad(const int index, sf::VertexArray& vA, const sf::FloatRect& Pos, const int ArrayIndex, const sf::Color& _color) {
		CreateQuad(CalcFramePos(index), vA, Pos, ArrayIndex, _color);
	}

	void ImageSet::CreateQuad(const glm::ivec2 FramePos, sf::VertexArray& vA, const sf::FloatRect& Pos, const int ArrayIndex, const sf::Color& _color) {
		updateTexture();
		geom::irect coords = CalcTexCoords(FramePos);
		if (ArrayIndex == -1)
			gfx::AppendQuad(vA, Pos, coords, _color);
		else
			gfx::UpdateQuad(vA, Pos, coords, ArrayIndex, _color);
	}
} // namespace sbe
