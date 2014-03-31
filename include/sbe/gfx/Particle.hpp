#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "sbe/geom/Point.hpp"

namespace sbe	{

	class Particle {
		public:
			Particle( Geom::Vec2f _p) : position(_p) {}

			Geom::Vec2f velocity = {0,0};
			Geom::Vec2f position = {0,0};
			float rotation = 0;
			float mass = 1;
	};


}

#endif // PARTICLE_HPP
