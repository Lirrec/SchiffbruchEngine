#ifndef SYSTEM_HPP
#define SYSTEM_HPP


#include <sbe/entity/EntityID.hpp>
#include <sbe/util/ClassFactory.hpp>

#include <boost/uuid/name_generator.hpp>

#include <SFML/System/Time.hpp>

#include <string>
#include <vector>

namespace sbe
{
	class Entity;

	class Component;

	namespace operators
	{
		/**
		 * A String literal for easy lookup of system ids.
		 * Allows to pass a string to any function taking a system id
		 */
		sbeID operator ""_sId (const char* str, size_t);
	}
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
		virtual void update(Entity& E, const sf::Time& delta) { };


		/**
			called for each Entity if any component has been accessed or once components are attached/detached
			@param delta the elapsed GameTime
		*/
		virtual void onEntityUpdate(Entity& E) { };

		/**
			Called once after a system has been Attached to an Entity
			@param E the entity
		*/
		virtual void onAttach(Entity& E) { };

		/**
			Called once after a system has been Attached to an Entity
			@param E the entity
		*/
		virtual void onDetach(Entity& E) { };

		/// overridden in SystemBuilder<T>
		virtual const sbeID getID() const = 0;

		/// overridden in SystemBuilder<T>
		virtual const std::string getName() const = 0;

		/// overridden in SystemBuilder<T>
		virtual const std::vector<sbeID> getRequirements() const = 0;

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

		const sbeID getID() const override { return ID; }

		void generateID() const override {
			boost::uuids::name_generator gen(sbeID_namespace);
			ID = gen(Name);
			for (auto& s : RequirementsPlain)
				Requirements.push_back(gen(s));
		}

		static const sbeID sID() { return ID; }

		static const std::string sName() { return Name; }

		static const std::vector<sbeID> sRequirements() { return Requirements; }

		static const std::vector<std::string> sRequirementsPlain() { return RequirementsPlain; }

		const std::string getName() const override { return Name; }

		const std::vector<sbeID> getRequirements() const override { return Requirements; }

		const std::vector<std::string> getRequirementsPlain() const override { return RequirementsPlain; }

		std::shared_ptr<Factory<System>> createFactory() const override { return std::dynamic_pointer_cast<Factory<System>>(std::make_shared<FactoryWithBase<T, System>>()); }

		static sbeID ID;
		static std::vector<sbeID> Requirements;

		static std::string Name;
		static std::vector<std::string> RequirementsPlain;
	private:


	};


//	template<class T>
//	std::string SystemBuilder<T>::Name;
//	template<class T>
//	std::vector<std::string> SystemBuilder<T>::RequirementsPlain;

	template<class T>
	sbeID SystemBuilder<T>::ID;
	template<class T>
	std::vector<sbeID> SystemBuilder<T>::Requirements;

} // namespace sbe
#endif // SYSTEM_HPP
