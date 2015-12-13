#ifndef AFFECTORS_HPP
#define AFFECTORS_HPP

#include <sbe/geom/Point.hpp>
#include <sbe/geom/PointHelpers.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

#include <sbe/gfx/particles/Particle.hpp>


#include <functional>
#include <cmath>

namespace sbe
{
	namespace particles
	{
		namespace affectors
		{
			/// moves a particle according
			inline void move(Particle& P, float delta, float dampening = 0.95f) {
				P.position += P.velocity*delta;
				P.velocity *= std::pow(dampening, delta);

				P.rotation = std::fmod(P.rotation + P.avelocity*delta, 2*glm::pi<float>());
				P.avelocity *= std::pow(dampening, delta);
			}

			/// increases the age of a particle
			inline void age(Particle& P, float delta, float maxage) {
				P.age += delta/maxage;
			}

			/// applies a gravitational pull to a particle
			inline void applyGravitation(Particle& P, float delta, glm::point2 pos, float m) {
				glm::vec2 dist = pos - P.position;
				if (dist.x == 0 && dist.y == 0) return;

				float accel = (P.size*m)/(glm::length(dist)*glm::length(dist));
				P.velocity += glm::normalize(dist)*(accel*delta);
			}

			/// applies a gravitational pull with linear falloff to a particle
			inline void applyLinearGravitation(Particle& P, float delta, glm::point2 pos, float m) {
				glm::vec2 dist = pos - P.position;
				if (dist.x == 0 && dist.y == 0) return;

				float accel = (P.size*m)/glm::length(dist);
				P.velocity += glm::normalize(dist)*(accel*delta);
			}

			/// applies a gravity ( down ) to a particle
			inline void applyGravity(Particle& P, float delta, glm::vec2 direction, float factor = 1.0f) {
				P.velocity += glm::normalize(direction)*factor*delta;
			}

			/// performs a collision with a bounding box
			inline void collideBoxed(Particle& P, float delta, glm::vec2 limits) {
				if (P.position.x > limits.x)
				{
					P.position.x = limits.x;
					P.velocity.x = 0;
				}
				else if (P.position.x < 0)
				{
					P.position.x = 0;
					P.velocity.x = 0;
				}

				if (P.position.y > limits.y)
				{
					P.position.y = limits.y;
					P.velocity.y = 0;
				}
				else if (P.position.y < 0)
				{
					P.position.y = 0;
					P.velocity.y = 0;
				}
			}

			/// performs a collision with a bounding box ( bouncy )
			inline void collideBoxedBouncy(Particle& P, float delta, glm::vec2 limits, float forceloss, float min) {
				if (P.position.x > limits.x)
				{
					if (P.velocity.x > min)
					{
						P.position.x = limits.x - (P.position.x - limits.x);
						P.velocity.x *= -forceloss;
					}
					else
					{
						P.position.x = limits.x;
						P.velocity.x = 0;
						P.velocity.y *= std::pow(forceloss, delta);
						P.avelocity *= std::pow(forceloss, delta);
					}
				}
				else if (P.position.x < 0)
				{
					if (std::fabs(P.velocity.x) > min)
					{
						P.position.x = -P.position.x;
						P.velocity.x *= -forceloss;
					}
					else
					{
						P.position.x = 0;
						P.velocity.x = 0;
						P.velocity.y *= std::pow(forceloss, delta);
						P.avelocity *= std::pow(forceloss, delta);
					}
				}

				if (P.position.y > limits.y)
				{
					if (P.velocity.y > min)
					{
						P.position.y = limits.y - (P.position.y - limits.y);
						P.velocity.y *= -forceloss;
					}
					else
					{
						P.position.y = limits.y;
						P.velocity.y = 0;
						P.velocity.x *= std::pow(forceloss, delta);
						P.avelocity *= std::pow(forceloss, delta);
					}

				}
				else if (P.position.y < 0)
				{
					if (std::fabs(P.velocity.y) > min)
					{
						P.position.y = -P.position.y;
						P.velocity.y *= -forceloss;
					}
					else
					{
						P.position.y = 0;
						P.velocity.y = 0;
						P.velocity.x *= std::pow(forceloss, delta);
						P.avelocity *= std::pow(forceloss, delta);
					}
				}
			}

			/// performs a warp to the other side of the particle system if a particle leaves one side
			inline void collideWarped(Particle& P, float delta, glm::vec2 limits) {
				P.position.x = P.position.x > 0 ? std::fmod(P.position.x, limits.x) : limits.x + std::fmod(P.position.x, -limits.x);
				P.position.y = P.position.y > 0 ? std::fmod(P.position.y, limits.y) : limits.y + std::fmod(P.position.y, -limits.y);
			}

			template<typename T>
			void set(Particle& P, float delta, std::function<T&(Particle&)> member, std::function<T(float)>& value) {
				member(P) = value(delta);
			}

		}
	} // namespace particles


} // namespace sbe

#endif // AFFECTORS_HPP

