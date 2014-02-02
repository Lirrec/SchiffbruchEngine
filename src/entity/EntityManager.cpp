#include "sbe/entity/EntityManager.hpp"

#include <boost/uuid/nil_generator.hpp>

#include "sbe/entity/components/Position.hpp"
#include "sbe/entity/components/SFMLDrawables.hpp"

#include "sbe/entity/systems/SpriteRenderer.hpp"

namespace sbe
{

	EntityManager::EntityManager()
	{
		registerComponents( sbe::components::getPositionComponents() );
		registerComponents( sbe::components::getSFMLComponents() );
		registerSystem<systems::SpriteRenderer>();
	}

	EntityManager::~EntityManager()
	{

	}

	void EntityManager::update(const sf::Time& delta)
	{
		for ( auto& E : Entities )
		{
			for( auto& p : E.second->Systems ) p.second->update(*(E.second), delta);
		}
	}

	void EntityManager::registerComponents( const std::vector<ComponentInfo>& Cs)
	{
		for ( auto C: Cs )
			registerComponent( C );
	}

	void EntityManager::registerComponent( const ComponentInfo& C )
	{
		boost::uuids::string_generator gen;
		auto ID = gen(C.first);
		ComponentMappings[C.first] = ID;
		ComponentNames[ID] = C.first;
		ComponentFactories[ID] = C.second;
	}

	void EntityManager::registerSystem(std::shared_ptr<System> S)
	{
		S->generateID();
		SystemMappings[S->getName()] = S->getID();
		SystemFactories[S->getID()] = S->createFactory();
		Systems[S->getID()] = S;
	}


	void EntityManager::onEntityChanged(boost::uuids::uuid eID)
	{

	}

	bool EntityManager::addEntity(Entity* E)
	{
		Entities[E->getID()] = std::shared_ptr<Entity>(E);
	}

	bool EntityManager::removeEntity(const boost::uuids::uuid eID)
	{
		return Entities.erase(eID);
	}

	const boost::uuids::uuid EntityManager::lookupSystemID( const std::string& name ) const
	{
		if ( !SystemMappings.count(name) ) return boost::uuids::nil_uuid();
		return SystemMappings.at(name);
	}

	const boost::uuids::uuid EntityManager::lookupComponentID( const std::string& name ) const
	{
		if ( !ComponentMappings.count(name) ) return boost::uuids::nil_uuid();
		return ComponentMappings.at(name);
	}

	const std::string EntityManager::lookupSystemName( boost::uuids::uuid sID ) const
	{
		if ( !Systems.count(sID) ) return "system_not_found";
		return Systems.at(sID)->getName();
	}

	const std::string EntityManager::lookupComponentName( boost::uuids::uuid cID ) const
	{
		if ( !ComponentNames.count(cID) ) return "component_not_found";
		return ComponentNames.at(cID);
	}

	std::shared_ptr<System> EntityManager::createSystem(boost::uuids::uuid sID)
	{
		if ( !SystemFactories.count(sID) ) return std::shared_ptr<System>();
		return SystemFactories[sID]->create();
	}

	boost::optional<boost::any> EntityManager::createComponent(boost::uuids::uuid cID)
	{
		if ( !ComponentFactories.count(cID) ) return boost::optional<boost::any>();
		return boost::make_optional(ComponentFactories[cID]->createCopy());
	}

} // namespace sbe
