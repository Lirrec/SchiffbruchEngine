#ifndef SCREEN_H
#define SCREEN_H


#include <sbe/event/EventUser.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cassert>

namespace sfg
{
	class Desktop;

	class SFGUI;
}
namespace sf
{
	class Clock;
}

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
			sf::Event::Resized to WINDOW_RESIZE ( local )
			sf::Event::MouseButtonReleased to WINDOW_RESIZE ( local )

		Config values used:
			system.renderer.windowsize

	*/
	class Screen : public EventUser, public sf::NonCopyable
	{
	public:
		Screen();

		virtual ~Screen() { };

		/**
			Render the Desktop and Renderer.
			No need to call this manually, called from the EVT_FRAME handler
		*/
		void Render();

		/// used to determine wether an event should be sent to the renderer/
		bool desktopHandledEvent;

		/// add this as a handler on all your custom SFGUI Bindings, otherwise the clicks on items will also be sent to the renderer
		void OnHandledEvent() { desktopHandledEvent = true; }

		/// static getter to access the singleton screen instance
		static Screen* get();

		/**
			Add an object which should receive all sfml events in addition to the converter and sfg desktop.
			This allows Users to handle sfml events in their own classes.
			@param U the new SFMLEventUser
		*/
		void addSFMLEventHandler(SFMLEventUser* U) { sfEvtHandlers.push_back(U); }

		/**
			Remove a registered sf events listener.
			@param U the listener to remove
		*/
		void removeSFMLEventHandler(SFMLEventUser* U);

		/// enable or disable clearing the screen every Frame
		void setClear(bool c = true) { clear = c; };

		/// set the color which is used to clear the screen
		/// default is sf::Color(180,180,180)
		void setClearColor(const sf::Color& c) { bgColor = c; }

		/// check clear status
		bool isCleared() { return clear; }

		/// get current clear color
		sf::Color getClearColor() { return bgColor; }

		/// static access to the Camera
		static std::shared_ptr<Camera> sCam();

		/// static access to the Renderer
		static std::shared_ptr<Renderer> sRndr();

		/// static access to the sfgui Desktop
		static std::shared_ptr<sfg::Desktop> sDesk();

		/// static Access to the SFMLEventConverter
		static std::shared_ptr<SFMLEventConverter> sEvtConv();

		/// access to the Camera
		std::shared_ptr<Camera> getCam() { assert(Cam); return Cam; }

		/// access to the Renderer
		std::shared_ptr<Renderer> getRenderer() { assert(Picasso); return Picasso; }

		/// access to the Desktop
		std::shared_ptr<sfg::Desktop> getDesktop() { assert(Desktop); return Desktop; }

		/// access to the SFMLEventConverter
		std::shared_ptr<SFMLEventConverter> getEvtConv() { assert(EvtConv); return EvtConv; }

	protected:
		/**
			Handles the following Events:
			 - EVT_FRAME: draws and updates the desktop
			 - EVT_QUIT: stopps the renderthread
			 - SCREEN_ADD_WINDOW (Data: sfg::Window::Ptr): Adds a new Window to the Desktop, use this to add your own Widgets
			 - SCREEN_REMOVE_WINDOW (Data: sfg::Window::Ptr): Removes a Window from the Desktop
			 - TOGGLE_FULLSCREEN: recreate the RenderWindow in fullscreen Mode
			 - WINDOW_RESIZE: adapts the Camera to the new window size
		*/
		void HandleEvent(Event& e) override;


	private:

		void Init();

		sf::Color bgColor;
		bool Fullscreen;
		bool clear;

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

