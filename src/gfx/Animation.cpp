#include <sbe/gfx/Animation.hpp>

#include <sbe/event/Event.hpp>
#include <sbe/Module.hpp>


namespace sbe
{

	Animation::Animation(const std::shared_ptr<ImageSet>& Anim) : AnimData(Anim) {

		reset(sf::Time());
		//	Engine::out() << "Animation(" << this << ")" << std::endl;
	}

	void Animation::printState() {
		Engine::out() << "current frame - playing/looping/reverse: " << CurFrame << " - " << isPlaying() << "/" << isLooping()  << "/" << isReverse() << std::endl;
		Engine::out() << "ImageSet (Image): " << AnimData->Name << "(" << AnimData->ImageName << ")" << std::endl;
	}

	void Animation::setImageSet(const std::shared_ptr<ImageSet>& A) {
		AnimData = A;

		if (A->NumFrames == 0) {
			Engine::out() << "Animation: got invalid ImageSet '" << A->Name << "'" << std::endl;
		}

		RangeToPlay = AnimData->FrameCount;
		setFrame(0);
	}

	void Animation::reset(const sf::Time& GameTime) {
		playing = false;
		reverse = false;
		looping = false;
		ScreenPosition = {};
		sendEndEvent = false;
		setFrame(0);
		RangeToPlay = AnimData->FrameCount;
		LastUpdate = GameTime;
	}

	void Animation::setScreenPosition(glm::point2 p) {
		ScreenPosition = p;
	}

	void Animation::setFrame(unsigned int index) {
		if (index >= AnimData->NumFrames)
		{
			Engine::out(Engine::ERROR) << "Animation::Play() # Can't set Frame, requested index out of range!" << std::endl;
			return;
		}

		CurFrame = index;
		CurFramePos = AnimData->CalcFramePos(index);
	}

	void Animation::playRandomized(const sf::Time& GameTime) {
		if (AnimData->NumFrames <= 0)
		{
			Engine::out(Engine::ERROR) << "Animation::Play() # Can't play Animation '" << AnimData->Name << "', no Frames!" << std::endl;
			return;
		}

		play(0, 0);
		setFrame(std::rand()%AnimData->NumFrames);

		// this should give an random offest off maximum 1.f ( 1 second )
		// maxes updates look much more naturally
		float tmp = (float) std::rand()/static_cast<float>(RAND_MAX);
		float sec = 1.0f/(float) AnimData->FramesPerSecond;
		LastUpdate = GameTime - sf::seconds(sec*tmp);
	}

	void Animation::play(unsigned int from, unsigned int to) {
		//if (from < 1) from = 1;

		if (from >= AnimData->NumFrames || to >= AnimData->NumFrames || AnimData->NumFrames <= 0)
		{
			Engine::out(Engine::ERROR) << "Animation::Play() # Can't play Animation, requested index out of range!" << std::endl;
			return;
		}
		if (to == 0) to = AnimData->NumFrames - 1;

		RangeToPlay = {from, to};
		reverse = (from > to);
		playing = true;
		setFrame(from);
	}


	void Animation::togglePlay() {
		playing = !playing;
	}

	bool Animation::isPlaying() {
		return playing;
	}

	bool Animation::isReverse() {
		return reverse;
	}

	bool Animation::isLooping() {
		return looping;
	}

	void Animation::setLooping(bool loop) {
		looping = loop;
	}

	void Animation::setReverse(bool _reverse) {
		this->reverse = _reverse;
	}

	void Animation::advance() {
		// Finished
		if ((CurFrame == RangeToPlay.y && !looping) || !playing)
		{
			finish();
			Engine::out(Engine::SPAM) << "Anim " << AnimData->Name << " finished." << std::endl;
			reset(sf::Time());
			return;
		}
		// we're looping, reset to first frame
		setFrame((CurFrame == RangeToPlay.y) ? RangeToPlay.x : CurFrame + 1);
	}

	void Animation::rAdvance() {
		// Finished
		if ((CurFrame == RangeToPlay.x && !looping) || !playing)
		{
			finish();
			reset(sf::Time());
			return;
		}
		// we're looping, reset to first frame
		setFrame((CurFrame == RangeToPlay.x) ? RangeToPlay.y : CurFrame - 1);
	}

	void Animation::finish() {
		if (sendEndEvent)
		{
			Module::Get()->QueueEvent("EVT_ANIM_END");
		}
	}


	void Animation::update(const sf::Time& GameTime) {
		if (AnimData->FramesPerSecond == 0 || AnimData->NumFrames == 0 || !playing)
		{
			//Engine::out() << "Anim not playing: " << AnimData->Name << std::endl;
			return;
		}

		sf::Time diff = GameTime - LastUpdate;
		sf::Time FrameTime = sf::seconds(1.0f/(float) AnimData->FramesPerSecond);

		if (diff > FrameTime)
		{
//			Engine::out() << "Diff: " << diff.asMilliseconds() << std::endl;
//			Engine::out() << "FrameTime: " << FrameTime.asMilliseconds() << std::endl;
			//		Engine::out() << "AnimData: " << AnimData->ImageName << std::endl;
			//		Engine::out() << "AnimData->fps: " << AnimData->FramesPerSecond << std::endl;

			int oldframe = CurFrame;

			while (diff > FrameTime)
			{
				//Engine::out() << "Franetime: " << FrameTime.asMilliseconds() << " - Diff: " << diff.asMilliseconds() << std::endl;
				reverse ? rAdvance() : advance();
				diff -= FrameTime;
			}

			if (oldframe != CurFrame)
				updateDrawable();
		}

		LastUpdate = GameTime - diff;
	}
} // namespace sbe
