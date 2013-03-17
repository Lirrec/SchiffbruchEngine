#ifndef SCREEN_H
#define SCREEN_H

#include "sbe/event/EventUser.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sfg { class Desktop; class SFGUI; }
namespace sf { class Clock; }

namespace sbe
{
	class SFMLEventUser;
	class Renderer;
	class Camera;



	/**
		Main class to manage the Renderwindow, Renderer and SFGUI Desktop.
		It creates and provides access to:
			- the SFGUI Desktop for all UI elements
			- the Renderer for drawing your gameview
			- the SFMLEventConverter to easily convert keypresses to Event s

		By default the following Events from SFML are converted:
			sf::Event::Closed to EVT_QUIT ( global )
			sf::Event::Resized to WINDOW_RESIZE ( lokal )
			sf::Event::MouseButtonReleased to WINDOW_RESIZE ( lokal )

		Config values used:
			system.renderer.windowsize

	*/
	class Screen : public EventUser, public sf::NonCopyable
	{
		public:
			Screen();
			virtual ~Screen() {};

			/**
				Render the Desktop and Renderer.
				No need to call this manually, called from the EVT_FRAME handler
			*/
			void Render();


			/**
				Handles the following Events:
				 - EVT_FRAME: draws and updates the desktop
				 - EVT_QUIT: stopps the renderthread
				 - SCREEN_ADD_WINDOW (Data: sfg::Window::Ptr): Adds a new Window to the Desktop, use this to add your own Widgets
				 - TOGGLE_FULLSCREEN: recreate the RenderWindow in fullscreen Mode
				 - WINDOW_RESIZE: adapts the Camera to the new window size
			*/
			void HandleEvent(Event& e);

			/// used to determine wether an event should be sent to the renderer/
			bool desktopHandledEvent;
			/// add this as a handler on all your custom SFGUI Bindings, otherwise the clicks on items will also be sent to the renderer
			void OnHandledEvent() { desktopHandledEvent = true; }

			/// static getter to access the singleton screen instance
			static Screen* get()
			{
				return Instance;
			}

			/**
				Add an object which should receive all sfml events in addition to the converter and sfg desktop.
				This allows Users to handle sfml events in their own classes.
				@param U the new SFMLEventUser
			*/
			void addSFMLEventHandler( SFMLEventUser* U ) { sfEvtHandlers.push_back(U); }

			/**
				Remove a registered sf events listener.
				@param U the listener to remove
			*/
			void removeSFMLEventHandler( SFMLEventUser* U );

			/// enable or disable clearing the screen every Frame
			void setClear( bool c = true ) { clear = c; };
			/// set the color which is used to clear the screen
			/// default is sf::Color(180,180,180)
			void setClearColor ( sf::Color& c ) { bgColor = c; }
			/// check clear status
			bool isCleared() { return clear; }
			/// get current clear color
			sf::Color getClearColor() { return bgColor; }

			/// static access to the Camera
			static std::shared_ptr<Camera> sCam() { return Instance->Cam; }
			/// static access to the Renderer
			static std::shared_ptr<Renderer> sRndr() { return Instance->Picasso; }
			/// static access to the sfgui Desktop
			static std::shared_ptr<sfg::Desktop> sDesk() { return Instance->Desktop; }
			/// static Access to the SFMLEventConverter
			static std::shared_ptr<SFMLEventConverter> sEvtConv() { return Instance->EvtConv; }

			/// access to the Camera
			std::shared_ptr<Camera> getCam() { return Cam; }
			/// access to the Renderer
			std::shared_ptr<Renderer> getRenderer() { return Picasso; }
			/// access to the Desktop
			std::shared_ptr<sfg::Desktop> getDesktop() { return Desktop; }
			/// access to the SFMLEventConverter
			std::shared_ptr<SFMLEventConverter> getEvtConv() { return EvtConv; }
		private:

			void Init();

			sf::Color bgColor;
			bool clear;
			bool Fullscreen;

			std::shared_ptr<SFMLEventConverter> EvtConv;
			std::vector<SFMLEventUser*> sfEvtHandlers;

			std::shared_ptr<sfg::SFGUI> SFG;
			std::shared_ptr<sfg::Desktop> Desktop;
			sf::Clock guiclock;

			std::shared_ptr<Renderer> Picasso;
			std::shared_ptr<Camera> Cam;

			static Screen* Instance;
	};
} // namespace sbe
#endif // SCREEN_H

