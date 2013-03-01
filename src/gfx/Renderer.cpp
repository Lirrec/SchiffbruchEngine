#include "sbe/gfx/Renderer.hpp"
#include "sbe/gfx/Camera.hpp"


#include <algorithm>



Renderer::Renderer()
{
	RegisterForEvent( "ADD_ACTOR" );
	RegisterForEvent( "UPDATE_ACTOR" );
	RegisterForEvent( "REMOVE_ACTOR" );
}

/*	Events handled:
	Event			| 	Data
	----------------+---------------------------------------------
	ADD_ACTOR		|	std::pair<std::shared_ptr<Actor>, int>
	UPDATE_ACTOR	|	std::shared_ptr<Actor>
	REMOVE_ACTOR	|	ActorID
*/
void Renderer::HandleEvent(Event& e)
{
	if ( e.Is ("ADD_ACTOR") )
	{
		if ( e.Data().type() == typeid ( std::pair<std::shared_ptr<Actor>, int> ) )
		{
			ActorInfo AI = boost::any_cast<ActorInfo>( e.Data() );
			addActor( AI.first, AI.second );
		}
	} else if ( e.Is ("UPDATE_ACTOR") )
	{
		if ( e.Data().type() == typeid ( std::shared_ptr<Actor> ) )
		{
			std::shared_ptr<Actor> A = boost::any_cast<std::shared_ptr<Actor>>( e.Data() );
			updateActor( A->getID(), A );
		}
	} else if ( e.Is ("REMOVE_ACTOR") )
	{
		if ( e.Data().type() == typeid ( ActorID ) )
		{
			ActorID ID = boost::any_cast<ActorID>( e.Data() );
			removeActor( ID );
		}
	}
}

void Renderer::render(sf::RenderTarget& t)
{
	for ( RenderLayer& L : Layers)
	{
		if ( L.isActive ) updateLayer( L );
		if ( L.cull && L.changed ) cullLayer( L );
		if ( L.isActive ) drawLayer( L, t );
	}
}


void Renderer::renderSingleLayer(int index, sf::RenderTarget& t)
{
	if ( index >= 0 && index < Layers.size())
		drawLayer( Layers[index], t );
}

void Renderer::updateLayer(RenderLayer& L)
{
	for ( std::shared_ptr<Actor>& A : L.RenderList)
	{
		A->update( RenderTime.getElapsedTime() );
	}
}

void Renderer::cullLayer(RenderLayer& L)
{
	// do culling, not yet implemented

	L.changed = false;
}

void Renderer::drawLayer(const RenderLayer& L, sf::RenderTarget& t)
{
	if ( L.Cam ) t.setView( L.Cam->getView() );

	for ( const std::shared_ptr<Actor>& A : L.RenderList)
		t.draw( A->getDrawable(), L.States );
}


void Renderer::addLayer(const RenderLayer& L, const int index)
{
	if ( index == -1 ) Layers.push_back( L );
	if ( index > 0 && index < Layers.size() ) {
			auto it = Layers.begin() + index;
			Layers.insert( it, L);
	}
}

void Renderer::removeLayer(int index)
{
	if ( index >= 0 && index < Layers.size() )
		Layers.erase( Layers.begin() + index );
}

RenderLayer* Renderer::getLayer(int index)
{
	if ( index >= 0 && index < Layers.size() )
		return &Layers[index];
	return nullptr;
}

void Renderer::addActor(std::shared_ptr<Actor>& A, int Layer)
{
	if ( Layer >= 0 && Layer < Layers.size() )
	{
		Layers[Layer].RenderList.push_back( A );
		Layers[Layer].changed = true;

		ActorMap.insert( std::make_pair( A->getID(), std::make_pair(A, Layer) ));
	}
}

void Renderer::updateActor(const ActorID& ID, std::shared_ptr<Actor>& A)
{
	if (!ActorMap.count( ID )) return;

	ActorInfo AI = ActorMap[ID];
	AI.first = A;
	std::vector<std::shared_ptr<Actor>>& RenderList = Layers[AI.second].RenderList;

	auto it = std::find( RenderList.begin(), RenderList.end(), AI.first );
	if ( it != RenderList.end() )
			*it = A;

	ActorMap[ID] = AI;

	Layers[AI.second].changed = true;
}

std::shared_ptr<Actor> Renderer::getActor(const ActorID& ID)
{
	return ActorMap[ID].first;
}

void Renderer::removeActor(const ActorID& ID)
{
	if ( !ActorMap.count(ID)) return;

	ActorInfo AI = ActorMap[ID];
	std::vector<std::shared_ptr<Actor>>& RenderList = Layers[AI.second].RenderList;

	auto it = std::find( RenderList.begin(), RenderList.end(), AI.first );
	if ( it != RenderList.end() )
			RenderList.erase ( it );
	ActorMap.erase ( ID );

	Layers[AI.second].changed = true;
}

