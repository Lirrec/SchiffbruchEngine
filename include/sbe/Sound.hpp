#ifndef SOUND_H
#define SOUND_H

#include <cstddef>
#include <SFML/Audio.hpp>

#include "util/NamedList.hpp"

#include <utility>
#include <memory>
#include <map>
#include <string>

class SoundSystem
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
