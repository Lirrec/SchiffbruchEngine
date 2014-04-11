#include "sbe/gfx/particles/ParticleSystem.hpp"

#include <sbe/Module.hpp>
#include <sbe/event/Event.hpp>

#include <functional>

namespace sbe {

	ParticleSystem::ParticleSystem()
	{
		Vertices[0].reset ( new sf::VertexArray );
		Vertices[1].reset ( new sf::VertexArray );

		Pool.InitThreads(cores);
	}

	ParticleSystem::~ParticleSystem()
	{
		Module::Get()->QueueEvent( "DESTROY_PARTICLES", true );
		Pool.StopThreads();
	}

	void ParticleSystem::setCores(unsigned int f)
	{
		cores = f;
		Pool.InitThreads(f);
	}

	void ParticleSystem::HandleEvent(Event& e)
	{
		if ( e.Is("VERTICES_RECEIVED"))
		{
			waitingforconfirmation = false;
			firstverts = !firstverts;
		}
	}

	void ParticleSystem::addAffector(Affector A)
	{
		Affectors.push_back( A );
	}

	void ParticleSystem::setRenderer(Renderer R, unsigned int partverts, sf::PrimitiveType primtype)
	{
		getVertices().setPrimitiveType( primtype );
		particleverts = partverts;
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

		for ( Affector& A : Affectors)
		{
			//Engine::out() << "Affector, delta " << delta << std::endl;
			std::function<void(Particle&)> j = std::bind(A, std::placeholders::_1, delta);
			Pool.setVectorJob( j );
			Pool.runVectorJob( Particles );
		}

		if ( !waitingforconfirmation && RenderTime.getElapsedTime().asSeconds() > 1/35  )
		{
			Module::Get()->DebugString( "particles", std::to_string(Particles.size()));
			RenderTime.restart();
			runRenderJob();
		}
	}

	void ParticleSystem::runRenderJob()
	{
		getVertices().resize( particleverts*Particles.size() );

		std::function<void(boost::any&, int)> Job = [&](boost::any& data, int tid)
		{
			auto range = boost::any_cast< std::vector<std::pair<std::ptrdiff_t,std::ptrdiff_t>> >(data);
			auto from = range[tid].first;
			auto to = range[tid].second;

			for (unsigned int i = from; i != to; ++i)
				Rendr( Particles[i], &(getVertices()[i*particleverts]) );
		};

		Pool.setCustomJob( Job );
		std::vector<std::pair<std::ptrdiff_t,std::ptrdiff_t>> chunks = chunkInts(Particles.size(), cores);
		Pool.runCustomJob( boost::any(chunks) );

		Module::Get()->QueueEvent( sbe::Event("UPDATE_PARTICLE_VERTICES", Vertices[firstverts]), true );
	}

	sf::VertexArray& ParticleSystem::getVertices() { return *(Vertices[firstverts]); }


}
