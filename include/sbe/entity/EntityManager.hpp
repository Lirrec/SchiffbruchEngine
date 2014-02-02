#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include "sbe/entity/Component.hpp"
#include "sbe/entity/Entity.hpp"
#include "sbe/entity/System.hpp"

#include "sbe/util/ClassFactory.hpp"

#include <string>
#include <unordered_map>
#include <list>
#include <memory>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

namespace sbe
{

	/**
		Holds a list of all Entities. Used to get all Entities containing a specific System/set of components.
	*/
	class EntityManager
	{
		public:
			EntityManager();
			~EntityManager();

			/**
				Update all Entities.
				This calls all Entities' Systems' update() method.
			*/
			void update(const sf::Time& delta);

			void onEntityChanged( const boost::uuids::uuid eID );

			bool addEntity( Entity* E );
			bool removeEntity( const boost::uuids::uuid eID );

			template <class T>
			void registerComponent( const std::string& Name)
			{
				registerComponent( Name, std::make_shared( new FactoryWithBase<T, boost::any>() ) );
			}

			void registerComponent( const ComponentInfo& C );

			void registerComponents( const std::vector<ComponentInfo>& Cs);

			template<class T>
			void registerSystem()
			{
				registerSystem(std::make_shared<T>());
			}

			void registerSystem( std::shared_ptr<System> S );

			const boost::uuids::uuid lookupSystemID( const std::string& name ) const;
			const boost::uuids::uuid lookupComponentID( const std::string& name ) const;
			const std::string lookupSystemName( boost::uuids::uuid sID ) const;
			const std::string lookupComponentName( boost::uuids::uuid cID ) const;

			std::shared_ptr<System> createSystem( boost::uuids::uuid sID );
			boost::optional<boost::any> createComponent( boost::uuids::uuid cID );

		private:

			std::map<std::string, boost::uuids::uuid> ComponentMappings;
			std::map<boost::uuids::uuid, std::string> ComponentNames;
			std::map<boost::uuids::uuid, std::shared_ptr< CopyFactory<boost::any> >> ComponentFactories;

			std::map<boost::uuids::uuid, std::shared_ptr<Entity> > Entities;

			std::map<boost::uuids::uuid, std::shared_ptr<System>> Systems;
			std::map<std::string, boost::uuids::uuid> SystemMappings;
			std::map<boost::uuids::uuid, std::shared_ptr< Factory<System> >> SystemFactories;

	};

} // namespace sbe

#endif // ENTITYMANAGER_HPP
