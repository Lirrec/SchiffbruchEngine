#ifndef VERTEXUTIL_HPP
#define VERTEXUTIL_HPP

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <sbe/geom/Rect.hpp>

namespace sbe
{
	/**
		Namespace for helper functions for dealing with Vertices and VertexArrays
	*/
	namespace gfx
	{

		/**
			Set the position of a quad.
			@param coords a sf::FloatRect containing the new texture coordinates
		*/
		inline void SetQuadPos(sf::Vertex* vs, const sf::FloatRect& Pos) {
			// Order is important here ( vertices are added counterclockwise in opengl )
			vs[0].position = sf::Vector2f(Pos.left, Pos.top);                        // top-left
			vs[1].position = sf::Vector2f(Pos.left, Pos.top + Pos.height);                // bottom-left
			vs[2].position = sf::Vector2f(Pos.left + Pos.width, Pos.top + Pos.height);    // bottom-right
			vs[3].position = sf::Vector2f(Pos.left + Pos.width, Pos.top);                // top-right

			//	Engine::out() << "vs[0] " << Pos.left << " - " << Pos.top << std::endl;
			//	Engine::out() << "vs[1] " << Pos.left+Pos.width << " - " << Pos.top << std::endl;
			//	Engine::out() << "vs[2] " << Pos.left+Pos.width << " - " <<  Pos.top+Pos.height << std::endl;
			//	Engine::out() << "vs[3] " << Pos.left << " - " <<  Pos.top+Pos.height << std::endl;
		}

		/**
			Set the texture coordinates of a quad.
			@param coords a geom::irect containing the new texture coordinates
		*/
		inline void SetQuadTex(sf::Vertex* vs, const geom::irect& coords) {
			vs[0].texCoords = sf::Vector2f(coords.x.x, coords.x.y);    // top-left
			vs[1].texCoords = sf::Vector2f(coords.x.x, coords.y.y); // bottom-left
			vs[2].texCoords = sf::Vector2f(coords.y.x, coords.y.y); // bottom-right
			vs[3].texCoords = sf::Vector2f(coords.y.x, coords.x.y); // top-right

			//	Engine::out() << "vs[0]tex " << coords.x.x << " - " << coords.x.y << std::endl;
			//	Engine::out() << "vs[1]tex " << coords.y.x << " - " << coords.x.y << std::endl;
			//	Engine::out() << "vs[2]tex " << coords.y.x << " - " <<  coords.y.y << std::endl;
			//	Engine::out() << "vs[3]tex " << coords.x.x << " - " <<  coords.y.y << std::endl;
		}

		/**
			Set the texture coordinates of a quad.
			@param coords a sf::FloatRect containing the new texture coordinates
		*/
		inline void SetQuadTex(sf::Vertex* vs, const sf::FloatRect& coords) {
			vs[0].texCoords = sf::Vector2f(coords.left, coords.top);    // top-left
			vs[1].texCoords = sf::Vector2f(coords.left, coords.top + coords.height); // bottom-left
			vs[2].texCoords = sf::Vector2f(coords.left + coords.width, coords.top + coords.height); // bottom-right
			vs[3].texCoords = sf::Vector2f(coords.left + coords.width, coords.top); // top-right

			//	Engine::out() << "vs[0]tex " << coords.x.x << " - " << coords.x.y << std::endl;
			//	Engine::out() << "vs[1]tex " << coords.y.x << " - " << coords.x.y << std::endl;
			//	Engine::out() << "vs[2]tex " << coords.y.x << " - " <<  coords.y.y << std::endl;
			//	Engine::out() << "vs[3]tex " << coords.x.x << " - " <<  coords.y.y << std::endl;
		}

		/**
			Set the Color off all Vertices of a Quad.
		*/
		inline void SetQuadColor(sf::Vertex* vs, const sf::Color& c) {
			vs[0].color = c; // top-left
			vs[1].color = c; // bottom-left
			vs[2].color = c; // bottom-right
			vs[3].color = c; // top-right
		}

		inline sf::Vector2f rot(const sf::Vector2f in, const float r, const sf::Vector2f center = {0.f, 0.f}) {
			auto cos = std::cos(r);
			auto sin = std::sin(r);
			auto x = in.x - center.x;
			auto y = in.y - center.y;
			return sf::Vector2f(x*cos + y*sin, -x*sin + y*cos) + center;
		}

