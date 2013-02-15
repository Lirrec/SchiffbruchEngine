#ifndef VERTEXUTIL_HPP
#define VERTEXUTIL_HPP



#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace gfx
{

	inline void SetQuadPos( sf::Vertex* vs, const sf::FloatRect& Pos )
	{
		// Order is important here ( vertices are added counterclockwise in opengl )
		vs[0].position = sf::Vector2f( Pos.left, Pos.top  );						// top-left
		vs[1].position = sf::Vector2f( Pos.left, Pos.top+Pos.height );			  	// bottom-left
		vs[2].position = sf::Vector2f( Pos.left+Pos.width, Pos.top+Pos.height ); 	// bottom-right
		vs[3].position = sf::Vector2f( Pos.left+Pos.width, Pos.top ); 				// top-right

		//	Engine::out() << "vs[0] " << Pos.left << " - " << Pos.top << std::endl;
		//	Engine::out() << "vs[1] " << Pos.left+Pos.width << " - " << Pos.top << std::endl;
		//	Engine::out() << "vs[2] " << Pos.left+Pos.width << " - " <<  Pos.top+Pos.height << std::endl;
		//	Engine::out() << "vs[3] " << Pos.left << " - " <<  Pos.top+Pos.height << std::endl;
	}

	inline void SetQuadTex( sf::Vertex* vs, const Geom::Rect& coords )
	{
		vs[0].texCoords = sf::Vector2f( coords.x.x, coords.x.y );	// top-left
		vs[1].texCoords = sf::Vector2f( coords.x.x, coords.y.y ); // bottom-left
		vs[2].texCoords = sf::Vector2f( coords.y.x, coords.y.y ); // bottom-right
		vs[3].texCoords = sf::Vector2f( coords.y.x, coords.x.y ); // top-right

		//	Engine::out() << "vs[0]tex " << coords.x.x << " - " << coords.x.y << std::endl;
		//	Engine::out() << "vs[1]tex " << coords.y.x << " - " << coords.x.y << std::endl;
		//	Engine::out() << "vs[2]tex " << coords.y.x << " - " <<  coords.y.y << std::endl;
		//	Engine::out() << "vs[3]tex " << coords.x.x << " - " <<  coords.y.y << std::endl;
	}

	inline void SetQuadColor( sf::Vertex* vs, const sf::Color& c )
	{
		vs[0].color = c; // top-left
		vs[1].color = c; // bottom-left
		vs[2].color = c; // bottom-right
		vs[3].color = c; // top-right
	}

	/**
		Append a quad to a vertexarray. This version takes Texturecoordinates
	*/
	void AppendQuad( sf::VertexArray& vA, const sf::FloatRect& Pos, const Geom::Rect& coords )
	{
		sf::Vertex vs[4];
		SetQuadPos( vs, Pos );
		SetQuadTex( vs, coords );

		vA.append( vs[0] );
		vA.append( vs[1] );
		vA.append( vs[2] );
		vA.append( vs[3] );
	}

	/**
		Append a quad to a vertexarray. This version takes Vertexcolors
	*/
	void AppendQuad( sf::VertexArray& vA, const sf::FloatRect& Pos, const sf::Color& c )
	{
		sf::Vertex vs[4];
		SetQuadPos( vs, Pos );
		SetQuadColor( vs, c );

		vA.append( vs[0] );
		vA.append( vs[1] );
		vA.append( vs[2] );
		vA.append( vs[3] );
	}

	/**
		Update a quad in a vertexarray ( 4 vertices ). This version takes Texturecoordinates
	*/
	void UpdateQuad( sf::VertexArray& vA, const sf::FloatRect& Pos, const Geom::Rect& coords, const int ArrayIndex)
	{
		sf::Vertex vs[4];
		SetQuadPos( vs, Pos );
		SetQuadTex( vs, coords );

		vA[ ArrayIndex ] = vs[0];
		vA[ ArrayIndex + 1 ] = vs[1];
		vA[ ArrayIndex + 2 ] = vs[2];
		vA[ ArrayIndex + 3 ] = vs[3];
	}

	/**
		Update a quad in a vertexarray ( 4 vertices ). This version takes Vertexcolors
	*/
	void UpdateQuad( sf::VertexArray& vA, const sf::FloatRect& Pos, const sf::Color& c, const int ArrayIndex)
	{
		sf::Vertex vs[4];
		SetQuadPos( vs, Pos );
		SetQuadColor( vs, c );

		vA[ ArrayIndex ] = vs[0];
		vA[ ArrayIndex + 1 ] = vs[1];
		vA[ ArrayIndex + 2 ] = vs[2];
		vA[ ArrayIndex + 3 ] = vs[3];
	}

	/**
		Update a line in a vertexarray ( 2 vertices).
	*/
	void UpdateLine( sf::VertexArray& vA, const Geom::Point& start, const Geom::Point& end, const sf::Color& c,  const int ArrayIndex )
	{
		vA[ArrayIndex] = sf::Vertex( sf::Vector2f(start.x, start.y), c );
		vA[ArrayIndex + 1] = sf::Vertex( sf::Vector2f(end.x, end.y), c );
	}

	/**
		Append a line to a vertexarray ( 2 vertices).
	*/
	void AppendLine( sf::VertexArray& vA, const Geom::Point& start, const Geom::Point& end, const sf::Color& c )
	{
		vA.append( sf::Vertex( sf::Vector2f(start.x, start.y), c) );
		vA.append( sf::Vertex( sf::Vector2f(end.x, end.y), c) );
	}

}

#endif // VERTEXUTIL_HPP

