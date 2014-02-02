#include "sbe/entity/Entity.hpp"

#include "sbe/entity/EntityManager.hpp"
#include "sbe/Engine.hpp"

namespace sbe
{

	Entity::Entity()
	{
		Engine::GetEntityMgr()->addEntity( this );
	}

	Entity::~Entity()
	{
		Engine::GetEntityMgr()->removeEntity( ID );
	}





	std::shared_ptr<System> Entity::getSystem(const boost::uuids::uuid sID)
	{
		auto it = Systems.find(sID);
		if ( it != Systems.end() ) return it->second;
		return std::shared_ptr<System>();
	}

	void Entity::addSystem(std::shared_ptr<System> S)
	{
		Systems[S->getID()] = S;
	}

	bool Entity::removeSystem(const boost::uuids::uuid sID)
	{
		return Systems.erase(sID);
	}



	void Entity::addComponent(const std::string& name)
	{
		addComponent(lookupComponentID(name)) ;
	}

	void Entity::addComponent(const std::string& name, boost::any& value)
	{
		addComponent( lookupComponentID(name), value );
	}

	void Entity::addComponent( const boost::uuids::uuid cID, boost::any& value)
	{
		Engine::GetEntityMgr()->onEntityChanged( ID );
		Components[cID] = value;
	}

	void Entity::addComponent(const boost::uuids::uuid cID)
	{
		Engine::GetEntityMgr()->onEntityChanged( ID );
		Components[cID] = Engine::GetEntityMgr()->createComponent( cID );
	}





	bool Entity::removeComponent(const boost::uuids::uuid cID)
	{
		bool re =  Components.erase(cID);
		if ( re ) Engine::GetEntityMgr()->onEntityChanged( ID );
		return re;
	}

	boost::optional<boost::any> Entity::getComponent(const boost::uuids::uuid cID)
	{
		auto it = Components.find(cID);
		if ( it != Components.end() ) return boost::optional<boost::any>(it->second);
		return boost::optional<boost::any>();
	}



	bool Entity::removeComponent(const std::string& name)
	{

	}

	boost::optional<boost::any> Entity::getComponent(const std::string& name)
	{

	}

	std::shared_ptr<System> Entity::getSystem(const std::string& name)
	{

	}

	void Entity::addSystem(const boost::uuids::uuid sID)
	{

	}

	void Entity::addSystem(const std::string& name)
	{

	}

	bool Entity::removeSystem(const std::string& name)
	{

	}

	boost::uuids::uuid Entity::lookupComponentID(const std::string& name) const
	{

	}

	boost::uuids::uuid Entity::lookupSystemID(const std::string& name) const
	{

	}


} // namespace sbe
