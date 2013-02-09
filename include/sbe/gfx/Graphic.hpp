#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <boost/uuid/uuid.hpp>

#include "Geom.hpp"

#include <string>
#include <memory>

enum RenderLayer
{
	WIN_BG = 0,

	TERRAIN_BG,
	TERRAIN_TILE,
		/// this is required to render tiles on tiles, as things as selected tiles
	/// and the current path are rendered as the highlight being rendered on top
	/// of the tiles
	TERRAIN_TILE2,
	TERRAIN_OBJ,
	TERRAIN_GUY,

	LAYER_COUNT
};

class ViewActor
{
	public:

	ViewActor( bool _dynamic, Geom::Point _ScreenPosition, Geom::Point _QuadTreePosition, RenderLayer _Layer, const std::string& _ImageSetName, int _Frame);
	ViewActor( bool _dynamic, Geom::Point _ScreenPosition, Geom::Point _QuadTreePosition, RenderLayer _Layer, const std::string& _ImageSetName, int _Frame, boost::uuids::uuid _ID );

	// this typedefs define the type of the arguments to the VIEW_ACTOR_* Events
	typedef std::pair< RenderLayer, Geom::Rect > EnableQuadTreeData;
	typedef std::shared_ptr<ViewActor> ActorData;
	typedef std::pair < boost::uuids::uuid /*ActorID*/, RenderLayer > ActorDeleteData;

	/// is this actor represented by a static sprite or an animation?
	bool dynamic;

	boost::uuids::uuid ID;
	/// where to render this actor
	Geom::Point ScreenPosition;
	/// where to put it in the quadtree (usually the midpoint of the actor)
	Geom::Point QuadTreePosition;
	/// which layer should it be rendered on
	RenderLayer Layer;

	/// The name of the imageset used to obtain the single frame or the animation
	std::string ImageSetName;
	/// defines which frame to use for static sprites
	/// for animations it defines the starting frame, or if it is "-1" the animation will be started at a random frame+time offset
	int 		Frame;
};

#endif // GRAPHIC_H
