#include "sbe/gfx/Screen.hpp"

#include "sbe/Engine.hpp"
#include "sbe/Module.hpp"
#include "sbe/Config.hpp"

#include "sbe/event/EventUser.hpp"
#include "sbe/event/SFMLEventUser.hpp"
#include "sbe/event/SFMLEvent.hpp"

#include "sbe/gfx/Renderer.hpp"
#include "sbe/gfx/Camera.hpp"

#include <SFML/System/Clock.hpp>
#include <SFGUI/SFGUI.hpp>


// ############# SCREEN ####

Screen* Screen::Instance = nullptr;


Screen::Screen()
 : Fullscreen(false)
{
	Instance = this;

	EvtConv.reset( new SFMLEventConverter() );
	RegisterForEvent( "EVT_FRAME" );
	RegisterForEvent( "EVT_QUIT" );
	RegisterForEvent( "WINDOW_RESIZE" );
	RegisterForEvent( "SCREEN_ADD_WINDOW" );
	RegisterForEvent( "TOGGLE_FULLSCREEN" );

	// load keybindings from default config
	int i = EvtConv->LoadKeyBindingsFromConfig();
	Engine::out(Engine::INFO) << "[Screen] Loaded " << i << " key bindings." << std::endl;

	EvtConv->AddEventConversion( sf::Event::Closed , "EVT_QUIT", true );
	EvtConv->AddEventConversion( sf::Event::Resized , "WINDOW_RESIZE" );

	Init();
}

void Screen::Init()
{
	Engine::out(Engine::INFO) << "[Screen] Creating Window..." << std::endl;

	bgColor = sf::Color(180,180,180);

	// create the renderwindow
	Engine::GetApp().create( sf::VideoMode ( Engine::getCfg()->get<int>("system.renderer.windowsize.x"),
											Engine::getCfg()->get<int>("system.renderer.windowsize.y") ),
											"SchiffbruchEngine powered." );
	// must be created before using SFGUI
	SFG.reset ( new sfg::SFGUI );

	// top-level container for all SFGUI widgets
	Desktop.reset ( new sfg::Desktop );
	Cam.reset ( new Camera );
	Cam->setup();

	Picasso.reset ( new Renderer );

	// We're not using SFML to render anything in this program, so reset OpenGL
    // states. Otherwise we wouldn't see anything.
    Engine::GetApp().resetGLStates();

	guiclock.restart();

}


void Screen::Render()
{
	// Process Hardware/SFML Events
	sf::Event sfEvent;


	while ( Engine::GetApp().pollEvent ( sfEvent ) )
	{

		desktopHandledEvent = false;
		// Try to consume the event, if that fails try to convert it
		Desktop->HandleEvent( sfEvent );

		if ( !desktopHandledEvent )
		{
			Cam->HandleEvent( sfEvent );
			for ( SFMLEventUser* U : sfEvtHandlers ) U->HandleSfmlEvent( sfEvent );
		}

		// give it to the converter
		EvtConv->HandleSfmlEvent( sfEvent );
	}

	// don't draw if the window is closed
	if (!Engine::GetApp().isOpen()) return;

	// update desktop
	Desktop->Update( guiclock.restart().asSeconds() );

	// Clear screen
	Engine::GetApp().clear( bgColor );

	Cam->update();
	Picasso->render( Engine::GetApp() );

	// draw SFGUI
	SFG->Display( Engine::GetApp() );

	// Blit
	Engine::GetApp().display();

}

void Screen::HandleEvent(Event& e)
{
	if (e.Is("EVT_FRAME"))
	{
		Render();
	}
	else if(e.Is("WINDOW_RESIZE")){
		Cam->setTargetSize(sf::Vector2f( Engine::GetApp().getSize().x, Engine::GetApp().getSize().y));
	}
	else if (e.Is("TOGGLE_FULLSCREEN"))
	{
		if ( !Fullscreen )
		{
			auto modes = sf::VideoMode::getFullscreenModes();

			if (modes.size() > 0)
			{
				Engine::GetApp().create(modes[0], "Maximum-Fish (fullscreen)", sf::Style::Fullscreen);
				Fullscreen = true;
			}
			else
			{
				Engine::out(Engine::WARNING) << "[Screen] No supported fullscreen mode found!" << std::endl;
			}
		}
		else
		{
			Engine::GetApp().create( sf::VideoMode ( Engine::getCfg()->get<int>("system.renderer.windowsize.x"),
														Engine::getCfg()->get<int>("system.renderer.windowsize.y") ),
														"Maximum-Fish!" );
			Fullscreen = false;
		}

		Event ev("WINDOW_RESIZE");
		Module::Get()->QueueEvent(ev, true);
	}
	else if (e.Is("SCREEN_ADD_WINDOW"))
	{
		if (e.Data().type() == typeid( sfg::Window::Ptr ) )
		{
			sfg::Window::Ptr P = boost::any_cast<sfg::Window::Ptr>(e.Data());
			P->GetSignal( sfg::Window::OnMouseEnter ).Connect( &Screen::OnHandledEvent, this );

			Engine::out() << "[Screen] Adding Window " << P->GetTitle().toAnsiString() << std::endl;
			Desktop->Add(P);
		}
		else
		{
			Engine::out(Engine::ERROR) << "[Screen] SCREEN_ADD_WIDGET Event with wrong parameters" << std::endl;
		}
	}
	else if (e.Is("EVT_QUIT"))
	{
		Module::Get()->RequestQuit();
	}
}

void Screen::removeSFMLEventHandler( SFMLEventUser* U )
{
	std::vector<SFMLEventUser*>::iterator it = std::find(sfEvtHandlers.begin(), sfEvtHandlers.end(), U);
	if ( it != sfEvtHandlers.end() ) sfEvtHandlers.erase(it);
}
