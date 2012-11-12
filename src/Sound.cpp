#include "sbe/Sound.hpp"

#include "sbe/Engine.hpp"
#include "sbe/ResourceManager.hpp"

#include <utility>

SoundSystem::SoundSystem()
{

}

std::shared_ptr<sf::Sound> SoundSystem::GetSound(const std::string& name)
{
	auto s = CurrentSounds.GetItem(name);
	if (s)
	{
		return s;
	}
	else
	{
		s = Engine::GetResMgr()->CreateSound(name);

		if (s)
		{
			return s;
		}
	}

	Engine::out() << "SoundSYS: Unable to play or load sound: " << name << std::endl;

	return s;
}


void SoundSystem::PlayPauseSound(const std::string& name)
{
	auto s = GetSound(name);
	if (s)
	{
		if (s->getStatus() == sf::Sound::Status::Paused )
		{
			s->play();
		}
		else
		{
			s->pause();
		}
	}
}

void SoundSystem::StopSound(const std::string& name)
{
	auto s = GetSound(name);
	if (s)
	{
		s->stop();
	}
}

void SoundSystem::SetVolume(const float v)
{
	//TODO: find a way to add a main/all volume bar

}

float SoundSystem::GetVolume()
{
	//TODO: find a way to add a main/all volume bar
	return -1.0;
}

void SoundSystem::SetBackgroundMusic(const std::string& name)
{
	auto s = GetSound(name);
	if (s)
	{
		s->play();
	}

}
