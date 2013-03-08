#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Window/Event.hpp>

/**
	A Wrapper class around sf::View to enable simple scrolling and zooming
	via mouse and keyboard.

	@todo make keys for moving and zooming customizable ( via Config )
*/
class Camera
{
	public:

		/** Default Constructor.
			Uses config values:
				Multiplier for zooming					system.camera.zoomFactor
				Multiplier for Mouse scrolling: 		system.camera.scrollFactor
				Amount of speedup when shift is held:	system.camera.delta
				Multiplier for zooming with the wheel:	system.camera.wheelZoomFactor
		*/
		Camera();

		/**
			Sets a few default values from the config:
				Size: 	system.renderer.windowsize.x")
				Center: system.renderer.windowsize/2

		*/
		void setup();

		/**
			Update the Camera ( continue smooth scrolling/zooming until target size/center is reached).
			Should be called every frame
		*/
		void update();

		/**
			Handles SFML events for zooming and Scrolling
		*/
		void HandleEvent( const sf::Event& e);

		void setViewport ( const sf::FloatRect& FR) { view.setViewport( FR ); }
		void setTargetSize ( const sf::Vector2f& size ) { TargetSize = size; }
		void setTargetCenter ( const sf::Vector2f& c ) { TargetCenter = c; }

		sf::View& getView() { return view; }

		/**
			Sends DEBUG_STRING Events with Camrect and Center.
		*/
		void showDebugInfo();

	private:

		sf::View view;

		// the desired camera size
		sf::Vector2f TargetSize;
		// the desired camera center
		sf::Vector2f TargetCenter;

		// how fast should we zoom
		float ZoomFactor;

		bool Scrolling;
		sf::Vector2i lastMousePos;

		float ScrollFactor;
		int delta;
		float WheelZoomFactor;

};

#endif // CAMERA_HPP

