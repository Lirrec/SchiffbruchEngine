#include "sbe/gfx/Renderer.hpp"
#include "sbe/gfx/Camera.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <sbe/geom/RectHelpers.hpp>
#include <sbe/geom/SfmlHelpers.hpp>

namespace sbe
{
	Renderer::Renderer() {
		//RegisterForEvent("ADD_ACTOR");


		RegisterForEvent<std::shared_ptr<Actor>>("UPDATE_ACTOR", [this](std::shared_ptr<Actor> actor){
			updateActor(actor->getID(), actor);
		});

		RegisterForEvent<ActorID>("REMOVE_ACTOR", [this](ActorID ID){
			removeActor(ID);
		});

		using namespace operators;
		RegisterMemberAsEventCallback(this, addActorEvent(), "ADD_ACTOR");
		RegisterMemberAsEventCallback(this, addActorsEvent(), "ADD_ACTORS");
	}

	void Renderer::render(sf::RenderTarget& t) {
		int i = 0;
		for (RenderLayer& L : Layers)
		{
			//Engine::out() << "Updating Layer " << ++i << std::endl;
			if (L.isActive) updateLayer(L);

			//Engine::out() << "Culling Layer " << i << std::endl;
			//if (L.cull && L.changed) cullLayer(L);
			if (L.cull) cullLayer(L, i);

			//Engine::out() << "Drawing Layer " << i << std::endl;
			if (L.isActive) drawLayer(L, t);

			i++;
		}
		//Engine::out() << "Done drawing" << std::endl;
	}


	void Renderer::renderSingleLayer(unsigned int index, sf::RenderTarget& t) {
		if (index < Layers.size())
		{
			drawLayer(Layers[index], t);
		}
	}

	void Renderer::updateLayer(RenderLayer& L) {
		//int i = 0;
		for (std::shared_ptr<Actor>& A : L.RenderList)
		{
			//Engine::out() << "Updating Actor " << ++i << std::endl;
			if (A->enabled) A->update(RenderTime.getElapsedTime());
		}
	}

	void Renderer::cullLayer(RenderLayer& L, const unsigned int index) {

		// do culling, not yet implemented
		auto& CameraBounds = L.Cam->getDrawnArea();
		int count = 0;
		//Engine::out() << "CameraBounds: " << geom::RectToString(CameraBounds) << std::endl;
		for (std::shared_ptr<Actor>& actor : L.RenderList) {
			auto ActorBounds = actor->getBounds();
			actor->enabled = geom::rcOverlap( CameraBounds, ActorBounds);

			if (actor->enabled) {
				count++;
			}
			//Engine::out() << "ActorBounds: " << geom::RectToString(ActorBounds)  << "CameraBounds: " << geom::RectToString(CameraBounds) << " -> " << actor->enabled << std::endl;
		}


		Module::Get()->DebugString("Actors rendered [L" + std::to_string(index) + "]", std::to_string(count));

		L.changed = false;
	}

	void Renderer::drawLayer(const RenderLayer& L, sf::RenderTarget& t) {
		if (L.Cam) {
			t.setView(L.Cam->getView());
		} else {
			t.setView(t.getDefaultView());
		}

		for (const std::shared_ptr<Actor>& A : L.RenderList)
		{
			//Engine::out() << "Drawing actor: " << A->getID() << " - drawable: " << &(A->getDrawable()) << std::endl;

			if (A->enabled)
				t.draw(A->getDrawable(), L.States);


		}
	}

	void Renderer::addLayer(const RenderLayer& L, const int index) {
		if (index == -1) Layers.push_back(L);
		else if ((unsigned int) index < Layers.size())
		{
			auto it = Layers.begin() + index;
			Layers.insert(it, L);
		}
	}

	void Renderer::removeLayer(unsigned int index) {
		if (index < Layers.size())
			Layers.erase(Layers.begin() + index);
	}

	RenderLayer* Renderer::getLayer(unsigned int index) {
		if (index < Layers.size())
			return &Layers[index];

		return nullptr;
	}

	void Renderer::clearLayer(unsigned int index) {
		if (index < Layers.size())
		{
			auto copy = Layers[index].RenderList;
			for (std::shared_ptr<Actor>& A : copy) removeActor(A->getID());
		}

	}


	void Renderer::addActor(std::shared_ptr<Actor> A, unsigned int Layer) {
		if (!A) return;

		if (Layer < Layers.size())
		{
			Layers[Layer].RenderList.push_back(A);
			Layers[Layer].changed = true;

			ActorMap.insert(std::make_pair(A->getID(), std::make_pair(A, Layer)));
		}
	}

	void Renderer::updateActor(const ActorID& ID, std::shared_ptr<Actor>& A) {
		if (!ActorMap.count(ID) || !A->enabled) return;

		ActorInfo AI = ActorMap[ID];
		AI.first = A;
		std::vector<std::shared_ptr<Actor>>& RenderList = Layers[AI.second].RenderList;

		auto it = std::find(RenderList.begin(), RenderList.end(), AI.first);
		if (it != RenderList.end())
			*it = A;

		ActorMap[ID] = AI;

		Layers[AI.second].changed = true;
	}

	std::shared_ptr<Actor> Renderer::getActor(const ActorID& ID) {
		return ActorMap[ID].first;
	}

	void Renderer::removeActor(const ActorID& ID) {
		if (!ActorMap.count(ID)) return;

		ActorInfo AI = ActorMap[ID];
		std::vector<std::shared_ptr<Actor>>& RenderList = Layers[AI.second].RenderList;

		auto it = std::find(RenderList.begin(), RenderList.end(), AI.first);
		if (it != RenderList.end())
			RenderList.erase(it);
		ActorMap.erase(ID);

		Layers[AI.second].changed = true;
	}

	void Renderer::addActors(std::shared_ptr<std::vector<std::shared_ptr<Actor>>> Actors, unsigned int Layer) {
		for (auto& actor : *Actors) addActor(actor, Layer);
	}

} // namespace sbe
