#include "sbe/gfx/ParticleSystem.hpp"

#include <sbe/Module.hpp>
#include <sbe/event/Event.hpp>

namespace sbe {

	ParticleSystem::ParticleSystem()
	{

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

        for ( Manipulator& E : Manipulators)
            E(Particles, delta);

        for ( GlobalAffector& G : GlobalAffectors)
            G(Particles.begin(), Particles.end(), delta);

		for ( Particle& P : Particles)
			for ( Affector& A : Affectors)
				A(P, delta);

		Rendr(Particles, Vertices);

		Module::Get()->QueueEvent( sbe::Event("UPDATE_PARTICLE_VERTICES", Vertices), true );
	}

	sf::VertexArray& ParticleSystem::getVertices() { return Vertices; }


}
