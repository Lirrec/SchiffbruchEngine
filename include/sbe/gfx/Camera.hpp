#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Window/Event.hpp>

namespace sbe
{
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

			/**
				Set the current viewport of the Camera.
				From sfml doc:
				The viewport is the rectangle into which the contents of the view are displayed, expressed as a factor (between 0 and 1) of the size of the RenderTarget to which the view is applied.
				For example, a view which takes the left side of the target would be defined with View.setViewport(sf::FloatRect(0, 0, 0.5, 1)). By default, a view has a viewport which covers the entire target.

				@see sf::View::setViewport()
			*/
			void setViewport ( const sf::FloatRect& FR) { view.setViewport( FR ); }

			/**
				Set the target size of the rectangle which should be shown by the Camera.
				The camera will smooth the transition to the new value over several frames.
				@param size the new size
				@param dontsmooth set to true to disable the smoothing and set the new size instantly
			*/
			void setTargetSize ( sf::Vector2f size, bool dontsmooth = false  );

			/**
				Set the center of the rectangle which should be shown by the Camera.
				The camera will smooth the transition to the new value over several frames.
				@param c the new center
				@param dontsmooth set to true to disable the smoothing and set the new center instantly
			*/
			void setTargetCenter ( sf::Vector2f c, bool dontsmooth = false );

			/**
				Set minimum and maximum limits, which the viewport of the camera may contain
				@param min the minimum limits
				@param max the maximum limits
			*/
			void setZoomLimits( const sf::Vector2f& min, const sf::Vector2f& max);

			/**
				Set the rectangle which limits the camera movement
				@param f rectangle containing an area in which the camera may scroll
			*/
			void setCamLimits( const sf::FloatRect& f ) { CamLimits = f; }

			void zoom( float zoomFactor ) { TargetSize *= zoomFactor; }

			/// access the internal sf::View
			sf::View& getView() { return view; }

			sf::FloatRect& getZoomLimits() { return ZoomLimits; }
			sf::FloatRect& getCamLimits() { return CamLimits; }

			sf::Vector2f&  getTargetCenter() { return TargetCenter; }
			sf::Vector2f&  getTargetSize() { return TargetSize; }
			sf::FloatRect& getDrawnArea() { return DrawnRectangle; }

			/**
				Sends DEBUG_STRING Events with Camrect and Center.
			*/
			void showDebugInfo();

		private:

			static const sf::Vector2f nullVector;

			sf::View view;

			// LIMITS
			/// minimum/maximum size of the Area shown by the viewrect
			sf::FloatRect ZoomLimits;
			/// allowed Scroll Area used to limit the scrollable area
			sf::FloatRect CamLimits;


			/// the desired camera size
			sf::Vector2f TargetSize;
			/// the desired camera center
			sf::Vector2f TargetCenter;
			// the rendered rectangle in RENDER coordinates
			sf::FloatRect DrawnRectangle;

			void CalcDrawArea();

			/// how fast should we zoom
			float ZoomFactor;

			/// are we currently scrolling? tracks mousedown and up events
			bool Scrolling;
			sf::Vector2i lastMousePos;

			float ScrollFactor;
			int delta;
			float WheelZoomFactor;

	};
} // namespace sbe
#endif // CAMERA_HPP

