#ifndef RESMGR_H
#define RESMGR_H

#include <string>
#include <memory>

#include <typeinfo>
#include <typeindex>



// fixes some ptrdiff_t not defined errors
#include <cstddef>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <boost/any.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Animation.hpp"

#include "util/NamedList.hpp"

/**
	Holds information about a resource
*/
class iResource
{
	std::string name; /// string name of the resource for debugging purposes
	bool savetosave; /// should this resource be included in savegames?
	bool preload;	  /// should all available resources of this type be autoloaded from a savegame?
}

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
        template < typename T >
        std::shared_ptr<T> get( const std::string& name)
        {
            auto ti = std::type_index(typeid(T));
            auto r = boost::any_cast<NamedList<T>> ( Resources[ti] );
            return r.GetItem(name);
        }

		/**
			@see add(std::shared_ptr<T> res, const std::string& name)
		*/
		template < typename T >
        bool add(T& res, const std::string& name)
        {
            add(std::shared_ptr<T>(res), name);
        }

		/**
			@see add(std::shared_ptr<T> res, const std::string& name)
		*/
		template < typename T >
        bool add( T* res, const std::string& name)
        {
            add(std::shared_ptr<T>(res), name);
        }

		/**
			Adds a new Object to the Resourcemanager
			@param res a pointer to the new object
			@param name the name by which the object will be accessed later
			@return true on success, false on error, invalid pointer or if the type of the given object is not registered as a resource
		*/
		template < typename T >
        bool add(std::shared_ptr<T> res, const std::string& name)
        {
            auto ti = std::type_index(typeid(T));

            if ( Resources.find(ti) == Resources.end() )
            {
                Engine::out() << "[ResourceManager] Class " << ti.name() << " not registered as Resource!" << std::endl;
                return false;
            }

            auto r = boost::any_cast<NamedList<T>> ( Resources[ti] );
            r.AddItem(name, res);

            return true;
        }


		/**
			Removes a single object from the Resourcemanager
			@param ti the type of the object
			@param name the name of the object
			@return true on succes, false on error or if the objects was not found
		*/
        template< typename T>
        bool remove(  std::type_info ti , std::string& name )
        {
            auto r = boost::any_cast<NamedList<T>> ( Resources[ std::type_index(ti) ] );
            r.erase(name);
        }


		/**
			Save a single object.
			The save-location is defined by the top of the path stack in the IO Plugin
			@return false on errors
		*/
        template<class T>
        bool saveObject( std::shared_ptr<T> pObj, const std::string& path );

		/**
			Save all Objects of a resource.
			The save location is defined by the top of the path stack in the IO Plugin
			@return false on errors
		*/
        template<class T>
        bool saveAllObjects();

        // - Plugin and Class Management -
        /**
        Register a new Resource with a BinaryIOPlugin
        @param iR the Resource information
        @param IOPlugin the plugin responsible for loading/saving this resource
        */
        template < typename T>
        void registerResource( iResource& iR, iBinaryIOPlugin& IOPlugin)
        {
            auto ti = std::type_index(typeid(T));

            if (Resources.find(ti) == Resources.end())
            {
                Resources[ti] = boost::any( NamedList<T>() );
                Engine::out() << "[ResourceManager] Registered class " << ti.name() << " as Resource." << std::endl;
            }
            else
            {
                Engine::out() << "[ResourceManager] Cant register class " << ti.name() << " as Resource, already registered!" << std::endl;
            }

        }

        /**
        Register a new Resource with a TreeIOPlugin
        @param iR the Resource information
        @param IOPlugin the plugin responsible for loading/saving this resource
        */
        void registerResource( iResource& iR, iTreeIOPlugin& IOPlugin)
        {
            auto ti = std::type_index(typeid(T));

            if (Resources.find(ti) == Resources.end())
            {
                Resources[ti] = boost::any( NamedList<T>() );
                Engine::out() << "[ResourceManager] Registered class " << ti.name() << " as Resource." << std::endl;
            }
            else
            {
                Engine::out() << "[ResourceManager] Cant register class " << ti.name() << " as Resource, already registered!" << std::endl;
            }

        }

        // - Settings/Registry -
        // ebenfalls über Events erreichbar
        boost::property_tree::ptree& getSettings() { return Settings; }
				const sf::Font& getDefaultFont();


		// - SAVEGAME MANAGEMENT -

		/// creates a new savegame by adding the given path to IO's path-stack and
		/// then saving all Resources marked with "save" to that savegame
		/// Also saves the "user." part of the Settings
		createSave(name);

		/// opens a savegame by adding its path to IO's path-stack and then loading
		/// all resources with marked with "preload"
		/// also loads the "user." part of the settings and OVERWRITES any existing keys
		loadSave(name);

		/// Removes the current Savegame from IO's path-stack
		popSave();

	private:

		/// stores the name of the current save if saving/loading is in progres.
		/// used to pop the right value from the path-stack in IO once the save/load has been marked as completed
		/// with PopSave()
		std::string currentsave;

        void init();

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
		void dumpDebugInfo();

        // map from std::type_index to NamedList<T>
        typedef std::map<std::type_index, boost::any> ResourceMap;
        typedef std::map<std::type_index, iResource> ResourceInfos;


        ResourceMap Resources;

        /// stores settings
        boost::property_tree::ptree Settings;
};

#endif // RESMGR_H

