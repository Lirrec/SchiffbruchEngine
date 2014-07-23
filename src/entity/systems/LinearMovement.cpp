#include "sbe/entity/systems/LinearMovement.hpp"

#include "sbe/entity/Entity.hpp"
#include "sbe/geom/PointHelpers.hpp"
#include "sbe/Engine.hpp"

#include <SFML/Graphics/Transformable.hpp>
#include <glm/glm.hpp>
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
			glm::ipoint2& TargetPosition = E.C<glm::ipoint2&>("TargetPosition2D");
			glm::point2 Pos = {T.getPosition().x, T.getPosition().y};

            glm::point2 Dist = geom::PIToF(TargetPosition) - Pos;
			if ( glm::length(Dist) < (unitspersecond * delta.asSeconds()) )
			{
				T.setPosition( TargetPosition.x, TargetPosition.y );
				E.removeSystem(getID());
				return;
			}


			glm::point2 newpos = Pos + glm::normalize(Dist) * delta.asSeconds() * unitspersecond;

			//Engine::out() << "oldpos : " << Pos << " - new: " << newpos << std::endl;

			T.setPosition( newpos.x, newpos.y );


		}
	} // namespace systems

} // namespace sbe