		/**
			Set the position of a quad.
			@param coords a sf::FloatRect containing the new texture coordinates
		*/
		inline void rotateQuad(sf::Vertex* vs, const float radians, const sf::Vector2f center = {0.f, 0.f}) {
			// Order is important here ( vertices are added counterclockwise in opengl )
			vs[0].position = rot(vs[0].position, radians, center);
			vs[1].position = rot(vs[1].position, radians, center);
			vs[2].position = rot(vs[2].position, radians, center);
			vs[3].position = rot(vs[3].position, radians, center);

			//	Engine::out() << "vs[0] " << Pos.left << " - " << Pos.top << std::endl;
			//	Engine::out() << "vs[1] " << Pos.left+Pos.width << " - " << Pos.top << std::endl;
			//	Engine::out() << "vs[2] " << Pos.left+Pos.width << " - " <<  Pos.top+Pos.height << std::endl;
			//	Engine::out() << "vs[3] " << Pos.left << " - " <<  Pos.top+Pos.height << std::endl;
		}

		/**
			Append a quad to a vertexarray. This version takes Texturecoordinates
		*/
		inline void AppendQuad(sf::VertexArray& vA, const sf::FloatRect& Pos, geom::irect coords) {
			sf::Vertex vs[4];
			SetQuadPos(vs, Pos);
			SetQuadTex(vs, coords);

			vA.append(vs[0]);
			vA.append(vs[1]);
			vA.append(vs[2]);
			vA.append(vs[3]);
		}

		/**
			Append a quad to a vertexarray. This version takes Texturecoordinates
		*/
		inline void AppendQuad(sf::VertexArray& vA, const sf::FloatRect& Pos, geom::irect coords, sf::Color _color) {
			sf::Vertex vs[4];
			SetQuadPos(vs, Pos);
			SetQuadTex(vs, coords);
			SetQuadColor(vs, _color);

			vA.append(vs[0]);
			vA.append(vs[1]);
			vA.append(vs[2]);
			vA.append(vs[3]);
		}

		/**
			Append a quad to a vertexarray. This version takes Vertexcolors
		*/
		inline void AppendQuad(sf::VertexArray& vA, const sf::FloatRect& Pos, sf::Color c) {
			sf::Vertex vs[4];
			SetQuadPos(vs, Pos);
			SetQuadColor(vs, c);

			vA.append(vs[0]);
			vA.append(vs[1]);
			vA.append(vs[2]);
			vA.append(vs[3]);
		}

		/**
			Update a quad in a vertexarray ( 4 vertices ). This version takes Texturecoordinates
		*/
		inline void UpdateQuad(sf::VertexArray& vA, const sf::FloatRect& Pos, const geom::irect& coords, const int ArrayIndex) {
			SetQuadPos(&vA[ArrayIndex], Pos);
			SetQuadTex(&vA[ArrayIndex], coords);
		}

		/**
			Update a quad in a vertexarray ( 4 vertices ). This version takes Texturecoordinates
		*/
		inline void UpdateQuad(sf::VertexArray& vA, const sf::FloatRect& Pos, const geom::irect& coords, const int ArrayIndex, sf::Color _color) {
			SetQuadPos(&vA[ArrayIndex], Pos);
			SetQuadTex(&vA[ArrayIndex], coords);
			SetQuadColor(&vA[ArrayIndex], _color);
		}

		/**
			Update a quad in a vertexarray ( 4 vertices ). This version takes Vertexcolors
		*/
		inline void UpdateQuad(sf::VertexArray& vA, const sf::FloatRect& Pos, const sf::Color& c, const int ArrayIndex) {
			SetQuadPos(&vA[ArrayIndex], Pos);
			SetQuadColor(&vA[ArrayIndex], c);
		}

		/**
			Update a line in a vertexarray ( 2 vertices).
		*/
		inline void UpdateLine(sf::VertexArray& vA, const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& c, const int ArrayIndex) {
			vA[ArrayIndex] = sf::Vertex(start, c);
			vA[ArrayIndex + 1] = sf::Vertex(end, c);
		}

		/**
			Append a line to a vertexarray ( 2 vertices).
		*/
		inline void AppendLine(sf::VertexArray& vA, const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& c) {
			vA.append(sf::Vertex(start, c));
			vA.append(sf::Vertex(end, c));
		}

	}
} // namespace sbe
#endif // VERTEXUTIL_HPP

