#include "Graphic.h"

#include "Module.h"

#include <boost/uuid/uuid_generators.hpp>

ViewActor::ViewActor( bool _dynamic, Geom::Point _ScreenPosition, Geom::Point _QuadTreePosition, RenderLayer _Layer, const std::string& _ImageSetName, int _Frame)
   : dynamic(_dynamic),
   ID(Module::NewUUID()),
   ScreenPosition(_ScreenPosition),
   QuadTreePosition(_QuadTreePosition),
   Layer(_Layer),
   ImageSetName(_ImageSetName),
   Frame(_Frame)
{

}

ViewActor::ViewActor( bool _dynamic, Geom::Point _ScreenPosition, Geom::Point _QuadTreePosition, RenderLayer _Layer, const std::string& _ImageSetName, int _Frame, boost::uuids::uuid _ID )
   : dynamic(_dynamic),
   ID(_ID),
   ScreenPosition(_ScreenPosition),
   QuadTreePosition(_QuadTreePosition),
   Layer(_Layer),
   ImageSetName(_ImageSetName),
   Frame(_Frame)
{

}
