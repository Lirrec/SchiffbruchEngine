#ifndef RESMGR_H
#define RESMGR_H

#include <string>
#include <memory>

// fixes some ptrdiff_t not defined errors
#include <cstddef>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Animation.h"

#include "util/NamedList.h"

/** This struct is returned when parsing files with lists of resources.
This applies to images, fonts and audio, which only need a path to be loaded.
More complex Resources ( atm only the animations) are parsed seperately
*/

struct Resource
{
	enum {
		RES_IMAGE = 0,
		RES_FONT,
		RES_SOUND
	} ResType;

	std::string Path;
};

class ResourceManager
{

	public:
		ResourceManager();
		~ResourceManager();


		// - Loading -

		/// try to load the given font
		bool LoadFont ( const std::string& path, const std::string& name );

		/// try to load the given image
		bool LoadTexture ( const std::string& path, const std::string& name );

		/// try to load the given audio file ( stored in an audiobuffer
		bool LoadAudio( const std::string& path, const std::string& name );


		// - Creation -

		/// loads the data required for an animation (ImageSet class)
		bool CreateImageSet ( const std::string& name, const std::string& from_image, const Geom::Vec2 StartPos, const Geom::Vec2 TileSize, const Geom::Vec2 TileCounts, const int fps );

		/// Adds an ImageSet to the list of Animations, loading the Image if required
		bool AddImageSet ( const ImageSet& I );

		/// creates a new instance of an animation based on the given name of the ImageSet
		std::shared_ptr<Animation> 	CreateAnimation ( const std::string& from_name ) const;
		/// creates a new instance of a Sound based on the given name of the SoundBuffer
		std::shared_ptr<sf::Sound> 	CreateSound(const std::string& from_name) const;
		/// creates a new Music based on the given name of the SoundBuffer
		std::shared_ptr<sf::Music> 	CreateMusic(const std::string& from_name) const;

		// - Requesting -

		/// returns a loaded Font or an invalid shared_ptr
		std::shared_ptr<sf::Font> 	GetFont(const std::string& name) const;

		/// returns a loaded Image or an invalid shared_ptr
		std::shared_ptr<sf::Texture> 	GetTexture(const std::string& name) const;

		/// returns a loaded ImageSet or an invalid shared_ptr
		std::shared_ptr<ImageSet> 	GetImageSet ( const std::string& name ) const;

		/// returns a loaded SoundBuffer or an invalid shared_ptr
		std::shared_ptr<sf::SoundBuffer> 	GetAudio(const std::string& name) const;

		static const sf::Font& getDefaultFont();

	private:

		/// dumps all internal namedlists
		void DumpDebugInfo();

		//resources
		NamedList<sf::Texture> 		Textures;
		NamedList<sf::Font>			Fonts;
		NamedList<ImageSet>			ImageSets;
		NamedList<sf::SoundBuffer> 	Sounds;
		NamedList<sf::Music> 		Tracks;

		/// used memory in mebibytes (rough estimation)
		double usedmemory;
};

#endif // RESMGR_H

