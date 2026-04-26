#ifndef SCREEN_H
#define SCREEN_H


#include <sbe/event/EventUser.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cassert>
#include <functional>
#include <vector>

namespace sf
{
	class Clock;
}

namespace sbe
{
	class SFMLEventUser;

	class Renderer;

	class Camera;

	class ImGuiWidget;


	/**
		Main class to manage the Renderwindow, Renderer and ImGui context.
		It creates and provides access to:
			- the ImGui widget registry for all UI elements
			- the Renderer for drawing your gameview
			- the SFMLEventConverter to easily convert keypresses to Event s

		By default the following Events from SFML are converted:
			sf::Event::Closed to EVT_QUIT ( global )
			sf::Event::Resized to WINDOW_RESIZE ( local )
			sf::Event::MouseButtonReleased to WINDOW_RESIZE ( local )

		Config values used:
			system.renderer.windowsize

	*/
	class Screen : public EventUser
	{
	public:
		Screen(const Screen&) = delete;
		Screen& operator=(const Screen&) = delete;
		Screen();

		virtual ~Screen();

		/**
			Render all ImGui widgets and the Renderer.
			No need to call this manually, called from the EVT_FRAME handler
		*/
		void Render();

		/// static getter to access the singleton screen instance
		static Screen* get();

		/**
			Add an object which should receive all sfml events in addition to the converter.
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

		/// static Access to the SFMLEventConverter
		static std::shared_ptr<SFMLEventConverter> sEvtConv();

		/// access to the Camera
		std::shared_ptr<Camera> getCam() { assert(Cam); return Cam; }

		/// access to the Renderer
		std::shared_ptr<Renderer> getRenderer() { assert(Picasso); return Picasso; }

		/// access to the SFMLEventConverter
		std::shared_ptr<SFMLEventConverter> getEvtConv() { assert(EvtConv); return EvtConv; }

		/**
			Register an ImGuiWidget to be rendered each frame.
			The widget's renderImGui() is called between ImGui::SFML::Update and ImGui::SFML::Render.
			Widgets should call this in their constructor.
		*/
		void registerImGuiWidget(ImGuiWidget* w);

		/**
			Unregister a previously registered ImGuiWidget.
			Widgets should call this in their destructor.
		*/
		void unregisterImGuiWidget(ImGuiWidget* w);

	protected:
		/**
			Handles the following Events:
			 - EVT_FRAME: draws and updates ImGui + game view
			 - EVT_QUIT: stops the renderthread
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

		std::vector<ImGuiWidget*> imguiWidgets;
		sf::Clock imguiClock;

		std::shared_ptr<Renderer> Picasso;
		std::shared_ptr<Camera> Cam;

		static Screen* Instance;
	};
} // namespace sbe
#endif // SCREEN_H
