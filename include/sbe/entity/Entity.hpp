#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "sbe/entity/Component.hpp"

#include "sbe/entity/EntityID.hpp"
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
	class Entity : public std::enable_shared_from_this<Entity>
	{
		public:

			friend class EntityManager;

			Entity();
			~Entity();

			/// register this entity with the entitymanager, enables systems to be run
			bool Register();


			template <class T>
			boost::optional<T> getComponentData( const std::string& name )
			{
				return getComponentData<T>( lookupComponentID(name) );
			}

			template <class T>
			boost::optional<T> getComponentData( const sbeID cID )
			{
				auto c = getComponent(cID);
				if ( !c ) return boost::optional<T>();
				return boost::make_optional<T>(boost::any_cast<T>(*c));
			}

			boost::any& operator[](const sbeID cID)
			{
				return Components[cID];
			}

			boost::any& operator[](const std::string& name)
			{
				return Components[lookupComponentID(name)];
			}

			template<class T>
			T& C(const std::string& name)
			{
				return boost::any_cast<T&>(Components[lookupComponentID(name)]);
			}

			template<class T>
			T& C(const sbeID cID)
			{
				return boost::any_cast<T&>(Components[cID]);
			}

			bool setComponentData( const sbeID cID, const boost::any& data );

			/** Adds a new component to the Entity, this will overwrite any existing component with the same id.
				@param cID the ID of the new Component. */
			void addComponent( const sbeID cID );
			/** Adds a new component to the Entity, this will overwrite any existing component with the same id.
				@param cID the ID of the new Component.
				@param value the value of the new component */
			void addComponent( const sbeID cID, boost::any& value );

			/** Adds a new component to the Entity, this will overwrite any existing component with the same id.
				@param name the string-identifier of the new Component. */
			void addComponent( const std::string& name);
			/** Adds a new component to the Entity, this will overwrite any existing component with the same id.
				@param name the string-identifier of the new Component.
				@param value the value of the new component */
			void addComponent( const std::string& name, boost::any& value );


			/**
				Removes a component by ID
				@return true if a component was removed
			*/
			bool removeComponent( const sbeID cID );
			/**
				Removes a component by Name
				@return true if a component was removed
			*/
			bool removeComponent( const std::string& name );


			/**
				get a component by ID
			*/
			boost::optional<boost::any&> getComponent( const sbeID sID );
			/**
				get a component by Name
			*/
			boost::optional<boost::any&> getComponent( const std::string& name );

			/**
				get a System by ID
				@return the system or an invalid  shared_ptr
			*/
			std::shared_ptr<System> getSystem( const sbeID sID );
			/**
				get a System by Name
				@return the system or an invalid  shared_ptr
			*/
			std::shared_ptr<System> getSystem( const std::string& name );

			/**
				Adds a new system to the Entity, this will overwrite any existing system with the same id
			*/
			void addSystem( std::shared_ptr<System> S);

			/**
				Adds a new system to the Entity, this will overwrite any existing system with the same id
			*/
			void addSystem( const sbeID sID);

			/**
				Adds a new System to the Entity, this will overwrite any existing system with the same id
			*/
			void addSystem(  const std::string& name);

			/**
				Removes a system by ID
				@return true if a system was removed
			*/
			bool removeSystem( const sbeID sID );

			/**
				Removes a system by Name
				@return true if a system was removed
			*/
			bool removeSystem( const std::string& name );

			/// returns this entities' id
			const sbeID getID() const { return ID; }

		private:

			sbeID lookupComponentID( const std::string& name ) const;
			sbeID lookupSystemID( const std::string& name ) const;

			sbeID ID;
			std::map<sbeID, boost::any> Components;
			std::map<sbeID, std::shared_ptr<System>> Systems;
	};

} // namespace sbe
#endif // ENTITY_HPP
