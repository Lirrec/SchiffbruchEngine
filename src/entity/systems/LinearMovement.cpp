#include "sbe/entity/systems/LinearMovement.hpp"

#include "sbe/entity/Entity.hpp"

#include <SFML/Graphics/Transformable.hpp>

#include "sbe/geom/PointHelpers.hpp"

#include "sbe/Engine.hpp"
#include <iostream>

namespace sbe
{
	template<>
	std::vector<std::string> SystemBuilder<systems::LinearMovement>::RequirementsPlain { "Transformable", "TargetPosition2D" };
	template<>
	std::string SystemBuilder<systems::LinearMovement>::Name = "LinearMovement";


	namespace systems
	{

		void LinearMovement::update(Entity& E, const sf::Time& delta)
		{
			sf::Transformable& T = E.C<sf::Transformable&>("Transformable");
			Geom::Point& TargetPosition = E.C<Geom::Point&>("TargetPosition2D");
			Geom::Pointf Pos = {T.getPosition().x, T.getPosition().y};

            Geom::Pointf Dist = Geom::PIToF(TargetPosition) - Pos;
			if ( Geom::length(Dist) < (unitspersecond * delta.asSeconds()) )
			{
				T.setPosition( TargetPosition.x, TargetPosition.y );
				E.removeSystem(getID());
				return;
			}


			Geom::Pointf newpos = Pos + Geom::normalize(Dist) * delta.asSeconds() * unitspersecond;

			//Engine::out() << "oldpos : " << Pos << " - new: " << newpos << std::endl;

			T.setPosition( newpos.x, newpos.y );


		}
	} // namespace systems

} // namespace sbe
