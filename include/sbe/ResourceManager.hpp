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


class ResourceManager
{

	public:
		ResourceManager();
		~ResourceManager();

        template < typename T >
        std::shared_ptr<T> get( const std::string& name)
        {
            auto ti = std::type_index(typeid(T));
            auto r = boost::any_cast<NamedList<T>> ( Resources[ti] );
            return r[name];
        }

				template < typename T >
        bool add(T& res, const std::string& name)
        {
            add(std::shared_ptr<T>(res), name);
        }

				template < typename T >
        bool add( T* res, const std::string& name)
        {
            add(std::shared_ptr<T>(res), name);
        }

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
            r[name] = res;

            return true;
        }


        template< typename T>
        bool remove(  std::type_info ti , std::string& name )
        {
            auto r = boost::any_cast<NamedList<T>> ( Resources[ std::type_index(ti) ] );
            r.erase(name);
        }


        // - Plugin and Class Management -
        template < typename T>
        void RegisterClass()
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
        boost::property_tree::ptree::node& getSettings() { return Settings; }


	private:

        void init();

		/// dumps all internal namedlists
		void DumpDebugInfo();

        // map from std::type_index to NamedList<T>
        typedef std::map<std::type_index, boost::any> ResourceMap;

        ResourceMap Resources;

        /// stores settings
        boost::property_tree::ptree Settings;
};

#endif // RESMGR_H

