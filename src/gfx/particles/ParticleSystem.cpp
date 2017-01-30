#include <sbe/gfx/particles/ParticleSystem.hpp>

#include <sbe/gfx/particles/Manipulators.hpp>

#include <sbe/Module.hpp>

namespace sbe
{

	ParticleSystem::ParticleSystem() {
		Vertices[0] = std::make_shared<sf::VertexArray>();
		Vertices[1] = std::make_shared<sf::VertexArray>();

		Pool.InitThreads(cores);

		RegisterForEvent("PARTICLES_RECEIVED", [this](Event& e) {
			waitingforconfirmation = false;
			firstverts = !firstverts;
		});
	}

	ParticleSystem::~ParticleSystem() {
		Module::Get()->QueueEvent("DESTROY_PARTICLES", true);
		Pool.StopThreads();
	}

	void ParticleSystem::setCores(unsigned int f) {
		cores = f;
		Pool.InitThreads(f);
	}

	void ParticleSystem::addEffect(const std::shared_ptr<particles::Effect>& E) {
		E->EffectId = Effects.size();
		Effects.push_back(E);
	}


	void ParticleSystem::setRenderer(Renderer R, unsigned int partverts, sf::PrimitiveType primtype) {
		getVertices().setPrimitiveType(primtype);
		particleverts = partverts;
		Rendr = R;
	}

	void ParticleSystem::executeManipulator(Manipulator M) {
		M(Particles, 0);
	}


	void ParticleSystem::executeAffector(Affector A) {
		for (Particle& P : Particles)
			A(P, 0);
	}

	void ParticleSystem::executeGlobalAffector(GlobalAffector G) {
		G(Particles.begin(), Particles.end(), 0);
	}

	void ParticleSystem::generateParticles(Generator G) {
		G(Particles);
	}

	void ParticleSystem::simulateStep() {
		if (!Effects.size()) return;

		using particles::Effect;
		float delta = Time.restart().asSeconds();

		for (std::shared_ptr<Effect>& E : Effects)
			E->Prepare();

		//Engine::out() << "delta: " << delta << std::endl;
		for (std::shared_ptr<Effect>& E : Effects)
			E->Manipulator(Particles, delta);

		for (std::shared_ptr<Effect>& E : Effects)
			E->GlobalAffector(Particles.begin(), Particles.end(), delta);

		std::function<void(Particle&)> j = [this, delta](Particle& P) {
			Effects[P.effectindex]->Affector(P, delta);
		};
		Pool.setVectorJob(j);
		Pool.runVectorJob(Particles);

		particles::manipulators::destroyOld(Particles, 0);

		if (!waitingforconfirmation && RenderTime.getElapsedTime().asSeconds() > 1/35)
		{
			Module::Get()->DebugString("particles", std::to_string(Particles.size()));
			RenderTime.restart();
			runRenderJob();
		}
	}

	void ParticleSystem::runRenderJob() {
		typedef std::pair<std::ptrdiff_t, std::ptrdiff_t> ThreadData;
		typedef std::vector<ThreadData> ThreadsData;

		getVertices().resize(particleverts*Particles.size());

		std::function<void(boost::any&, int)> Job = [&](boost::any& data, int tid) {
			auto range = boost::any_cast<ThreadsData>(data);
			auto from = range[tid].first;
			auto to = range[tid].second;

			for (unsigned int i = from; i != to; ++i)
				Rendr(Particles[i], &(getVertices()[i*particleverts]));
		};

		Pool.setCustomJob(Job);
		ThreadsData chunks = chunkInts(Particles.size(), cores);
		Pool.runCustomJob(boost::any(chunks));

		Module::Get()->QueueEvent(sbe::Event("UPDATE_PARTICLE_VERTICES", Vertices[firstverts]), true);
	}

	sf::VertexArray& ParticleSystem::getVertices() { return *(Vertices[firstverts]); }


}
