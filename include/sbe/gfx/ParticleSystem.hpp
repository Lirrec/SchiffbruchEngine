#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <sbe/gfx/Particle.hpp>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

#include <functional>
#include <vector>

namespace sbe
{

	/**

		A simple ParticleSystem. It uses functional composition to manage particle generators, particle affectors and a renderer to create and animate a particlesystem

	*/
	class ParticleSystem
	{
		public:
			typedef std::function< void(std::vector<Particle>&, sf::VertexArray&) > Renderer;
			typedef std::function< void(Particle&, float) > Affector;
			typedef std::function< void(Particle::Iterator, Particle::Iterator, float) > GlobalAffector;
			typedef std::function< void(std::vector<Particle>&) > Generator;

			ParticleSystem( Renderer R );
			~ParticleSystem();

			void setFps( int f ) { fps = f; }

			void addAffector( Affector A );
			void addGlobalAffector( GlobalAffector GA );
			void generateParticles( Generator G );

			void simulateStep();
			sf::VertexArray& getVertices();

		private:
			Geom::Vec2 Size;
			sf::Clock Time;

			std::vector<Particle> Particles;
			std::vector<Affector> Affectors;
			std::vector<GlobalAffector> GlobalAffectors;
			Renderer Rendr;

			int fps = 60;

			sf::VertexArray Vertices;
	};

}


#endif // PARTICLESYSTEM_HPP

