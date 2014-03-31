#include "sbe/gfx/ParticleAffectors.hpp"


#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <sbe/gfx/Screen.hpp>
#include <sbe/gfx/Renderer.hpp>
#include <sbe/gfx/Camera.hpp>
#include <sbe/geom/PointHelpers.hpp>
#include <sbe/Engine.hpp>
#include <cmath>

namespace sbe
{

	namespace particles
	{
		namespace affectors
		{

			void move(Particle& P, float delta, float dampening)
			{
				P.position += P.velocity * delta;
				P.velocity *= std::pow(dampening,delta);
			}

			void applyGravitation(Particle& P, float delta, Geom::Pointf pos, float m)
			{
				Geom::Vec2f dist = pos - P.position;

				if ( dist.x == 0 && dist.y == 0) return;

				float accel = (P.mass*m) / Geom::length(dist);
				//float accel = (mass*m) / (Geom::length(dist)*Geom::length(dist));
				P.velocity += Geom::normalize(dist) * (accel * delta);

				/*
				Engine::out() << "pos: " << pos << " pos2: " << position << std::endl;
				Engine::out() << "M1*m2: " << mass << "*" << m << "; dist^2: " << dist*dist << " -- " << velocity << std::endl;
				*/
			}

			void applyGravity(Particle& P, float delta, Geom::Vec2f direction, float factor)
			{
				P.velocity += Geom::normalize(direction)*factor * delta;
			}

			void collideBoxed(Particle& P, float delta, Geom::Vec2f limits)
			{
				if ( P.position.x > limits.x)
				{
					P.position.x = limits.x;
					P.velocity.x = 0;
				}
				else if ( P.position.x < 0 )
				{
					P.position.x = 0;
					P.velocity.x = 0;
				}

				if ( P.position.y > limits.y)
				{
					P.position.y = limits.y;
					P.velocity.y = 0;
				}
				else if ( P.position.y < 0 )
				{
					P.position.y = 0;
					P.velocity.y = 0;
				}
			}

			void collideBoxedBouncy(Particle& P, float delta, Geom::Vec2f limits, float forceloss, float min)
			{
				if ( P.position.x > limits.x+min)
				{
					if ( P.velocity.x > min)
					{
						P.position.x = limits.x -( P.position.x - limits.x);
						P.velocity.x *= -forceloss;
					}
					else
					{
						P.position.x = limits.x;
						P.velocity.x = 0;
					}
				}
				else if ( P.position.x < 0 )
				{
					if ( P.velocity.x > min )
					{
						P.position.x = -P.position.x;
						P.velocity.x *= -forceloss;
					}
					else
					{
						P.position.x = 0;
						P.velocity.x = 0;
					}
				}

				if ( P.position.y > limits.y)
				{
					if ( P.velocity.y > min)
					{
						P.position.y = limits.y -( P.position.y - limits.y);
						P.velocity.y *= -forceloss;
					}
					else
					{
						P.position.y = limits.y;
						P.velocity.y = 0;
					}

				}
				else if ( P.position.y < 0 )
				{
					if ( P.velocity.x > min )
					{
						P.position.y = -P.position.y;
						P.velocity.y *= -forceloss;
					}
					else
					{
						P.position.y = 0;
						P.velocity.y = 0;
					}
				}
			}

			void collideWarped(Particle& P, float delta, Geom::Vec2f limits)
			{
				P.position.x = P.position.x > 0 ? std::fmod(P.position.x, limits.x) : limits.x +P.position.x;
				P.position.y = P.position.y > 0 ? std::fmod(P.position.y, limits.y) : limits.y +P.position.y;
			}

			void applyMouseGravitation(Particle& P, float delta, float mass)
			{
				if ( sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					auto pos = sf::Mouse::getPosition( (sf::Window&)Engine::GetApp() );
					auto vpos = Engine::GetApp().mapPixelToCoords( pos, sbe::Screen::sRndr()->getLayer(1)->Cam->getView());
					//Engine::out() << "mouseinwinPos: " << pos.x << "/" << pos.y << " - vpos: " << vpos.x << "/" << vpos.y << std::endl;

					applyGravitation( P, delta, {vpos.x,vpos.y}, mass);
				}
			}

		}

		namespace renderers {
			void renderPoints(std::vector<Particle>& Particles, sf::VertexArray& V)
			{
				V.resize(Particles.size());

				int i = 0;
				for ( Particle& p : Particles)
				{
					V[i].position.x = p.position.x;
					V[i].position.y = p.position.y;
					++i;
				}
			}
		}

		namespace generators {
			void generateGrid( std::vector<Particle>& Particles, Geom::Vec2 Size, Geom::Vec2 Count )
			{
				Particles.clear();

				Geom::Vec2f steps { Size.x/(float)Count.x, Size.y/(float)Count.y };

				Geom::Pointf pos;
				for ( float x = 0; x < Count.x; ++x)
				{
					pos.x += steps.x;
					pos.y = 0;
					for ( float y = 0; y < Count.y; ++y)
					{
						pos.y += steps.y;
						Particles.push_back( Particle( pos ) );
						//Engine::out() << "Creating particle at: " << x << "," << y << std::endl;
					}
				}
			}
		}


	}

}
