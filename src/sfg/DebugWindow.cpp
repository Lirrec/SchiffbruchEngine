#include "sbe/sfg/DebugWindow.hpp"

#include <sbe/util/console/CommandParser.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/ScrolledWindow.hpp>

using namespace sfg;

namespace sbe
{
	DebugWindow::DebugWindow( const Geom::Point& RelativePosition, const Geom::Vec2 Size)
	{
		RegisterForEvent( "VIEW_DBG_STRING" );
		RegisterForEvent( "EVT_FRAME" );
		RegisterForEvent( "TOGGLE_SHOW_DBGWIN" );
		RegisterForEvent( "KEY_PRESSED_ENTER" );
		RegisterForEvent( "KEY_PRESSED_TAB" );
		RegisterForEvent( "KEY_PRESSED_ARROW_UP" );
		RegisterForEvent( "KEY_PRESSED_ARROW_DOWN" );

		updateCounter = 0;
		currentlabeltext = 0;
		ListenToActionKeys = false;
		CreateWindow(RelativePosition, Size);
	}

	void DebugWindow::CreateWindow( const Geom::Point& RelativePosition, const Geom::Vec2 Size )
	{
		Win = Window::Create( Window::Style::TOPLEVEL | Window::Style::SHADOW );

		//DbgText and DbgLabels are different, so it looks like two columns.
		DbgText = Label::Create();
		DbgText->SetAlignment( sf::Vector2f(0.f, 0.f) );

		DbgLabels = Label::Create();
		DbgLabels->SetAlignment( sf::Vector2f(0.f, 0.f) );

		LogText = Label::Create();

		// create Inputbox for console commands.
		ConsoleInput = Entry::Create();

		ConsoleInput->GetSignal( Entry::OnGainFocus ).Connect( std::bind( &DebugWindow::EntryGainFocus , this) );
		ConsoleInput->GetSignal( Entry::OnLostFocus ).Connect( std::bind(  &DebugWindow::EntryLostFocus , this) );

		//ConsoleInput->AppendText( "Not yet implemented." );
		///ConsoleInput->SetState( Widget::State::INSENSITIVE );

		Win->SetPosition( sf::Vector2f(RelativePosition.x, RelativePosition.y ) );
		//Win->SetRequisition( sf::Vector2f(Size.x, Size.y ) );

		// main box, vertical
		Box::Ptr wholeBox( Box::Create( Box::Orientation::VERTICAL, 3.0f ) );
		// topbox, horizontal
		Box::Ptr topBox( Box::Create( Box::Orientation::HORIZONTAL, 3.0f ) );
			Box::Ptr boxInfo( Box::Create( Box::Orientation::HORIZONTAL, 3.0f ) );
			boxInfo->Pack( DbgLabels, false, false);
			boxInfo->Pack( DbgText, false, false);
		topBox->Pack(boxInfo, false, false);

			LogBox = Box::Create( Box::Orientation::VERTICAL );
			LogBox->Pack(LogText, true, true);

			scrolledwindow = ScrolledWindow::Create();
			scrolledwindow->AddWithViewport( LogBox );
			scrolledwindow->SetRequisition( sf::Vector2f( 400.f, 100.f ) );
			scrolledwindow->SetScrollbarPolicy( ScrolledWindow::HORIZONTAL_AUTOMATIC | ScrolledWindow::VERTICAL_AUTOMATIC );
		topBox->Pack ( scrolledwindow , true , true );
		wholeBox->Pack ( topBox , true , true );
		wholeBox->Pack ( ConsoleInput , false , false );


		// Create a window and add the box layouter to it. Also set the window's title.

		Win->SetTitle( "Debug Information [F3]" );
		Win->Add( wholeBox );

		Module::Get()->QueueEvent( Event("SCREEN_ADD_WINDOW", Win) );
	}

