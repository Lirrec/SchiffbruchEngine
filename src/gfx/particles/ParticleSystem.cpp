#include "sbe/gfx/particles/ParticleSystem.hpp"

#include <sbe/Module.hpp>
#include <sbe/event/Event.hpp>

#include <functional>

namespace sbe {

	ParticleSystem::ParticleSystem()
	{
		Pool.InitThreads(5);
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::addAffector(Affector A)
	{
		Affectors.push_back( A );
	}

	void ParticleSystem::setRenderer(Renderer R)
	{
		Rendr = R;
	}
	void ParticleSystem::addManipulator(Manipulator M)
	{
		Manipulators.push_back(M);
	}

	void ParticleSystem::executeManipulator(Manipulator M)
	{
		M(Particles, 0);
	}


	void ParticleSystem::executeAffector(Affector A)
	{
		for ( Particle& P : Particles)
				A(P, 0);
	}

	void ParticleSystem::executeGlobalAffector(GlobalAffector G)
	{
		G(Particles.begin(), Particles.end(), 0);
	}


	void ParticleSystem::addGlobalAffector(GlobalAffector GA)
	{
		GlobalAffectors.push_back( GA );
	}

	void ParticleSystem::generateParticles(Generator G)
	{
		G( Particles );
	}

	void ParticleSystem::simulateStep()
	{
		float delta = Time.restart().asSeconds();

		//Engine::out() << "delta: " << delta << std::endl;
		Module::Get()->DebugString( "particles", std::to_string(Particles.size()));


        for ( Manipulator& E : Manipulators)
            E(Particles, delta);

        for ( GlobalAffector& G : GlobalAffectors)
            G(Particles.begin(), Particles.end(), delta);

		for ( Affector& A : Affectors)
		{
			//Engine::out() << "Affector, delta " << delta << std::endl;
			std::function<void(Particle&)> j = std::bind(A, std::placeholders::_1, delta);
			Pool.runJobOnVector( Particles, j );
		}

		if ( RenderTime.getElapsedTime().asSeconds() > 1/35  )
		{
			Rendr(Particles, Vertices);
			Module::Get()->QueueEvent( sbe::Event("UPDATE_PARTICLE_VERTICES", Vertices), true );
			RenderTime.restart();
		}
	}

	sf::VertexArray& ParticleSystem::getVertices() { return Vertices; }


}
