#include "sbe/gfx/Camera.hpp"

#include "sbe/Config.hpp"
#include "sbe/Engine.hpp"
#include "sbe/Module.hpp"
#include "sbe/geom/Point.hpp"

#include <boost/format.hpp>
using boost::format;
using boost::io::group;

Camera::Camera()
 : Scrolling( false )
{
	ZoomFactor = Engine::getCfg()->get<float>("system.camera.zoomFactor");
	ScrollFactor = Engine::getCfg()->get<float>("system.camera.scrollFactor");
	delta = Engine::getCfg()->get<float>("system.camera.delta");
	WheelZoomFactor = Engine::getCfg()->get<float>("system.camera.wheelZoomFactor");
}

void Camera::showDebugInfo()
{
	Module::Get()->DebugString("View Size",  str(format("%.0f x %.0f") % TargetSize.x % TargetSize.y));
	Module::Get()->DebugString("View Pos", str(format("%.0f x %.0f") % TargetCenter.x % TargetCenter.y));
}

void Camera::setup()
{
	view.setSize(Engine::getCfg()->get<int>("system.renderer.windowsize.x"),
					Engine::getCfg()->get<int>("system.renderer.windowsize.y"));

	view.setCenter(Engine::getCfg()->get<int>("system.renderer.windowsize.x")/2,
					Engine::getCfg()->get<int>("system.renderer.windowsize.y")/2);

	TargetSize = sf::Vector2f(Engine::getCfg()->get<int>("system.renderer.windowsize.x"),
							Engine::getCfg()->get<int>("system.renderer.windowsize.y"));

	TargetCenter = sf::Vector2f(Engine::getCfg()->get<int>("system.renderer.windowsize.x")/2,
								Engine::getCfg()->get<int>("system.renderer.windowsize.y")/2);
}

void Camera::setTargetSize ( const sf::Vector2f& size, bool dontsmooth)
{
	 TargetSize = size;
	 if ( dontsmooth ) view.setSize( size );
}

void Camera::setTargetCenter ( const sf::Vector2f& c, bool dontsmooth)
{
	TargetCenter = c;
	if (dontsmooth) view.setCenter ( c );
}

void Camera::update()
{
	// minimum difference between Target and current position to be smoothed
	const int minDiff = 2;
	sf::Vector2f CurrentSize = view.getSize();
	sf::Vector2f CurrentCenter = view.getCenter();

	sf::Vector2f Target;

	if ( TargetSize != CurrentSize )
	{
		//Engine::out() << "Size: " << CurrentSize.x << "/" << CurrentSize.y << std::endl;
		//Engine::out() << "Target: " << TargetSize.x << "/" << TargetSize.y << std::endl;

		Target.x = CurrentSize.x + (TargetSize.x - CurrentSize.x)*ZoomFactor;
		Target.y = CurrentSize.y + (TargetSize.y - CurrentSize.y)*ZoomFactor;
		if ( std::abs(CurrentSize.x - TargetSize.x) < minDiff ) Target.x = TargetSize.x;
		if ( std::abs(CurrentSize.y - TargetSize.y) < minDiff ) Target.y = TargetSize.y;

		view.setSize( Target );
	}

	if ( TargetCenter != CurrentCenter )
	{
		Target.x = CurrentCenter.x + (TargetCenter.x - CurrentCenter.x)*ZoomFactor;
		Target.y = CurrentCenter.y + (TargetCenter.y - CurrentCenter.y)*ZoomFactor;
		if ( std::abs(CurrentCenter.x - TargetCenter.x) < minDiff ) Target.x = TargetCenter.x;
		if ( std::abs(CurrentCenter.y - TargetCenter.y) < minDiff ) Target.y = TargetCenter.y;

		view.setCenter( Target );
	}

	CalcDrawArea();
}

void Camera::CalcDrawArea()
{
	DrawnRectangle.left = view.getCenter().x - view.getSize().x/2;
	DrawnRectangle.top = view.getCenter().y - view.getSize().y/2;
	DrawnRectangle.width = view.getSize().x;
	DrawnRectangle.height = view.getSize().y;
}

void Camera::HandleEvent( const sf::Event& e)
{
	switch (e.type)
	{
		case sf::Event::KeyPressed:

			if (e.key.shift) delta *= 10;

			switch (e.key.code)
			{
				case sf::Keyboard::Key::Up:
					TargetCenter.y += -delta;
					break;

				case sf::Keyboard::Key::Down:
					TargetCenter.y += delta;
					break;

				case sf::Keyboard::Key::Left:
					TargetCenter.x += -delta;
					break;

				case sf::Keyboard::Key::Right:
					TargetCenter.x += delta;
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
			break;

		case sf::Event::MouseWheelMoved:


			for (int i = 0; i < std::abs(e.mouseWheel.delta); ++i)
			{
				TargetSize *= (e.mouseWheel.delta < 0) ? 1 + WheelZoomFactor : 1 - WheelZoomFactor;
			}
			break;

		case sf::Event::MouseMoved:
		{
			sf::Vector2i mouseMove(e.mouseMove.x, e.mouseMove.y);

			sf::Vector2f glLastMousePos = Engine::GetApp().mapPixelToCoords( lastMousePos, view);
			sf::Vector2f glMouseMove = Engine::GetApp().mapPixelToCoords( mouseMove, view);

			if (Scrolling)
			{
				TargetCenter += (glLastMousePos - glMouseMove )*ScrollFactor;
			}

			lastMousePos.x = e.mouseMove.x;
			lastMousePos.y = e.mouseMove.y;
		}
			break;

		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Middle) Scrolling = true;
			break;

		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button == sf::Mouse::Middle) Scrolling = false;
			break;
	}
}
