#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

#include <SFML/System/Time.hpp>

#include "sbe/util/ClassFactory.hpp"

#include <string>
#include <vector>

namespace sbe
{
	class Entity;
	class Component;

	/**
		A System ( other common Words are behaviour /Controller ) operates on an Entities Components( Data/attributes ).
		Systems are attached automatically to Entities matching the required Components of the System.

		E.g. the RendererSystem may require a drawable Component

		The Components required for this  System to work on an Entity ( AND Condition, this means all listed Entities are required ).
		The Possibility to specify more than one list ( = OR Condition ) or optional Entities may be added later.

		To implement a System derive from SystemBuilder and define the static Name member of SystemBuilder<T> next to your class.
		@see SystemBuilder<T>

	*/
	class System
	{
		public:
			System();
			virtual ~System();




			/**
				Entry point for the logic of each System, called for each Entity
				@param delta the elapsed GameTime
			*/
			virtual void update(Entity& E, const sf::Time& delta) = 0;

			/**
				Called once after a system has been Attached to an Entity
				@param E the entity
			*/
			virtual void onAttach(Entity& E) = 0;

			/**
				Called once after a system has been Attached to an Entity
				@param E the entity
			*/
			virtual void onDetach(Entity& E) = 0;

			/// overridden in SystemBuilder<T>
			virtual const boost::uuids::uuid getID() const = 0;
			/// overridden in SystemBuilder<T>
			virtual const std::string getName() const = 0;
			/// overridden in SystemBuilder<T>
			virtual const std::vector<boost::uuids::uuid> getRequirements() const = 0;
			/// overridden in SystemBuilder<T>
			virtual const std::vector<std::string> getRequirementsPlain() const = 0;

		private:

			friend class EntityManager;

			/// overridden in SystemBuilder<T>
			virtual void generateID() const = 0;
			/// overridden in ComponentBuilder<T>
			virtual std::shared_ptr<Factory<System>> createFactory() const = 0;


	};

	/**
		Helper for implementing a System.

		Also remember that at some point in your Code you have to Call EntityManager::registerSystem( YourSystemObject ) for
		the Systems ID to be generated and registered in the EntityManager.
	*/
	template<class T>
	class SystemBuilder : public System
	{
		public:

			const boost::uuids::uuid getID() const override  { return ID; }

			void generateID()  const override  {
				boost::uuids::string_generator gen;
				ID = gen(Name);
				for ( auto& s : RequirementsPlain )
					Requirements.push_back( gen(s) );
			 }

			static const boost::uuids::uuid sID(){ return ID; }
			static const std::string sName(){ return Name; }

			static const std::vector<boost::uuids::uuid> sRequirements(){ return Requirements; }
			static const std::vector<std::string> sRequirementsPlain(){ return RequirementsPlain; }

			const std::string getName() const override { return Name; }
			const std::vector<boost::uuids::uuid> getRequirements() const override { return Requirements; }
			const std::vector<std::string> getRequirementsPlain() const override { return RequirementsPlain; }
			std::shared_ptr<Factory<System>> createFactory() const override { new FactoryWithBase<T, System>();  }

			static boost::uuids::uuid ID;
			static std::vector<boost::uuids::uuid> Requirements;

			static std::string Name;
			static std::vector<std::string> RequirementsPlain;
		private:




	};


//	template<class T>
//	std::string SystemBuilder<T>::Name;
//	template<class T>
//	std::vector<std::string> SystemBuilder<T>::RequirementsPlain;

	template<class T>
	boost::uuids::uuid SystemBuilder<T>::ID;
	template<class T>
	std::vector<boost::uuids::uuid> SystemBuilder<T>::Requirements;

} // namespace sbe
#endif // SYSTEM_HPP
