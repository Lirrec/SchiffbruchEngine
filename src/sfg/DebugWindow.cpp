#include "sbe/sfg/DebugWindow.hpp"

#include "sbe/gfx/Screen.hpp"

DebugWindow::DebugWindow( const Geom::Point& RelativePosition, const Geom::Vec2 Size)
{
	RegisterForEvent( "VIEW_DBG_STRING" );
	RegisterForEvent( "EVT_FRAME" );
	RegisterForEvent( "TOGGLE_SHOW_DBGWIN" );

	updateCounter = 0;
	currentlabeltext = 0;
	CreateWindow(RelativePosition, Size);
}

void DebugWindow::CreateWindow( const Geom::Point& RelativePosition, const Geom::Vec2 Size )
{
    Win = sfg::Window::Create( sfg::Window::Style::BACKGROUND | sfg::Window::Style::TITLEBAR | sfg::Window::Style::SHADOW  | sfg::Window::Style::RESIZE );

    //DbgText and DbgLabels are different, so it looks like two columns.
	DbgText = sfg::Label::Create();
	DbgText->SetAlignment( sf::Vector2f(0.f, 0.f) );

	DbgLabels = sfg::Label::Create();
	DbgLabels->SetAlignment( sf::Vector2f(0.f, 0.f) );

	LogText = sfg::Label::Create();

    // create Inputbox for console commands.
    sfg::Entry::Ptr consoleInput = sfg::Entry::Create();

    consoleInput->GetSignal( sfg::Entry::OnTextChanged ).Connect( &Screen::OnHandledEvent , Screen::get() );

    //consoleInput->AppendText( "Not yet implemented." );
    consoleInput->SetState( sfg::Widget::State::INSENSITIVE );

	Win->SetPosition( sf::Vector2f(RelativePosition.x, RelativePosition.y ) );
	//Win->SetRequisition( sf::Vector2f(Size.x, Size.y ) );

	// main box, vertical
	sfg::Box::Ptr wholeBox( sfg::Box::Create( sfg::Box::VERTICAL, 3.0f ) );
	// topbox, horizontal
	sfg::Box::Ptr topBox( sfg::Box::Create( sfg::Box::HORIZONTAL, 3.0f ) );
		sfg::Box::Ptr boxInfo( sfg::Box::Create( sfg::Box::HORIZONTAL, 3.0f ) );
		boxInfo->Pack( DbgLabels, false, false);
		boxInfo->Pack( DbgText, false, false);
	topBox->Pack(boxInfo, false, false);

		LogBox = sfg::Box::Create( sfg::Box::VERTICAL );
		LogBox->Pack(LogText, true, true);

		scrolledwindow = sfg::ScrolledWindow::Create();
		scrolledwindow->AddWithViewport( LogBox );
		scrolledwindow->SetRequisition( sf::Vector2f( 400.f, 100.f ) );
		scrolledwindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	topBox->Pack ( scrolledwindow , true , true );
	wholeBox->Pack ( topBox , true , true );
	wholeBox->Pack ( consoleInput , false , false );


	// Create a window and add the box layouter to it. Also set the window's title.

	Win->SetTitle( "Debug Information [F3]" );
	Win->Add( wholeBox );

	Event e("SCREEN_ADD_WINDOW");
	e.SetData( Win );
	Module::Get()->QueueEvent( e );
}

void DebugWindow::HandleEvent( Event& e )
{
	if (e.Is("VIEW_DBG_STRING"))
	{
		//Engine::out() << "[DebugWindow] DebugString Event" << std::endl;

		if (e.Data().type() == typeid( std::pair< std::string, std::string> ))
		{
			std::pair< std::string, std::string> D = boost::any_cast<  std::pair< std::string, std::string>  >(e.Data());
			DebugStrings[D.first] = D.second;

		}
		else
		{
			Engine::out(Engine::ERROR) << "[DebugWindow] DebugString Event with wrong parameters" << std::endl;
		}
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
		LogText = sfg::Label::Create();
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
