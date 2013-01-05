#include "sbe/ResourceManager.hpp"

// ############################################
// 				ADD / GET / REMOVE
// ############################################

template < typename T >
std::shared_ptr<T> ResourceManager::get( const std::string& name)
{
    auto ti = std::type_index(typeid(T));
    auto r = dynamic_pointer_cast<NamedList<T>> ( Resources[ti] );
    return r->GetItem(name);
}


template < typename T >
bool ResourceManager::add(T& res, const std::string& name)
{
    return add(std::shared_ptr<T>(res), name);
}

template < typename T >
bool ResourceManager::add( T* res, const std::string& name)
{
    return add(std::shared_ptr<T>(res), name);
}


template < typename T >
bool ResourceManager::add(std::shared_ptr<T> res, const std::string& name)
{
	auto ti = std::type_index(typeid(T));
	if ( ! isResource( ti ) ) return false;

    auto r = dynamic_pointer_cast<NamedList<T>> ( Resources[ std::type_index(ti) ] );
    r->AddItem(name, res);

    return true;
}


template< typename T >
bool ResourceManager::remove( std::string& name )
{
	auto ti = std::type_index(typeid(T));
	if ( !isResource( ti ) ) return false;

    auto r = dynamic_pointer_cast<NamedList<T>>( Resources[ std::type_index(ti) ] );
    r->erase(name);
    return true;
}



// ############################################
// 				SAVE / LOAD
// ############################################


template<class T>
bool ResourceManager::saveObject( const std::string& name, std::shared_ptr<T> pObj, const std::string& path )
{
	if ( ! isResource( typeid(T) ) ) return false;
    return IO::saveObject( name, *pObj, false);
}


template<class T>
bool ResourceManager::saveAllObjects()
{
	auto ti = std::type_index(typeid(T));
	if ( ! isResource( ti ) ) return false;

	std::shared_ptr<NamedList<T>> NL = dynamic_pointer_cast<NamedList<T>>( Resources[ std::type_index(ti) ] );

    IO::saveObjects( NL.GetMap().begin(), NL.GetMap().end());
}

// - Plugin and Class Management -

template < typename T>
void ResourceManager::registerResource( const iResource& iR, std::shared_ptr<IOPlugin> IOP)
{
	auto ti = std::type_index(typeid(T));

    if (Resources.find(ti) == Resources.end())
    {
        Resources[ti] = std::shared_ptr<BaseList> ( new NamedList<T>() );
        ResInfos[ti] = iR;

        Engine::GetIO()->addPlugin<T>( IOP );

        Engine::out() << "[ResourceManager] Registered class " << ti.name() << " as Resource." << std::endl;
    }
    else
    {
        Engine::out() << "[ResourceManager] Cant register class " << ti.name() << " as Resource, already registered!" << std::endl;
    }
}

template < typename T>
void ResourceManager::registerResource( const std::string& name)
{
    auto ti = std::type_index(typeid(T));

    if (Resources.find(ti) == Resources.end())
    {
        Resources[ti] = std::shared_ptr<BaseList> ( new NamedList<T>() );
        ResInfos[ti] = iResource::createResInfo( name, false, false);

        Engine::out() << "[ResourceManager] Registered class " << ti.name() << " as Resource." << std::endl;
    }
    else
    {
        Engine::out() << "[ResourceManager] Cant register class " << ti.name() << " as Resource, already registered!" << std::endl;
    }
}
