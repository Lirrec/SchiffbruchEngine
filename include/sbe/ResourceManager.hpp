#ifndef RESMGR_H
#define RESMGR_H

#include <string>
#include <memory>
#include <vector>

#include <typeinfo>
#include <typeindex>

#include <boost/any.hpp>
#include <boost/property_tree/ptree.hpp>

#include <sbe/util/NamedList.hpp>
#include <sbe/io/IOPlugin.hpp>
#include <sbe/io/IO.hpp>

namespace sf
{
	class Font;
}

namespace sbe
{
	class BaseList;

	class IOPlugin;

	class IO;

	/**
		Holds information about a resource.
	*/
	class iResource
	{
	public:

		static iResource createResInfo(const std::string& name, bool savetosave, bool preload) {
			iResource re;
			re.name = name;
			re.savetosave = savetosave;
			re.preload = preload;
			return re;
		}

		std::string name;    /// string name of the resource for debugging purposes
		bool savetosave;    /// should this resource be included in savegames?
		bool preload;        /// should all available resources of this type be autoloaded from a savegame?
	};

	/**
		The ResourceManager stores Resources to avoid copying and allow access by name.
		A Resource is every kind of object that is required by the Game and is usually unique.
		This applies to sounds, textures, models, levels and similiar.

		This class does not load or save those Resources itself.
		@see IO, IOPLugin
		All available Resources should be registered with the ResourceManager and IO before use.
		Example how the engines own plugins are loaded:
		@code
			std::shared_ptr<IOPlugin> SoundIOP ( new SoundIOPlugin() );
			registerResource<sf::SoundBuffer>( iResource::createResInfo("SoundBuffer", false, false), SoundIOP);

			std::shared_ptr<IOPlugin> ImgIOP ( new ImageIOPlugin() );
			registerResource<sf::Image>( iResource::createResInfo("Image", false, false), ImgIOP);

			std::shared_ptr<IOPlugin> ShaderIOP ( new ShaderIOPlugin() );
			registerResource<sf::Shader>( iResource::createResInfo("SHADER", false, false), ShaderIOP);

			std::shared_ptr<IOPlugin> ImageSetIOP ( new ImageSetIOPlugin() );
			registerResource<ImageSet>( iResource::createResInfo("ImageSet", false, false), ImageSetIOP);

			registerResource<sf::Font>( "Font" );
		@endcode


		It provides a simple savegame feature to save/load all resources to a user defined path.

	*/
	class ResourceManager
	{

	public:
		ResourceManager();

		~ResourceManager();

		/**
			Access a stored object
			@param name the name of the object to return
			@return a shared_ptr to the object or null if the object or type was not found
		*/
		template<typename T>
		std::shared_ptr<T> get(const std::string& name);

/**
			Access multiple stored objects with a given string in their name
			@param name the string to search for
			@return a vector of pointers to the found objects, may be empty
		*/
		template<typename T>
		std::vector<std::shared_ptr<T>> getMatching(const std::string& name);

		/**
			@see add(std::shared_ptr<T> res, const std::string& name)
		*/
		template<typename T>
		bool add(T& res, const std::string& name);

		/**
			@see add(std::shared_ptr<T> res, const std::string& name)
		*/
		template<typename T>
		bool add(T* res, const std::string& name);

		/**
			Adds a new Object to the Resourcemanager
			@param res a pointer to the new object
			@param name the name by which the object will be accessed later
			@return true on success, false on error, invalid pointer or if the type of the given object is not registered as a resource
		*/
		template<typename T>
		bool add(std::shared_ptr<T> res, const std::string& name);


		/**
			Removes a single object from the Resourcemanager
			@param name the name of the object
			@return true on succes, false on error or if the objects was not found
		*/
		template<typename T>
		bool remove(std::string& name);

		/**
			Save a single object.
			The save-location is defined by the top of the path stack in the IO Plugin
			@return false on errors
		*/
		template<class T>
		bool saveObject(const std::string& name, std::shared_ptr<T> pObj, bool overwrite = false);

		/**
			Save all Objects of a resource.
			The save location is defined by the top of the path stack in the IO Plugin
			@return false on errors
		*/
		template<class T>
		bool saveAllObjects(bool overwrite = false);

		//~ template<class T>
		//~ std::shared_ptr<T> loadObject(const std::string name,  lObj);
		template<class T>
		bool loadAllObjects();


		// - Plugin and Class Management -



		/**
			Register a new Resource with an IOPlugin
			@param iR the Resource information
			@param IOP the plugin responsible for loading/saving this resource
		*/
		template<typename T>
		void registerResource(const iResource& iR, std::shared_ptr<IOPlugin> IOP);

		/**
			Register a new Resource without an IOPlugin. The Engine won't be able to save/load the given resource
			@param name the Name of the Resource
		*/
		template<typename T>
		void registerResource(const std::string& name);


		const sf::Font& getDefaultFont();


		// - SAVEGAME MANAGEMENT -

		/// creates a new savegame by adding the given path to IO's path-stack and
		/// then saving all Resources marked with "save" to that savegame
		/// Also saves the "user." part of the Settings
		void createSave(const std::string& name);

		/// opens a savegame by adding its path to IO's path-stack and then loading
		/// all resources with marked with "preload"
		/// also loads the "user." part of the settings and OVERWRITES any existing keys
		void loadSave(const std::string& name);

		/// Removes the current Savegame from IO's path-stack
		void popSave();

		/// initialises all default Resources and loads the IO System
		void init();

	private:

		/// stores the name of the current save if saving/loading is in progres.
		/// used to pop the right value from the path-stack in IO once the save/load has been marked as completed
		/// with PopSave()
		std::string currentsave;

		/// returns true if the class is a valid resource, outputs an error otherwise
		bool isResource(const std::type_index& ti);

		/**
			Register IOPlugins/iResources for all the Classes supported by default by the engine.
			This includes:
				sf::Texture
				sf::Soundbuffer
				sf::Music
				sf::Font
				ImageSet

		*/
		void loadDefaultPlugins();

		/// dumps all internal namedlists
		void DumpDebugInfo();

		typedef std::map<std::type_index, std::shared_ptr<BaseList> > ResourceMap;
		typedef std::map<std::type_index, iResource> ResourceInfos;


		ResourceMap Resources;
		ResourceInfos ResInfos;

		std::shared_ptr<sbe::IO> mIO;
	};

} // namespace sbe

// Include Implementation
#include "ResourceManager.ipp"

#endif // RESMGR_H
