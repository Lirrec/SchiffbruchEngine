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

			/// renders a particlesystem as point
			inline void renderPoint(Particle& p, sf::Vertex* V)
			{
				V->color = p.color;
				V->position.x = p.position.x + 0.5;
				V->position.y = p.position.y + 0.5;
			}

			/// renders a particle as Quad including rotation
			inline void renderQuad(Particle& p, sf::Vertex* V, Geom::irect textureRect = {{0,0},{0,0}} )
			{
				gfx::SetQuadPos( V, { p.position.x - p.size/2, p.position.y - p.size/2, p.size, p.size });
				gfx::SetQuadColor( V, p.color );
				gfx::rotateQuad( V, p.rotation, {p.position.x, p.position.y} );
				gfx::SetQuadTex( V, textureRect);
			}

			/// renders a particle as Quad including rotation
			inline void renderFixedSizeQuad(Particle& p, sf::Vertex* V, int size, Geom::irect textureRect = {{0,0},{0,0}} )
			{
				gfx::SetQuadPos( V, { p.position.x - size/2, p.position.y - size/2, size, size });
				gfx::SetQuadColor( V, p.color );
				gfx::rotateQuad( V, p.rotation, {p.position.x, p.position.y} );
				gfx::SetQuadTex( V, textureRect);
			}
		}
	} // namespace particles
} // namespace sbe

#endif // RENDERERS_HPP

