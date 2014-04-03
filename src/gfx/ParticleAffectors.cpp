#include "sbe/gfx/ParticleAffectors.hpp"


#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <sbe/gfx/Screen.hpp>
#include <sbe/gfx/Renderer.hpp>
#include <sbe/gfx/Camera.hpp>
#include <sbe/gfx/VertexUtils.hpp>

#include <sbe/geom/PointHelpers.hpp>
#include <sbe/geom/Geom.hpp>
#include <sbe/util/FactoriesAndManipulators.hpp>
#include <sbe/Engine.hpp>

#include <cmath>
#include <random>

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

				P.rotation = std::fmod(P.rotation+P.avelocity*delta, 2*Geom::pi() );
				P.avelocity *= std::pow(dampening, delta);
			}

			void age(Particle& P, float delta, float maxage)
			{
				P.age += delta / maxage;
			}

			void applyGravitation(Particle& P, float delta, Geom::Pointf pos, float m)
			{
				Geom::Vec2f dist = pos - P.position;

				if ( dist.x == 0 && dist.y == 0) return;

				float accel = (P.size*m) / Geom::length(dist);
				//float accel = (size*m) / (Geom::length(dist)*Geom::length(dist));
				P.velocity += Geom::normalize(dist) * (accel * delta);

				/*
				Engine::out() << "pos: " << pos << " pos2: " << position << std::endl;
				Engine::out() << "M1*m2: " << size << "*" << m << "; dist^2: " << dist*dist << " -- " << velocity << std::endl;
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
				if ( P.position.x > limits.x)
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
						P.velocity.y *= std::pow(forceloss, delta);
						P.avelocity *= std::pow(forceloss, delta);
					}
				}
				else if ( P.position.x < 0 )
				{
					if ( std::fabs(P.velocity.x) > min )
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
						P.velocity.x *= std::pow(forceloss, delta);
						P.avelocity *= std::pow(forceloss, delta);
					}

				}
				else if ( P.position.y < 0 )
				{
					if ( std::fabs(P.velocity.y) > min )
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

			void collideWarped(Particle& P, float delta, Geom::Vec2f limits)
			{
				P.position.x = P.position.x > 0 ? std::fmod(P.position.x, limits.x) : limits.x +P.position.x;
				P.position.y = P.position.y > 0 ? std::fmod(P.position.y, limits.y) : limits.y +P.position.y;
			}



		}

		namespace globalaffectors
		{
			void applyMouseAffector(Particle::Iterator start, Particle::Iterator end, float delta, std::function<void(Particle&,float,Geom::Vec2f)> A)
			{
				if ( sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					auto pos = sf::Mouse::getPosition( (sf::Window&)Engine::GetApp() );
					auto vpos = Engine::GetApp().mapPixelToCoords( pos, sbe::Screen::sRndr()->getLayer(1)->Cam->getView());
					//Engine::out() << "mouseinwinPos: " << pos.x << "/" << pos.y << " - vpos: " << vpos.x << "/" << vpos.y << std::endl;

                    for ( ; start != end; ++start )
                        A( *start, delta, {vpos.x,vpos.y});
				}
			}
		}

		namespace manipulators
		{
			void emitRandom(std::vector<Particle>& Particles, float, Geom::Vec2f Origin, float maxvelocity)
			{
				Particle re;
				re.position = Origin;
				re.rotation = factories::randomFloat( {0, 2*Geom::pi() });
				float  speed = factories::randomFloat( {0, maxvelocity});
				auto tmp = gfx::rot( {0, speed}, re.rotation );
				re.velocity = {tmp.x, tmp.y};
				re.avelocity = factories::randomFloat( {-maxvelocity, maxvelocity});
				re.color = factories::randomColor();
				re.size = factories::randomFloat({0.5, 1.5});

				Particles.push_back(re);
			}

			void emitRay(std::vector<Particle>& Particles, float, Geom::Vec2f Origin, float direction, float maxspread, float velocity)
			{
				Particle re;
				re.position = Origin;
				re.rotation = factories::randomFloat( {0, 2*Geom::pi() });
				float spread = factories::randomFloat( {-maxspread, maxspread} );
				auto tmp = gfx::rot( {0, velocity}, direction + spread );
				re.velocity = {tmp.x, tmp.y};
				re.avelocity = factories::randomFloat( {-velocity, velocity});
				re.color = factories::randomColor();
				re.size = factories::randomFloat({0.5, 1.5});

				Particles.push_back(re);
			}

			void destroyOld(std::vector<Particle>& Particles, float)
			{
				std::remove_if(Particles.begin(), Particles.end(), [](Particle& P){ return P.age>1; });
			}
		}

		namespace renderers {
			void renderPoints(std::vector<Particle>& Particles, sf::VertexArray& V)
			{
				V.setPrimitiveType(sf::PrimitiveType::Points);
				V.resize(Particles.size());

				int i = 0;
				for ( Particle& p : Particles)
				{
					V[i].color = p.color;
					V[i].position.x = p.position.x + 0.5;
					V[i].position.y = p.position.y + 0.5;
					++i;
				}
			}

			void renderQuads(std::vector<Particle>& Particles, sf::VertexArray& V, float size)
			{
				V.setPrimitiveType(sf::PrimitiveType::Quads);
				V.resize( 4*Particles.size() );
				int i = 0;
				for ( Particle& p : Particles )
				{
					gfx::UpdateQuad( V, { p.position.x - size/2, p.position.y - size/2, size, size }, p.color, i);
					gfx::rotateQuad( &(V[i]), p.rotation, {p.position.x, p.position.y} );
					i+=4;
				}

			}
		}

		namespace generators {
			void generateGrid( std::vector<Particle>& Particles, Geom::Vec2 Size, Geom::Vec2 Count )
			{
				Particles.clear();

				std::random_device rd;
				std::default_random_engine e1(rd());
				std::uniform_int_distribution<int> uniform_dist(0, 255);


				Geom::Vec2f steps { Size.x/(float)Count.x, Size.y/(float)Count.y };

				Geom::Pointf pos;
				for ( float x = 0; x < Count.x; ++x)
				{
					pos.x += steps.x;
					pos.y = 0;
					for ( float y = 0; y < Count.y; ++y)
					{
						pos.y += steps.y;
						Particle P( pos );
						P.size = 1.0f + (uniform_dist(e1)/255.0f)/10.0f;
						P.color = sf::Color( uniform_dist(e1), uniform_dist(e1), uniform_dist(e1) );
						Particles.push_back( P );
						//Engine::out() << "Creating particle at: " << x << "," << y << std::endl;
					}
				}
			}
		}


	}

}
