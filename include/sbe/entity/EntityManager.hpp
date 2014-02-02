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

#include "sbe/entity/EntityID.hpp"

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

			void onEntityChanged( const sbeID eID );

			bool addEntity( std::shared_ptr<Entity> E );
			bool removeEntity( const sbeID eID );

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

			const sbeID lookupSystemID( const std::string& name ) const;
			const sbeID lookupComponentID( const std::string& name ) const;
			const std::string lookupSystemName( sbeID sID ) const;
			const std::string lookupComponentName( sbeID cID ) const;

			std::shared_ptr<System> createSystem( sbeID sID );
			boost::optional<boost::any> createComponent( sbeID cID );

		private:

			std::map<std::string, sbeID> ComponentMappings;
			std::map<sbeID, std::string> ComponentNames;
			std::map<sbeID, std::shared_ptr< CopyFactory<boost::any> >> ComponentFactories;

			std::map<sbeID, std::shared_ptr<Entity> > Entities;

			std::map<sbeID, std::shared_ptr<System>> Systems;
			std::map<std::string, sbeID> SystemMappings;
			std::map<sbeID, std::shared_ptr< Factory<System> >> SystemFactories;

	};

} // namespace sbe

#endif // ENTITYMANAGER_HPP
