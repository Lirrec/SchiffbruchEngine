#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <sbe/event/EventUser.hpp>
#include <sbe/gfx/particles/Particle.hpp>
#include <sbe/util/ThreadPool.hpp>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Clock.hpp>

#include <functional>
#include <vector>

namespace sbe
{

	/**

		A simple ParticleSystem. It uses functional composition to manage particle generators, particle affectors and a renderer to create and animate a particlesystem

	*/
	class ParticleSystem : public EventUser
	{
		public:
			/// fills a vertexarray with a graphical representation particles
			typedef std::function< void(Particle&, sf::Vertex*) > Renderer;
			/// makes some kind of computation on a Particle
			typedef std::function< void(Particle&, float) > Affector;
			/// makes some kind of computation on all Particles ( allows interaction )
			typedef std::function< void(Particle::Iterator, Particle::Iterator, float) > GlobalAffector;
			/// fills the given vector with particles
			typedef std::function< void(std::vector<Particle>&) > Generator;
			/// generates or deletes new particles at runtime
			typedef std::function< void(std::vector<Particle>&, float) > Manipulator;

			/// create a ParticleSystem
			ParticleSystem();
			~ParticleSystem();

			/// indicate to the ParticleSystem how often simulateStep will be called each second
			void setFps( unsigned int f ) { fps = f; }
			void setCores( unsigned int f );

			/// use the given Renderer  to generate the Vertexarray, the renderer has to use exactly partverts vertices per particle
			void setRenderer( Renderer R, unsigned int partverts, sf::PrimitiveType primtype );
			/// add an Affector and run it every frame on all Particles
			void addAffector( Affector A );
			/// add a GlobalAffector and run it every frame
			void addGlobalAffector( GlobalAffector GA );
			/// add an Manipulator and run it every frame
			void addManipulator( Manipulator E );

			/// run the given Generator
			void generateParticles( Generator G );

			/// execute the given affector once on all Particles ( the delta param will be set to 0! )
			void executeAffector( Affector A );
			/// execute the given affector once  ( the delta param will be set to 0! )
			void executeGlobalAffector( GlobalAffector G);
			/// execute an Manipulator once  ( the delta param will be set to 0! )
			void executeManipulator( Manipulator E );

			/** Simulate on physics step.
				Runs all GlobalAffectors, then all Affectors and creates the internal Vertexarray with the set Generator.
			*/
			void simulateStep();

			/// Get a reference to the sf::VertexArray representing the ParticleSystem
			sf::VertexArray& getVertices();

		private:

			void runRenderJob();

			Geom::Vec2 Size;
			sf::Clock Time;
			sf::Clock RenderTime;

			sbe::ThreadPool Pool;
			std::vector<Particle> Particles;


			std::vector<Affector> Affectors;
			std::vector<GlobalAffector> GlobalAffectors;
			std::vector<Manipulator> Manipulators;
			Renderer Rendr;
			unsigned int particleverts = 0;

			unsigned int fps = 60;
			unsigned int cores = 2;


			std::shared_ptr<sf::VertexArray> Vertices[2];
			bool firstverts = false;
			bool waitingforconfirmation = false;
	};

}


#endif // PARTICLESYSTEM_HPP

