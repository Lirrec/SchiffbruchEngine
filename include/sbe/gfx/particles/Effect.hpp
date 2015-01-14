#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <sbe/gfx/particles/Particle.hpp>



namespace sbe
{
	class ParticleSystem;

	namespace particles
	{
		/**
			An Effect is a group of functions manipulation particles.
			Its groups together an Affector(), a Manipulator() and a globalAffector() ( all optional ).
		*/
		class Effect
		{
			public:
				Effect();
				virtual ~Effect();

				/// called once each frame before the other functions are executed
				/// use this to compute values you only need to compute once for all particles
				virtual void Prepare() {};

				/// affect single particles
				virtual void Affector( Particle& P, float delta) {};


				/** may modify all particles at once ( allow interaction, will not be executed in parallel )
					@note remember to modifiy only particles where P.effectindex == EffectId
				*/
				virtual void GlobalAffector( Particle::Iterator, Particle::Iterator, float ) {};


				/** may insert or remove particles
					@note remember to modifiy only particles where P.effectindex == EffectId
				*/
				virtual void Manipulator( std::vector<Particle>&, float ) {};

				friend class ::sbe::ParticleSystem;
			protected:
				/// set by the particlesystem
				short EffectId = -1;

		};
	} // namespace particles
} // namespace sbe

#endif // EFFECT_HPP

