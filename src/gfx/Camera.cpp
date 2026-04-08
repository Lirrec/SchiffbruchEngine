#include <sbe/gfx/Camera.hpp>

#include <sbe/Config.hpp>
#include <sbe/Module.hpp>
#include <sbe/geom/Point.hpp>
#include <sbe/geom/SfmlHelpers.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <boost/format.hpp>

using boost::format;
using boost::io::group;

namespace sbe
{

	const sf::Vector2f Camera::nullVector = sf::Vector2f(0, 0);

	Camera::Camera()
			: Scrolling(false),
			  ScrollEnabled(true) {
		ZoomFactor = Engine::getCfg()->get<float>("system.camera.zoomFactor", 0.2);
		ScrollFactor = Engine::getCfg()->get<float>("system.camera.scrollFactor", 1.0);
		delta = Engine::getCfg()->get<float>("system.camera.delta", 10);
		WheelZoomFactor = Engine::getCfg()->get<float>("system.camera.wheelZoomFactor", 0.2);

		CamLimits = sf::FloatRect({0, 0}, {10000, 10000});
		ZoomLimits = sf::FloatRect({0, 0}, {10000, 10000});
	}

	void Camera::printDebugInfo() {
		Engine::out() << "[Camera] View Size " << str(format("%.0f x %.0f")%TargetSize.x%TargetSize.y) << std::endl;
		Engine::out() << "[Camera] View Pos " << str(format("%.0f x %.0f")%TargetCenter.x%TargetCenter.y) << std::endl;
		Engine::out() << "[Camera] View Limits " << str(format("%.0f/%.0f size: %.0f/%.0f")%CamLimits.position.x%CamLimits.position.y%CamLimits.size.x%CamLimits.size.y) << std::endl;
		Engine::out() << "[Camera] Zoom Limits " << str(format("%.0f/%.0f size: %.0f/%.0f")%ZoomLimits.position.x%ZoomLimits.position.y%ZoomLimits.size.x%ZoomLimits.size.y) << std::endl;
	}

	void Camera::showFrameInfo() {
		Module::Get()->DebugString("View Size", str(format("%.0f x %.0f")%TargetSize.x%TargetSize.y));
		Module::Get()->DebugString("View Pos", str(format("%.0f x %.0f")%TargetCenter.x%TargetCenter.y));
		Module::Get()->DebugString("View Limits", str(format("%.0f/%.0f/%.0f/%.0f")%CamLimits.position.x%CamLimits.position.y%CamLimits.size.x%CamLimits.size.y));
		Module::Get()->DebugString("Zoom Limits", str(format("%.0f/%.0f/%.0f/%.0f")%ZoomLimits.position.x%ZoomLimits.position.y%ZoomLimits.size.x%ZoomLimits.size.y));
	}

	void Camera::setup() {

		sf::Vector2f WindowSize {
			(float) Engine::getCfg()->get<int>("system.renderer.windowsize.x", 1024),
			(float) Engine::getCfg()->get<int>("system.renderer.windowsize.y", 768)
		};

		view.setSize(WindowSize);

		view.setCenter({WindowSize.x/2, WindowSize.y/2});

		TargetSize = WindowSize;

		TargetCenter = {WindowSize.x/2, WindowSize.y/2};
	}

	void Camera::setTargetSize(sf::Vector2f size, bool dontsmooth) {
		if (ZoomLimits.size.x != 0 && ZoomLimits.size.y != 0) if (geom::clip(size, ZoomLimits) != size) return;

		TargetSize = size;

		if (dontsmooth) view.setSize(size);
	}

	void Camera::setTargetCenter(sf::Vector2f c, bool dontsmooth) {
		if (CamLimits.size.x != 0 && CamLimits.size.y != 0)
			c = geom::clip(c, CamLimits);

		TargetCenter = c;

		if (dontsmooth) view.setCenter(c);
	}

	void Camera::setZoomLimits(const sf::Vector2f& min, const sf::Vector2f& max) {
		ZoomLimits = geom::makeSfRectf(min, max);
	}

