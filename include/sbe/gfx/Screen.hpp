#ifndef SCREEN_H
#define SCREEN_H

#include "sbe/event/EventUser.hpp"

class SFMLEventUser;
class Renderer;
class Camera;

namespace sfg { class Desktop; class SFGUI; }
namespace sf { class Clock; }

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

		static Screen* get()
		{
			return Instance;
		}

		/**
			Add an object which should receive all sfml events in addition to the converter and sfg desktop.
			This allows Users to handle sfml events in their own classes.
		*/
		void addSFMLEventHandler( SFMLEventUser* U ) { sfEvtHandlers.push_back(U); }

		/**
			Remove a registered sf events listener.
		*/
		void removeSFMLEventHandler( SFMLEventUser* U );

		static std::shared_ptr<Camera> sCam() { return Instance->Cam; }
		static std::shared_ptr<Renderer> sRndr() { return Instance->Picasso; }
		static std::shared_ptr<sfg::Desktop> sDesk() { return Instance->Desktop; }
		static std::shared_ptr<SFMLEventConverter> sEvtConv() { return Instance->EvtConv; }

		std::shared_ptr<Camera> getCam() { return Cam; }
		std::shared_ptr<Renderer> getRenderer() { return Picasso; }
		std::shared_ptr<sfg::Desktop> getDesktop() { return Desktop; }
		std::shared_ptr<SFMLEventConverter> getEvtConv() { return EvtConv; }
	private:

		void Init();

		sf::Color bgColor;
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

#endif // SCREEN_H

