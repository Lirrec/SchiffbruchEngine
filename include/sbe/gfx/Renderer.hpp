#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "sbe/gfx/Actor.hpp"
#include "sbe/event/EventUser.hpp"

#include "sbe/util/BoostUUIDHash.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Clock.hpp>



#include <vector>
#include <unordered_map>
#include <memory>

class Camera;

/**
	Defines a RenderLayer
*/
class RenderLayer
{
	public:
		RenderLayer() :
			isActive(true),
			cull(false),
			changed(true) {}

		RenderLayer( std::shared_ptr<Camera> C, sf::RenderStates S )
		: RenderLayer()
		{
			Cam = C;
			States = S;
		}

		RenderLayer( std::shared_ptr<Camera> C )
		: RenderLayer()
		{
			Cam = C;
		}

		RenderLayer( sf::RenderStates S )
		: RenderLayer() {
			States = S;
		}

		/// enable or disable rendering of this layer
		bool isActive;
		/// enable or disable culling
		bool cull;

		/// the renderstates to use ( for setting a custom texture for vertexarrays or shaders
		sf::RenderStates States;
		std::shared_ptr<Camera> Cam;

		friend class Renderer;
	private:

		/// did the RenderList change since last frame ( used to determine a recull
		bool changed;

		std::vector< std::shared_ptr<Actor> > RenderList;
};

/**
	This Class provides a simple Renderer for 2D Games.
	It supports an arbitrary amount of Renderlayers and manages a list of Actors on each Layer.
	It is possible to add/remove/update both Layers and Actors at any time.
	Each layer can be configured to use a specific set of renderstates, a Camera/View (optional) and can be toggled on/off at any time.
	Layers indices start at 0.

	The engine features a few Actor subclasses for different use ( Sprite, Animation, VertexArray )
	@see Actor

	@note Culling is not yet implemented.

	A few tips for efficient rendering in General:
		- avoid using a large amount of sprites ( e.g. for Tilemaps ), use a Vertexarray instead
		- avoid many textureswitches ( draw everything that uses one texture together ( e.g. on one layer )
*/
class Renderer : EventUser
{
	public:
		Renderer();

		~Renderer() {};

		/**
			Events handled:
			Event			| 	Data
			----------------|---------------------------------------------
			ADD_ACTOR		|	std::shared_ptr<Actor>
			UPDATE_ACTOR	|	std::pair<ActorID, std::shared_ptr<Actor>>
			REMOVE_ACTOR	|	std::pair<ActorID, std::shared_ptr<Actor>>
		*/
		void HandleEvent( Event& e );

		// - Render Interface -
		/**
			Render all active Layers to the given RenderTarget and use Camera C
		*/
		void render( sf::RenderTarget& t);
		/**
			Render a single Layer to the given Target.
			This ignores the isActive setting of the Layer.
		*/
		void renderSingleLayer(int index, sf::RenderTarget& t);

		// - Layer Management -
		/**
			Add a RenderLayer.
			@param L the new RenderLayer
			@param index the index of the new Layer will be inserted BEFORE Layer[index], default is -1 ( append at the end )
		*/
		void addLayer( const RenderLayer& L, const int index = -1 );

		/**
			Remove a RenderLayer by index.
			@param index the index of the Layer which should be removed
		*/
		void removeLayer ( int index );

		/**
			Get Access to a Layer
			@param index the index of the Layer
			@return a pointer to the requested Layer or nullptr if the index is invalid
		*/
		RenderLayer* getLayer( int index );

		/**
			Remove all Actors from a Layer
			@param index the index of the Layer
		*/
		void clearLayer( int index );

		// - Actor Management -
		/**
			Add a new Actor
			@param A the actor
			@param Layer which layer should the actor be rendered on
		*/
		void addActor( std::shared_ptr<Actor>& A, int Layer );

		/**
			Update an Actor.
			@param ID the ID of the Actor to update
			@param A the updated Version of the Actor
		*/
		void updateActor( const ActorID& ID, std::shared_ptr<Actor>& A);

		/**
			Get Access to an Actor.
			@param ID the ID of the requested
			@return a shared_ptr to the Actor, empty if the Actor was not found
		*/
		std::shared_ptr<Actor> getActor( const ActorID& ID );

		/**
			Remove an Actor
			@param ID the ID of the Actor to remove
		*/
		void removeActor( const ActorID& ID );

	private:

		void updateLayer( RenderLayer& L );
		/**
			Cull the layer.
		*/
		void cullLayer( RenderLayer& L );
		void drawLayer(const RenderLayer& L, sf::RenderTarget& t);

		/// used to time Animations
		sf::Clock RenderTime;

		typedef std::pair<std::shared_ptr<Actor>, int> ActorInfo;
		typedef std::unordered_map<ActorID, ActorInfo> ActorList;
		ActorList ActorMap;

		std::vector<RenderLayer> Layers;
};

#endif // RENDERER_HPP

