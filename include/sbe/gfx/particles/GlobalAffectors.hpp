#ifndef GLOBALAFFECTORS_HPP
#define GLOBALAFFECTORS_HPP

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <sbe/Engine.hpp>

#include <sbe/gfx/Screen.hpp>
#include <sbe/gfx/Renderer.hpp>
#include <sbe/gfx/Camera.hpp>

#include <sbe/geom/Point.hpp>
#include <sbe/gfx/particles/Particle.hpp>
#include <sbe/gfx/particles/ParticleSystem.hpp>

namespace sbe
{
	namespace particles
	{

		namespace globalaffectors
		{
			///	apply a affector if the left mouse button is clicked
			inline void applyMouseAffector(Particle::Iterator start, Particle::Iterator end, float delta, std::function<void(Particle&,float,glm::vec2)> A)
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


	} // namespace particles


} // namespace sbe

#endif // GLOBALAFFECTORS_HPP

