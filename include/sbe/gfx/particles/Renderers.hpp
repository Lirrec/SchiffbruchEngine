#ifndef RENDERERS_HPP
#define RENDERERS_HPP

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>

#include <sbe/geom/Point.hpp>
#include <sbe/gfx/particles/Particle.hpp>

#include <sbe/gfx/VertexUtils.hpp>

#include <vector>

namespace sbe
{
	namespace particles
	{
		namespace renderers {
			/// renders a particlesystem as points
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

			/// renders a particlesystem as Quads including rotation
			void renderQuads(std::vector<Particle>& Particles, sf::VertexArray& V, float size = 0.0f)
			{
				V.setPrimitiveType(sf::PrimitiveType::Quads);
				V.resize( 4*Particles.size() );
				int i = 0;

				if ( size != 0 )
				{
					for ( Particle& p : Particles )
					{
						gfx::UpdateQuad( V, { p.position.x - size/2, p.position.y - size/2, size, size }, p.color, i);
						gfx::rotateQuad( &(V[i]), p.rotation, {p.position.x, p.position.y} );
						i+=4;
					}
				}
				else
				{
					for ( Particle& p : Particles )
					{
						gfx::UpdateQuad( V, { p.position.x - p.size/2, p.position.y - p.size/2, p.size, p.size }, p.color, i);
						gfx::rotateQuad( &(V[i]), p.rotation, {p.position.x, p.position.y} );
						i+=4;
					}
				}
			}
		}
	} // namespace particles
} // namespace sbe

#endif // RENDERERS_HPP

