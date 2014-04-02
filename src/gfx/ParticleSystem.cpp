#include "sbe/gfx/ParticleSystem.hpp"

#include <sbe/Module.hpp>
#include <sbe/event/Event.hpp>

namespace sbe {

	void ParticleSystem::addAffector(Affector A)
	{
		Affectors.push_back( A );
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
		float delta = Time.restart().asSeconds() * fps;

		//Engine::out() << "delta: " << delta << std::endl;

        for ( GlobalAffector& G : GlobalAffectors)
            G(Particles.begin(), Particles.end(), delta);

		for ( Particle& P : Particles)
			for ( Affector& A : Affectors)
				A(P, delta);



		Rendr(Particles, Vertices);

		Module::Get()->QueueEvent( sbe::Event("UPDATE_PARTICLE_VERTICES", Vertices), true );
	}

	sf::VertexArray& ParticleSystem::getVertices() { return Vertices; }

	ParticleSystem::ParticleSystem( Renderer R )
	 : Rendr(R)
	{

	}

	ParticleSystem::~ParticleSystem()
	{

	}

}
