#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "sbe/entity/Component.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/optional.hpp>
#include <boost/any.hpp>

#include <memory>
#include <map>
#include <vector>

namespace sbe
{

	class System;

	/**
		An entity represents any ingame object which has any kind of data(=components) or behaviour(=systems) attached.
		Each entity consists of any number of components and systems.
		Components carry data while systems operate on this data.
	*/
	class Entity
	{
		public:

			friend class EntityManager;

			Entity();
			~Entity();

			template <class T>
			boost::optional<T&> getComponentData( const boost::uuids::uuid cID )
			{
				auto c = getComponent(cID);
				if ( !c ) return boost::optional<T&>();
				return boost::make_optional(boost::any_cast<T&>(*c));
			}

			/**
				Adds a new component to the Entity, this will overwrite any existing component with the same id
			*/
			void addComponent( const boost::uuids::uuid cID, boost::any& value );

			/**
				Removes a component by ID
				@return true if a component was removed
			*/
			bool removeComponent( const boost::uuids::uuid cID );


			/**
				get a component by ID
			*/
			boost::optional<boost::any> getComponent( const boost::uuids::uuid sID );

			/**
				get a System by ID
			*/
			std::shared_ptr<System> getSystem( const boost::uuids::uuid sID );

			/**
				Adds a new component to the Entity, this will overwrite any existing component with the same id
			*/
			void addSystem( std::shared_ptr<System> S);

			/**
				Removes a component by ID
				@return true if a component was removed
			*/
			bool removeSystem( const boost::uuids::uuid sID );

			/// returns this entities' id
			const boost::uuids::uuid getID() const { return ID; }

		private:
			boost::uuids::uuid ID;
			std::map<boost::uuids::uuid, boost::any> Components;
			std::map<boost::uuids::uuid, std::shared_ptr<System>> Systems;
	};

} // namespace sbe
#endif // ENTITY_HPP
