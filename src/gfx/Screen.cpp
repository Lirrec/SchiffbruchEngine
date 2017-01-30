#include <sbe/gfx/Screen.hpp>

#include <sbe/Config.hpp>

#include <sbe/event/SFMLEventUser.hpp>
#include <sbe/event/SFMLEvent.hpp>

#include <sbe/gfx/Renderer.hpp>
#include <sbe/gfx/Camera.hpp>

#include <sbe/Module.hpp>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Window.hpp>

namespace sbe
{
	Screen* Screen::Instance = nullptr;


	Screen::Screen()
			: Fullscreen(false),
			  clear(true) {
		Instance = this;

		EvtConv = std::make_shared<SFMLEventConverter>();
		RegisterForEvent("EVT_FRAME", [this](Event&) { Render(); });
		RegisterForEvent("EVT_QUIT", [this](Event&) { Module::Get()->RequestQuit(); });

		RegisterForEvent("WINDOW_RESIZE");
		RegisterForEvent("SCREEN_ADD_WINDOW");
		RegisterForEvent("SCREEN_REMOVE_WINDOW");
		RegisterForEvent("TOGGLE_FULLSCREEN");

		// load keybindings from default config
		int i = EvtConv->LoadKeyBindingsFromConfig();
		Engine::out(Engine::INFO) << "[Screen] Loaded " << i << " key bindings." << std::endl;

		EvtConv->AddEventConversion(sf::Event::Closed, "EVT_QUIT", true);
		EvtConv->AddEventConversion(sf::Event::Resized, "WINDOW_RESIZE");

		Init();


		Engine::out() << "[Screen] Initialized." << std::endl;
	}

	void Screen::Init() {
		Engine::out(Engine::INFO) << "[Screen] Creating Window..." << std::endl;

		bgColor = sf::Color(180, 180, 180);

		std::string Icon = "res/" + Engine::getCfg()->get<std::string>("system.renderer.icon", "");
		std::string Title = Engine::getCfg()->get<std::string>("system.renderer.title", "SchiffbruchEngine powered.");

		// create the renderwindow
		Engine::GetApp().create(
			sf::VideoMode(	Engine::getCfg()->get<unsigned int>("system.renderer.windowsize.x", 1024),
							Engine::getCfg()->get<unsigned int>("system.renderer.windowsize.y", 768)),
			Title);

		if (Icon != "")
		{
			Engine::out(Engine::INFO) << "[Screen] Loading Icon '" << Icon << "'." << std::endl;
			sf::Image IconImage;
			if (!IconImage.loadFromFile(Icon))
			{
				Engine::out(Engine::ERROR) << "[Screen] Error Loading Icon '" << Icon << "' ( not found/corrupted )." << std::endl;
			}
			else
			{
				if (IconImage.getSize().x <= 256 && IconImage.getSize().y <= 256)
					Engine::GetApp().setIcon(IconImage.getSize().x, IconImage.getSize().y, IconImage.getPixelsPtr());
				else
					Engine::out(Engine::ERROR) << "[Screen] Icon '" << Icon << "' is larger than 256x256!" << std::endl;
			}

		}


		// must be created before using SFGUI
		SFG = std::make_shared<sfg::SFGUI>();

		// top-level container for all SFGUI widgets
		Desktop = std::make_shared<sfg::Desktop>();
		Cam = std::make_shared<Camera>();
		Cam->setup();

		Picasso = std::make_shared<Renderer>();

		// We're not using SFML to render anything in this program, so reset OpenGL
		// states. Otherwise we wouldn't see anything.
		Engine::GetApp().resetGLStates();

		guiclock.restart();

		Module::Get()->QueueEvent("SCREEN_CREATED", true);
	}


	void Screen::Render() {
		// Process Hardware/SFML Events
		sf::Event sfEvent;


		while (Engine::GetApp().pollEvent(sfEvent))
		{

			desktopHandledEvent = false;
			// Try to consume the event, if that fails try to convert it
			Desktop->HandleEvent(sfEvent);

			if (!desktopHandledEvent)
			{
				Cam->HandleEvent(sfEvent);
				for (SFMLEventUser* U : sfEvtHandlers) U->HandleSfmlEvent(sfEvent);
			}

			// give it to the converter
			EvtConv->HandleSfmlEvent(sfEvent);
		}

		// don't draw if the window is closed
		if (!Engine::GetApp().isOpen()) return;

		// update desktop
		Desktop->Update(guiclock.restart().asSeconds());

		// Clear screen
		if (clear) Engine::GetApp().clear(bgColor);

		Cam->update();
		Picasso->render(Engine::GetApp());

		// draw SFGUI
		SFG->Display(Engine::GetApp());

		// Blit
		Engine::GetApp().display();

	}

	void Screen::HandleEvent(Event& e) {
		if (e.Is("WINDOW_RESIZE"))
		{
			float xzoom = Cam->getTargetSize().x/Engine::GetApp().getSize().x;
			float yzoom = Cam->getTargetSize().y/Engine::GetApp().getSize().y;
			Cam->setTargetSize(sf::Vector2f(Engine::GetApp().getSize().x, Engine::GetApp().getSize().y));
			Cam->zoom(std::min(xzoom, yzoom));
		}
		else if (e.Is("TOGGLE_FULLSCREEN"))
		{
			if (!Fullscreen)
			{
				auto modes = sf::VideoMode::getFullscreenModes();

				if (modes.size() > 0)
				{
					Engine::GetApp().create(modes[0], "SchiffbruchEngine powered (fullscreen)", sf::Style::Fullscreen);
					Fullscreen = true;
				}
				else
				{
					Engine::out(Engine::WARNING) << "[Screen] No supported fullscreen mode found!" << std::endl;
				}
			}
			else
			{
				Engine::GetApp().create(
					sf::VideoMode(	Engine::getCfg()->get <unsigned int> ("system.renderer.windowsize.x", 1024),
									Engine::getCfg()->get <unsigned int> ("system.renderer.windowsize.y", 768)),
					Engine::getCfg()->get<std::string>("system.renderer.title", "SchiffbruchEngine powered."));
				Fullscreen = false;
			}

			Module::Get()->QueueEvent("WINDOW_RESIZE", true);
		}
		else if (e.Is("SCREEN_ADD_WINDOW", typeid(sfg::Window::Ptr)))
		{
			sfg::Window::Ptr P = boost::any_cast<sfg::Window::Ptr>(e.Data());
			P->GetSignal(sfg::Window::OnMouseEnter).Connect(std::bind(&Screen::OnHandledEvent, this));

			Engine::out() << "[Screen] Adding Window '" << P->GetTitle().toAnsiString() << "'" << std::endl;
			Desktop->Add(P);
		}
		else if (e.Is("SCREEN_REMOVE_WINDOW", typeid(sfg::Window::Ptr)))
		{
			sfg::Window::Ptr P = boost::any_cast<sfg::Window::Ptr>(e.Data());

			Engine::out() << "[Screen] Removing Window '" << P->GetTitle().toAnsiString() << "'" << std::endl;
			Desktop->Remove(P);
		}
	}

	void Screen::removeSFMLEventHandler(SFMLEventUser* U) {
		std::vector<SFMLEventUser*>::iterator it = std::find(sfEvtHandlers.begin(), sfEvtHandlers.end(), U);
		if (it != sfEvtHandlers.end()) sfEvtHandlers.erase(it);
	}
} // namespace sbe