	void Camera::update() {

		// minimum difference between Target and current position to be smoothed
		const float minDiff = 0.33;
		sf::Vector2f CurrentSize = view.getSize();
		sf::Vector2f CurrentCenter = view.getCenter();

		sf::Vector2f Target;

		if (TargetSize != CurrentSize)
		{
			Target.x = CurrentSize.x + (TargetSize.x - CurrentSize.x)*ZoomFactor;
			Target.y = CurrentSize.y + (TargetSize.y - CurrentSize.y)*ZoomFactor;
			if (std::abs(CurrentSize.x - TargetSize.x) < minDiff) Target.x = TargetSize.x;
			if (std::abs(CurrentSize.y - TargetSize.y) < minDiff) Target.y = TargetSize.y;

			view.setSize(Target);
		}

		if (TargetCenter != CurrentCenter)
		{
			Target.x = CurrentCenter.x + (TargetCenter.x - CurrentCenter.x)*ZoomFactor;
			Target.y = CurrentCenter.y + (TargetCenter.y - CurrentCenter.y)*ZoomFactor;
			if (std::abs(CurrentCenter.x - TargetCenter.x) < minDiff) Target.x = TargetCenter.x;
			if (std::abs(CurrentCenter.y - TargetCenter.y) < minDiff) Target.y = TargetCenter.y;

			view.setCenter(Target);
		}

		CalcDrawArea();
	}

	void Camera::CalcDrawArea() {
		DrawnRectangle.position.x = view.getCenter().x - view.getSize().x/2;
		DrawnRectangle.position.y = view.getCenter().y - view.getSize().y/2;
		DrawnRectangle.size.x = view.getSize().x;
		DrawnRectangle.size.y = view.getSize().y;
	}

	void Camera::enableScrolling(bool enable) {
		ScrollEnabled = enable;
	}


	void Camera::HandleEvent(const sf::Event& e) {
		if (!ScrollEnabled) return;

		float curdelta = delta;

		if (const auto* keyEvent = e.getIf<sf::Event::KeyPressed>())
		{
			if (keyEvent->shift) curdelta *= 10;

			switch (keyEvent->code)
			{
				case sf::Keyboard::Key::Up:
					TargetCenter.y += -curdelta;
					break;

				case sf::Keyboard::Key::Down:
					TargetCenter.y += curdelta;
					break;

				case sf::Keyboard::Key::Left:
					TargetCenter.x += -curdelta;
					break;

				case sf::Keyboard::Key::Right:
					TargetCenter.x += curdelta;
					break;

				case sf::Keyboard::Key::PageUp:
					TargetSize *= 1.1f;
					break;

				case sf::Keyboard::Key::PageDown:
					TargetSize *= 0.9f;
					break;

				default:
					break;
			}

			TargetCenter = geom::clip(TargetCenter, CamLimits);
		}
		else if (const auto* wheelEvent = e.getIf<sf::Event::MouseWheelScrolled>())
		{
			float d = wheelEvent->delta;
			sf::Vector2f NewSize = TargetSize * ((d < 0) ? 1 + WheelZoomFactor : 1 - WheelZoomFactor);

			// only set TargetSize if NewSize doesnt exceed any ZoomLimits
			if (NewSize == geom::clip(NewSize, ZoomLimits))
				TargetSize = NewSize;
		}
		else if (const auto* mouseMoveEvent = e.getIf<sf::Event::MouseMoved>())
		{
			sf::Vector2i mouseMove(mouseMoveEvent->position.x, mouseMoveEvent->position.y);

			sf::Vector2f glLastMousePos = Engine::GetApp().mapPixelToCoords(lastMousePos, view);
			sf::Vector2f glMouseMove = Engine::GetApp().mapPixelToCoords(mouseMove, view);

			if (Scrolling)
			{
				TargetCenter += (glLastMousePos - glMouseMove)*ScrollFactor;
				TargetCenter = geom::clip(TargetCenter, CamLimits);
			}

			lastMousePos = mouseMoveEvent->position;
		}
		else if (const auto* mousePressedEvent = e.getIf<sf::Event::MouseButtonPressed>())
		{
			if (mousePressedEvent->button == sf::Mouse::Button::Middle) Scrolling = true;
		}
		else if (const auto* mouseReleasedEvent = e.getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseReleasedEvent->button == sf::Mouse::Button::Middle) Scrolling = false;
		}
	}
} // namespace sbe
