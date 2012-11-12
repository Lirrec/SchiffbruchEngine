#include "sbe/ResourceManager.hpp"

ResourceManager::ResourceManager()
 : usedmemory(0)
{
	sf::Font* f = new sf::Font(getDefaultFont());
	Fonts.AddItem("default", f );
}

ResourceManager::~ResourceManager()
{

	DumpDebugInfo();
}

void ResourceManager::DumpDebugInfo()
{
	Engine::out() << "# -------- Dumping Resources -------- #" << std::endl;
	Engine::out() << "Fonts:" << std::endl;
	Fonts.DebugDump();
	Engine::out() << "Textures:" << std::endl;
	Textures.DebugDump();
	Engine::out() << "ImageSets:" << std::endl;
	ImageSets.DebugDump();
	Engine::out() << "Sounds:" << std::endl;
	Sounds.DebugDump();
	Engine::out() << "Tracks:" << std::endl;
	Tracks.DebugDump();
	Engine::out() << "# -------- # -------- #" << std::endl;
}

// #######################################
// ############# Creation #################
// #######################################


std::shared_ptr<Animation> ResourceManager::CreateAnimation(const std::string& from_name) const
{
	std::shared_ptr<ImageSet> a_data = ImageSets.GetItem(from_name);
	std::shared_ptr<Animation> a( 0 );

	if (a_data)
	{
		a.reset(new Animation(*a_data));
		return a;
	}

	Engine::out() << "ResourceManager::CreateAnimation(): Can't find ImageSet for Animation creation: '" << from_name << "'" << std::endl;
	//Engine::out() << "ResourceManager::CreateAnimation(): Can't find ImageSet for Animation creation: '" << from_name << "'" << std::endl;
	return a;
}

std::shared_ptr<sf::Sound> 	ResourceManager::CreateSound(const std::string& from_name) const
{
	std::shared_ptr<sf::SoundBuffer> sb = Sounds.GetItem(from_name);
	std::shared_ptr<sf::Sound> s;

	if (sb)
	{
		s.reset(new sf::Sound());
		s->setBuffer(*sb);

		return s;
	}

	Engine::out() << "ResourceManager::CreateSound(): Can't find SoundBuffer for sound creation: '" << from_name << std::endl;
	return s;
}

bool ResourceManager::AddImageSet ( const ImageSet& I )
{
	auto txt_ptr = Textures.GetItem(I.ImageName);

	if (txt_ptr)
	{
		ImageSets.AddItem(I.Name, I);
		return true;
	}
	else
	{
		Engine::out() << "ResourceManager::CreateImageSet(): Can't create ImageSet, Image '" << I.ImageName << "' not found!" << std::endl;
	}
	return false;
}

bool ResourceManager::CreateImageSet(const std::string& name, const std::string& from_image, const Geom::Vec2 StartPos, const Geom::Vec2 TileSize, const Geom::Vec2 TileCounts, const int fps)
{
	auto txt_ptr = Textures.GetItem(from_image);

	if (txt_ptr)
	{
		//Engine::out() << "Creating imageset " << name << " TileCount: " << TileCounts << std::endl;
		//Engine::out() << "Start: " << StartPos << std::endl;

		ImageSet* i = new ImageSet( name, from_image, StartPos, Geom::Origin(), TileSize, TileCounts, fps );
		ImageSets.AddItem(name, i);
		return true;
	}
	else
	{
		Engine::out() << "ResourceManager::CreateImageSet(): Can't create ImageSet, Image '" << from_image << "' not found!" << std::endl;
		return false;
	}

}

// #######################################
// ############# Loading #################
// #######################################


bool ResourceManager::LoadFont(const std::string& path, const std::string& name)
{
	std::shared_ptr<sf::Font> f(new sf::Font);

	if (f->loadFromFile ( path ) )
	{
		Fonts.AddItem(name, f);

		// no memory assumption done here, would be pure speculation
		// besides, not more then a few fonts will be used anyway

		return true;
	}
	else
	{
		Engine::out() << "Couldn't load Font: " << path << std::endl;
		return false;
	}
}

bool ResourceManager::LoadTexture(const std::string& path, const std::string& name)
{
	std::shared_ptr<sf::Texture> t( new sf::Texture);

	if (t->loadFromFile( path ) )
	{
		t->setSmooth(false);
		Textures.AddItem(name, t);

		// 4byte pixel size assumed (rgba)
		usedmemory += (t->getSize().x * t->getSize().y * 4);

		return true;
	}
	else
	{
		Engine::out() << "Couldn't load image: " << path << std::endl;
		return false;
	}
}



bool ResourceManager::LoadAudio( const std::string& path, const std::string& name )
{
	std::shared_ptr<sf::SoundBuffer> sb( new sf::SoundBuffer);

	if (sb->loadFromFile ( path ) )
	{
		Sounds.AddItem(name, sb);

		// assume 3 byte samples
		usedmemory += (long)( (float)sb->getChannelCount() * ( (float)sb->getSampleRate() ) * ( (sb->getDuration().asSeconds()) * 3));

		return true;
	}
	else
	{
		Engine::out() << "Couldn't load image: " << path << std::endl;
		return false;
	}
}

// ##########################################
// ############# Requesting #################
// ##########################################

std::shared_ptr<ImageSet> ResourceManager::GetImageSet(const std::string& name) const
{
	return ImageSets.GetItem(name);
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::GetAudio(const std::string& name) const
{
	return Sounds.GetItem(name);
}

std::shared_ptr<sf::Texture> ResourceManager::GetTexture(const std::string& name) const
{
	return Textures.GetItem(name);
}

std::shared_ptr<sf::Font> ResourceManager::GetFont(const std::string& name) const
{
	return Fonts.GetItem(name);
}

const sf::Font& ResourceManager::getDefaultFont(){
	static sf::Font font;
	static bool loaded = false;

	if(!loaded){
		static const signed char data[] = {
			#include "Arial.hpp"
		};
		font.loadFromMemory(data, sizeof(data));
		loaded = true;
	}
	return font;
}

