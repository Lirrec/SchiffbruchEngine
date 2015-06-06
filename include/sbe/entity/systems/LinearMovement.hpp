#ifndef LINEARMOVEMENT_HPP
#define LINEARMOVEMENT_HPP

#include "sbe/entity/System.hpp"
#include "sbe/geom/Point.hpp"

namespace sbe
{
	class Entity;

	namespace systems
	{

		class LinearMovement : public SystemBuilder<LinearMovement>
		{
		public:
			LinearMovement()
					: unitspersecond(10) { }

			LinearMovement(float ups)
					: unitspersecond(ups) { }

			void update(Entity& E, const sf::Time& delta) override;

		private:
			float unitspersecond;
		};

	} // namespace systems

} // namespace sbe

#endif // LINEARMOVEMENT_HPP