	void DebugWindow::HandleEvent( Event& e )
	{
		if (e.Is("VIEW_DBG_STRING", typeid( std::pair< std::string, std::string> )))
		{
			std::pair< std::string, std::string> D = boost::any_cast<  std::pair< std::string, std::string>  >(e.Data());
			DebugStrings[D.first] = D.second;
		}
		else if (e.Is("EVT_FRAME"))
		{
			if ( updateCounter > 20 )
			{
				updateCounter = 0;
				UpdateText();
			}
			updateCounter++;
		}
		else if (e.Is("TOGGLE_SHOW_DBGWIN"))
		{
			if (Win->IsGloballyVisible())
			{
				Win->Show(false);
			}
			else
			{
				Win->Show(true);
				Win->GrabFocus();
			}
		}
		else if ( e.Is( "KEY_PRESSED_TAB" ) && ListenToActionKeys )
		{
			std::string displayedCommandText = ConsoleInput->GetText().toAnsiString();
			if ( displayedCommandText != "" ) //if text exists
			{
				// send text to parser
				displayedCommandText = Engine::GetCmdParser()->Complete( displayedCommandText );
				// fill into sfg::entry
				ConsoleInput->SetText( displayedCommandText );
				ConsoleInput->SetCursorPosition( displayedCommandText.length() );
			}
		}
		else if ( e.Is( "KEY_PRESSED_ENTER" ) && ListenToActionKeys )
		{
			std::string displayedCommandText = ConsoleInput->GetText().toAnsiString();
			if ( displayedCommandText.empty() )
				return;

			// send text to parser
			Engine::GetCmdParser()->Execute( displayedCommandText );
			// clear sfg::entry
			ConsoleInput->SetCursorPosition( 0 );
			ConsoleInput->SetText( "" );

		}
		else if ( e.Is( "KEY_PRESSED_ARROW_UP" ) && ListenToActionKeys )
		{
			std::string displayedCommandText = ConsoleInput->GetText().toAnsiString();
			displayedCommandText = Engine::GetCmdParser()->FetchHistory( displayedCommandText, 1 );
			ConsoleInput->SetText( displayedCommandText );
			ConsoleInput->SetCursorPosition( displayedCommandText.length() );
		}
		else if ( e.Is( "KEY_PRESSED_ARROW_DOWN" ) && ListenToActionKeys )
		{
			std::string displayedCommandText = ConsoleInput->GetText().toAnsiString();
			displayedCommandText = Engine::GetCmdParser()->FetchHistory( displayedCommandText, -1 );
			ConsoleInput->SetText( displayedCommandText );
			ConsoleInput->SetCursorPosition( displayedCommandText.length() );
		}
	}

	void DebugWindow::EntryGainFocus()
	{
		ListenToActionKeys = true;
	}

	void DebugWindow::EntryLostFocus()
	{
		ListenToActionKeys = false;
	}

	void DebugWindow::UpdateText(FilterLevel level)
	{
		std::string values;
		std::string labels;

		const int maxTextPerFrame = 1000;
		const int labelTextLimit = 1000;

		for ( auto dbgString : DebugStrings )
		{
			labels += dbgString.first  + "\n";
			values += dbgString.second + "\n";
		}

		//Engine::out() << " Text: \n" << text << std::endl;

		DbgLabels->SetText( labels );
		DbgText->SetText( values );

		std::string newtext = "";
		int lastsize;

		switch ( level )
		{
			case FilterLevel::DEFAULT:
			newtext += Engine::GetLogger()->GetLog();
			break;
			case FilterLevel::VERBOSE:
				newtext = Engine::GetLogger(Engine::INFO)->GetLog().substr(0, maxTextPerFrame);
				Engine::GetLogger(Engine::INFO)->ClearCache( newtext.size() );
				lastsize = newtext.size();

				//Engine::out(Engine::SPAM) << "newtextsize (info) " << lastsize << std::endl;

				newtext += Engine::GetLogger(Engine::WARNING)->GetLog().substr(0, maxTextPerFrame);
				Engine::GetLogger(Engine::WARNING)->ClearCache( newtext.size() - lastsize);
				lastsize = newtext.size();

				//Engine::out(Engine::SPAM) << "newtextsize (warning) " << lastsize << std::endl;

				newtext += Engine::GetLogger(Engine::ERROR)->GetLog().substr(0, maxTextPerFrame);
				Engine::GetLogger(Engine::ERROR)->ClearCache( newtext.size() - lastsize);

				//Engine::out(Engine::SPAM) << "newtextsize (ERROR) " << lastsize << std::endl;

				Engine::GetLogger(Engine::SPAM)->ClearCache();
			break;
			case FilterLevel::PEDANTIC:
			break;
		}

		if (!newtext.empty())
		{
			AddLogText( newtext, labelTextLimit );
		}

	}

	void DebugWindow::AddLogText( std::string& newtext, int labelTextLimit )
	{

		if ((newtext.size() + LogText->GetText().getSize()) > labelTextLimit )
		{
			std::string labeltext = newtext.substr(0, labelTextLimit - LogText->GetText().getSize() );

			LogText->SetText( LogText->GetText() + labeltext);

			Engine::out() << " Adding one more label.. " << std::endl;
			LogText = Label::Create();
			LogText->SetAlignment( sf::Vector2f(0.f, 0.f) );
			LogBox->Pack(LogText, true, true);

			newtext = newtext.substr( labeltext.size() );
			AddLogText( newtext, labelTextLimit );
		}
		else
		{
			LogText->SetText( LogText->GetText() + newtext);
		}

		scrolledwindow->GetVerticalAdjustment()->SetValue( scrolledwindow->GetVerticalAdjustment()->GetUpper() );

	}

} // namespace sbe
