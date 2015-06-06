#include "sbe/entity/Entity.hpp"

#include "sbe/entity/EntityManager.hpp"

#include <boost/uuid/random_generator.hpp>

namespace sbe
{

	Entity::Entity() {
		boost::uuids::random_generator gen;
		ID = gen();
	}

	Entity::~Entity() {
		for (auto S : Systems)
			S.second->onDetach(*this);
	}

	bool Entity::Register() {
		return Engine::GetEntityMgr()->addEntity(shared_from_this());
	}

	void Entity::addComponent(const sbeID cID, boost::any& value) {
		Engine::GetEntityMgr()->onEntityChanged(ID);
		Components[cID] = value;
		changed = true;
	}

	void Entity::addComponent(const sbeID cID) {
		auto c = Engine::GetEntityMgr()->createComponent(cID);
		if (!c) return;
		Components[cID] = *c;
		changed = true;
		Engine::GetEntityMgr()->onEntityChanged(ID);
	}


	bool Entity::setComponentData(const sbeID cID, const boost::any& data) {
		auto c = getComponent(cID);
		if (!c) return false;
		Components[cID] = data;
		changed = true;
		return true;
	}


	bool Entity::removeComponent(const sbeID cID) {
		bool re = Components.erase(cID);
		if (re)
		{
			Engine::GetEntityMgr()->onEntityChanged(ID);
			changed = true;
		}

		return re;
	}

	boost::optional<boost::any&> Entity::getComponent(const sbeID cID) {
		auto it = Components.find(cID);
		if (it != Components.end()) return boost::optional<boost::any&>(it->second);
		changed = true;
		return boost::optional<boost::any&>();
	}

	void Entity::addSystem(const sbeID sID) {
		auto S = Engine::GetEntityMgr()->createSystem(sID);
		if (S)
		{
			Systems[sID] = S;
			S->onAttach(*this);
			changed = true;
		}
	}

	std::shared_ptr<System> Entity::getSystem(const sbeID sID) {
		auto it = Systems.find(sID);
		if (it != Systems.end()) return it->second;
		return std::shared_ptr<System>();
	}

	void Entity::addSystem(std::shared_ptr<System> S) {
		Systems[S->getID()] = S;
		S->onAttach(*this);
		changed = true;
	}

	bool Entity::removeSystem(const sbeID sID) {
		auto S = getSystem(sID);
		if (S) S->onDetach(*this);
		changed = true;
		return Systems.erase(sID);
	}

} // namespace sbe
