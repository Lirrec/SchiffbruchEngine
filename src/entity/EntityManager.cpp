#include "sbe/entity/EntityManager.hpp"

#include "sbe/entity/components/Position.hpp"

namespace sbe
{

	EntityManager::EntityManager()
	{
		registerComponents( sbe::components::PositionComponents );
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

	void EntityManager::registerComponents( std::vector<ComponentInfo>& Cs)
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

	void EntityManager::registerSystem(System& S)
	{
		S.generateID();
		SystemMappings[S.getName()] = S.getID();
		SystemFactories[S.getID()] = S.createFactory();
		//Systems[s.getID()] = S;
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

} // namespace sbe
