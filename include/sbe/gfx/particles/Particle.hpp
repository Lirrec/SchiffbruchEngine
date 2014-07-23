#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "sbe/geom/Point.hpp"

#include <SFML/Graphics/Color.hpp>

#include <iterator>
#include <vector>

namespace sbe	{

	class Particle {
		public:
			Particle() {};
			Particle( glm::vec2 _p) : position(_p) {}

			typedef std::vector<Particle>::iterator Iterator;

			/// current velocity/speed
			glm::vec2 velocity = {0,0};
			/// current positionglm::vec2
			glm::vec2 position = {0,0};

			/// the rotation of the particel in degress/radian ( not yet decided )
			float rotation = 0;
			/// angular velocity
			float avelocity = 0;


			/// the color of the particle
			sf::Color color = sf::Color::White;
			/// the age of the particle in a range from 0 to 1
			float age = 0;
			/// the mass/size of the particle
			float size = 1;
			/// which effect should compute this particle
			short effectindex = 0;
	};


}

#endif // PARTICLE_HPP
