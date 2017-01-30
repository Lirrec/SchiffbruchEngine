#include <sbe/gfx/actors/AnimationActor.hpp>
#include <sbe/Module.hpp>
#include <sbe/ResourceManager.hpp>

namespace sbe
{

	AnimationActor::AnimationActor(const std::shared_ptr<ImageSet>& ImgSet)
			: Anim(ImgSet) {
	}

	void AnimationActor::update(const sf::Time& RenderTime) {
		Anim.update(RenderTime);
	}

	sf::Drawable& AnimationActor::getDrawable() {
		return Anim.getSprite();
	}

	geom::rect AnimationActor::getBounds() {
		return Anim.getSprite().getGlobalBounds();
	}

	void AnimationActor::setAnimation(const std::string& name) {
		auto ImgSet = Engine::GetResMgr()->get<sbe::ImageSet>( name );

		if (!ImgSet) {
			Engine::out() << "ImageSet '" << name << "' for Player missing." << std::endl;
			return;
		}

		Anim.setImageSet(ImgSet);
		Anim.playRandomized( Module::Get()->GetModuleTime() );
		// set origin to bottom center
		Anim.getSprite().setOrigin( { ImgSet->FrameSize.x/2, ImgSet->FrameSize.y } );
		Anim.updateDrawable();
	}

} // namespace sbe