#include "sbe/gfx/Camera.hpp"

#include "sbe/Config.hpp"
#include "sbe/Engine.hpp"
#include "sbe/Module.hpp"
#include "sbe/geom/Point.hpp"
#include "sbe/geom/SfmlHelpers.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <boost/format.hpp>
using boost::format;
using boost::io::group;

namespace sbe
{

	const sf::Vector2f Camera::nullVector = sf::Vector2f(0,0);

	Camera::Camera()
	 : Scrolling( false ),
	 ScrollEnabled( true )
	{
		ZoomFactor = Engine::getCfg()->get<float>("system.camera.zoomFactor");
		ScrollFactor = Engine::getCfg()->get<float>("system.camera.scrollFactor");
		delta = Engine::getCfg()->get<float>("system.camera.delta");
		WheelZoomFactor = Engine::getCfg()->get<float>("system.camera.wheelZoomFactor");

		CamLimits = sf::FloatRect( 0,0,10000,10000 );
		ZoomLimits = sf::FloatRect( 0,0,10000,10000 );
	}

	void Camera::printDebugInfo()
	{
	    Engine::out() << "[Camera] View Size " << str(format("%.0f x %.0f") % TargetSize.x % TargetSize.y) << std::endl;
	    Engine::out() << "[Camera] View Pos " << str(format("%.0f x %.0f") % TargetCenter.x % TargetCenter.y) << std::endl;
	    Engine::out() << "[Camera] View Limits " << str(format("%.0f/%.0f size: %.0f/%.0f") % CamLimits.left % CamLimits.top % CamLimits.width % CamLimits.height) << std::endl;
	    Engine::out() << "[Camera] Zoom Limits " << str(format("%.0f/%.0f size: %.0f/%.0f") % ZoomLimits.left % ZoomLimits.top % ZoomLimits.width % ZoomLimits.height ) << std::endl;
	}

	void Camera::showFrameInfo()
	{
		Module::Get()->DebugString("View Size",  str(format("%.0f x %.0f") % TargetSize.x % TargetSize.y));
		Module::Get()->DebugString("View Pos", str(format("%.0f x %.0f") % TargetCenter.x % TargetCenter.y));
		Module::Get()->DebugString("View Limits", str(format("%.0f/%.0f/%.0f/%.0f") % CamLimits.left % CamLimits.top % CamLimits.width % CamLimits.height));
		Module::Get()->DebugString("Zoom Limits", str(format("%.0f/%.0f/%.0f/%.0f") % ZoomLimits.left % ZoomLimits.top % ZoomLimits.width % ZoomLimits.height ));
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

	void Camera::setTargetSize ( sf::Vector2f size, bool dontsmooth)
	{
		if ( ZoomLimits.width != 0 && ZoomLimits.height != 0)
			if (geom::clip( size, ZoomLimits ) != size) return;

		TargetSize = size;

		if ( dontsmooth ) view.setSize( size );
	}

	void Camera::setTargetCenter ( sf::Vector2f c, bool dontsmooth)
	{
		if ( CamLimits.width != 0 && CamLimits.height != 0 )
			c = geom::clip( c, CamLimits );

		TargetCenter = c;

		if (dontsmooth) view.setCenter ( c );
	}

	void Camera::setZoomLimits( const sf::Vector2f& min, const sf::Vector2f& max)
	{
		ZoomLimits = geom::makeSfRectf(min, max);
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

			//Target = geom::clip( Target, ZoomLimits);
			view.setSize( Target );
		}

		if ( TargetCenter != CurrentCenter )
		{
			Target.x = CurrentCenter.x + (TargetCenter.x - CurrentCenter.x)*ZoomFactor;
			Target.y = CurrentCenter.y + (TargetCenter.y - CurrentCenter.y)*ZoomFactor;
			if ( std::abs(CurrentCenter.x - TargetCenter.x) < minDiff ) Target.x = TargetCenter.x;
			if ( std::abs(CurrentCenter.y - TargetCenter.y) < minDiff ) Target.y = TargetCenter.y;

			//Target = geom::clip( Target, CamLimits);
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

	void Camera::enableScrolling(bool enable)
	{
		ScrollEnabled = enable;
	}


	void Camera::HandleEvent( const sf::Event& e)
	{
	    if (!ScrollEnabled) return;

	    float curdelta = delta;

		switch (e.type)
		{
			case sf::Event::KeyPressed:

				if (e.key.shift) curdelta *= 10;

				switch (e.key.code)
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

				TargetCenter = geom::clip( TargetCenter, CamLimits);

				break;

			case sf::Event::MouseWheelMoved:
			{
				sf::Vector2f NewSize;
				for (int i = 0; i < std::abs(e.mouseWheel.delta); ++i)
				{
					NewSize = TargetSize * ( (e.mouseWheel.delta < 0) ? 1 + WheelZoomFactor : 1 - WheelZoomFactor );
				}

				// only set TargetSize if NewSize doesnt exceed any ZoomLimits
				if ( NewSize == geom::clip( NewSize, ZoomLimits))
					TargetSize = NewSize;

				break;
			}
			case sf::Event::MouseMoved:
			{
				sf::Vector2i mouseMove(e.mouseMove.x, e.mouseMove.y);

				sf::Vector2f glLastMousePos = Engine::GetApp().mapPixelToCoords( lastMousePos, view);
				sf::Vector2f glMouseMove = Engine::GetApp().mapPixelToCoords( mouseMove, view);

				if (Scrolling)
				{
					TargetCenter += (glLastMousePos - glMouseMove )*ScrollFactor;
					TargetCenter = geom::clip( TargetCenter, CamLimits);
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
} // namespace sbe
