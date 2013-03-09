#ifndef SOUND_H
#define SOUND_H

#include <cstddef>
#include <SFML/Audio.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "util/NamedList.hpp"

#include <utility>
#include <memory>
#include <map>
#include <string>

/**
	A Soundsystem for managing currently active Sounds and volume.
	@note this is not completely implemented
*/
class SoundSystem : sf::NonCopyable
{
	public:

		SoundSystem();

		void PlayPauseSound(const std::string& name);
		void StopSound(const std::string& name);

		void SetVolume(const float v);
		float GetVolume();

		void SetBackgroundMusic(const std::string& name);

	private:

		/// Gets a sound, if the sound doesnt exist yet one is requested from the
		/// resourcemanager
		std::shared_ptr<sf::Sound> GetSound(const std::string& name);

		NamedList<sf::Sound> CurrentSounds;

};


#endif // SOUND_H
