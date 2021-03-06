#include "SoundIOPlugin.hpp"

#include <sbe/io/SFistream.hpp>

namespace sbe
{

	SoundIOPlugin::SoundIOPlugin()
			: iBinaryIOPlugin("sounds") { }

	SoundIOPlugin::~SoundIOPlugin() {

	}

	SoundIOPlugin::ObjectList SoundIOPlugin::decodeStream(std::istream& in) {
		ObjectList re;
		std::shared_ptr<sf::SoundBuffer> SB(new sf::SoundBuffer());
		sfIStream stream(in);
		if (SB->loadFromStream(stream))
		{
			re.push_back(SB);
		}

		return re;
	}

	bool SoundIOPlugin::encodeStream(const sf::SoundBuffer& o, std::ostream& out) {
		Engine::out(Engine::ERROR) << "[SoundIOPlugin] Sorry, saving not implemented!" << std::endl;
		return false;
	}

	const std::vector<std::string>& SoundIOPlugin::getSupportedFileExtensions() {
		static std::vector<std::string> fileexts{"ogg", "wav", "flac", "aiff", "au"};
		return fileexts;
	}
} // namespace sbe
