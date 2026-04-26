#include "config.h"
#include <sbe/gfx/Screen.hpp>

#include <sbe/Config.hpp>

#include <sbe/event/SFMLEventUser.hpp>
#include <sbe/event/SFMLEvent.hpp>

#include <sbe/gfx/Renderer.hpp>
#include <sbe/gfx/Camera.hpp>
#include <sbe/imgui/ImGuiWidget.hpp>

#include <sbe/Module.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Image.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <algorithm>


namespace sbe
{
	DLLEXPORT Screen* Screen::Instance = nullptr;

	Screen* Screen::get() {
		return Instance;
	}

	/// static access to the Camera
	std::shared_ptr<Camera> Screen::sCam() { assert(Instance->Cam); return Instance->Cam; }

	/// static access to the Renderer
	std::shared_ptr<Renderer> Screen::sRndr() { assert(Instance->Picasso); return Instance->Picasso; }

	/// static Access to the SFMLEventConverter
	std::shared_ptr<SFMLEventConverter> Screen::sEvtConv() { assert(Instance->EvtConv); return Instance->EvtConv; }

	Screen::Screen()
			: Fullscreen(false),
			  clear(true) {
		Instance = this;

		EvtConv = std::make_shared<SFMLEventConverter>();
		RegisterForEvent("EVT_FRAME", [this](Event&) { Render(); });
		RegisterForEvent("EVT_QUIT", [this](Event&) { Module::Get()->RequestQuit(); });

		RegisterForEvent("WINDOW_RESIZE");
		RegisterForEvent("TOGGLE_FULLSCREEN");

		// load keybindings from default config
		int i = EvtConv->LoadKeyBindingsFromConfig();
		Engine::out(Engine::INFO) << "[Screen] Loaded " << i << " key bindings." << std::endl;

		EvtConv->AddEventConversion<sf::Event::Closed>("EVT_QUIT", true);
		EvtConv->AddEventConversion<sf::Event::Resized>("WINDOW_RESIZE");

		Init();


		Engine::out() << "[Screen] Initialized." << std::endl;
	}

	Screen::~Screen() {
		ImGui::SFML::Shutdown();
	}

	void Screen::Init() {
		Engine::out(Engine::INFO) << "[Screen] Creating Window..." << std::endl;

		bgColor = sf::Color(180, 180, 180);

		std::string Icon = "res/" + Engine::getCfg()->get<std::string>("system.renderer.icon", "");
		std::string Title = Engine::getCfg()->get<std::string>("system.renderer.title", "SchiffbruchEngine powered.");

		// create the renderwindow
		Engine::GetApp().create(
			sf::VideoMode({	Engine::getCfg()->get<unsigned int>("system.renderer.windowsize.x", 1024),
							Engine::getCfg()->get<unsigned int>("system.renderer.windowsize.y", 768)}),
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
					Engine::GetApp().setIcon(IconImage.getSize(), IconImage.getPixelsPtr());
				else
					Engine::out(Engine::ERROR) << "[Screen] Icon '" << Icon << "' is larger than 256x256!" << std::endl;
			}

		}

		assert(ImGui::SFML::Init(Engine::GetApp()));

		Cam = std::make_shared<Camera>();
		Cam->setup();

		Picasso = std::make_shared<Renderer>();

		// We're not using SFML to render anything in this program, so reset OpenGL
		// states. Otherwise we wouldn't see anything.
		Engine::GetApp().resetGLStates();

		imguiClock.restart();

		Module::Get()->QueueEvent("SCREEN_CREATED", true);
	}


	void Screen::Render() {
		// Process Hardware/SFML Events
		while (const auto sfEvent = Engine::GetApp().pollEvent())
		{
			ImGui::SFML::ProcessEvent(Engine::GetApp(), *sfEvent);

			const auto& io = ImGui::GetIO();
			if (!io.WantCaptureMouse && !io.WantCaptureKeyboard)
			{
				Cam->HandleEvent(*sfEvent);
				for (SFMLEventUser* U : sfEvtHandlers) U->HandleSfmlEvent(*sfEvent);
			}

			// always give it to the converter (key bindings fire regardless of ImGui focus)
			EvtConv->HandleSfmlEvent(*sfEvent);
		}

		// don't draw if the window is closed
		if (!Engine::GetApp().isOpen()) return;

		// update ImGui
		ImGui::SFML::Update(Engine::GetApp(), imguiClock.restart());

		// Clear screen
		if (clear) Engine::GetApp().clear(bgColor);

		Cam->update();
		Picasso->render(Engine::GetApp());

		// draw all registered ImGui widgets
		for (ImGuiWidget* w : imguiWidgets) w->renderImGui();

		// render ImGui draw lists to window
		ImGui::SFML::Render(Engine::GetApp());

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
					Engine::GetApp().create(modes[0], "SchiffbruchEngine powered (fullscreen)", sf::State::Fullscreen);
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
					sf::VideoMode({	Engine::getCfg()->get<unsigned int>("system.renderer.windowsize.x", 1024),
									Engine::getCfg()->get<unsigned int>("system.renderer.windowsize.y", 768)}),
					Engine::getCfg()->get<std::string>("system.renderer.title", "SchiffbruchEngine powered."));
				Fullscreen = false;
			}

			Module::Get()->QueueEvent("WINDOW_RESIZE", true);
		}
	}

	void Screen::registerImGuiWidget(ImGuiWidget* w) {
		imguiWidgets.push_back(w);
	}

	void Screen::unregisterImGuiWidget(ImGuiWidget* w) {
		auto it = std::find(imguiWidgets.begin(), imguiWidgets.end(), w);
		if (it != imguiWidgets.end()) imguiWidgets.erase(it);
	}

	void Screen::removeSFMLEventHandler(SFMLEventUser* U) {
		std::vector<SFMLEventUser*>::iterator it = std::find(sfEvtHandlers.begin(), sfEvtHandlers.end(), U);
		if (it != sfEvtHandlers.end()) sfEvtHandlers.erase(it);
	}
} // namespace sbe
