#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Window/Event.hpp>

/**
	This class proveds a small wrapper around sf::View to enable simple scrolling and zooming
	via mouse and keyboard.

	@todo make keys for moving and zooming customizable ( via Config )
*/
class Camera
{
	public:

		Camera();

		void Setup();
		void Update();

		void HandleEvent( const sf::Event& e);

		void setViewport ( const sf::FloatRect& FR) { view.setViewport( FR ); }
		void setTargetSize ( const sf::Vector2f& size ) { TargetSize = size; }
		void setTargetCenter ( const sf::Vector2f& c ) { TargetCenter = c; }

		sf::View& getView() { return view; }

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

