
%module sbeGeom
%{
#include "sbe/geom/Point.hpp"

template class sbe::Geom::point<int>;
template class sbe::Geom::point<float>;
%}

%include "sbe/geom/Point.hpp"

%template(Point) sbe::Geom::point<int>;
%template(Pointf) sbe::Geom::point<float>;
%template(Vec2f) sbe::Geom::point<float>;
%template(Vec2) sbe::Geom::point<int>;
