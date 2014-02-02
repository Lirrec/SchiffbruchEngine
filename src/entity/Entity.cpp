#include "sbe/entity/Entity.hpp"

#include "sbe/entity/EntityManager.hpp"
#include "sbe/Engine.hpp"

#include <boost/uuid/random_generator.hpp>

namespace sbe
{

	Entity::Entity()
	{
		boost::uuids::random_generator gen;
		ID = gen();
	}

	Entity::~Entity()
	{
		Engine::GetEntityMgr()->removeEntity( ID );
	}

	bool Entity::Register()
	{
		return Engine::GetEntityMgr()->addEntity( shared_from_this() );
	}

	void Entity::addComponent(const std::string& name)
	{
		addComponent(lookupComponentID(name));
	}

	void Entity::addComponent(const std::string& name, boost::any& value)
	{
		addComponent( lookupComponentID(name), value );
	}

	void Entity::addComponent( const sbeID cID, boost::any& value)
	{
		Engine::GetEntityMgr()->onEntityChanged( ID );
		Components[cID] = value;
	}

	void Entity::addComponent(const sbeID cID)
	{
		Components[cID] = Engine::GetEntityMgr()->createComponent( cID );
		Engine::GetEntityMgr()->onEntityChanged( ID );
	}





	bool Entity::removeComponent(const sbeID cID)
	{
		bool re =  Components.erase(cID);
		if ( re ) Engine::GetEntityMgr()->onEntityChanged( ID );
		return re;
	}

	boost::optional<boost::any> Entity::getComponent(const sbeID cID)
	{
		auto it = Components.find(cID);
		if ( it != Components.end() ) return boost::optional<boost::any>(it->second);
		return boost::optional<boost::any>();
	}

	boost::optional<boost::any> Entity::getComponent(const std::string& name)
	{
		return getComponent( lookupComponentID( name ));
	}

	bool Entity::removeComponent(const std::string& name)
	{
		return removeComponent( lookupComponentID(name));
	}

	std::shared_ptr<System> Entity::getSystem(const std::string& name)
	{
		return getSystem(lookupSystemID(name));
	}

	void Entity::addSystem(const sbeID sID)
	{
		auto S = Engine::GetEntityMgr()->createSystem(sID);
		if (S) Systems[sID] = S;
	}

	void Entity::addSystem(const std::string& name)
	{
		addSystem(lookupSystemID(name));
	}

	bool Entity::removeSystem(const std::string& name)
	{
		return removeSystem(lookupSystemID(name));
	}


	std::shared_ptr<System> Entity::getSystem(const sbeID sID)
	{
		auto it = Systems.find(sID);
		if ( it != Systems.end() ) return it->second;
		return std::shared_ptr<System>();
	}

	void Entity::addSystem(std::shared_ptr<System> S)
	{
		Systems[S->getID()] = S;
	}

	bool Entity::removeSystem(const sbeID sID)
	{
		return Systems.erase(sID);
	}


	sbeID Entity::lookupComponentID(const std::string& name) const
	{
		return Engine::GetEntityMgr()->lookupComponentID(name);
	}

	sbeID Entity::lookupSystemID(const std::string& name) const
	{
		return Engine::GetEntityMgr()->lookupSystemID(name);
	}


} // namespace sbe
