#include "sbe/sfg/Message.hpp"

#include "sbe/sfg/MessageHandler.hpp"

#include <SFGUI/Window.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Button.hpp>

using namespace sfg;

namespace sbe
{

	Message::Message ( Message::Type type_, const std::string& title_, const std::string& message_, const std::string& answerEventName_, bool pause_ )
	{
		RegisterForEvent( "WINDOW_RESIZE" );
		RegisterForEvent( "TOGGLE_FULLSCREEN" );
		Type_ = type_;
		Title_ = title_;
		Message_ = message_;
		AnswerEventName_ = answerEventName_;
		pause = pause_;
	}

	void Message::ShowMessage()
	{
		Win = Window::Create( Window::Style::TITLEBAR | Window::Style::BACKGROUND | Window::Style::SHADOW );
		Win->SetRequisition( sf::Vector2f( 200, 100 ) );
		Win->SetTitle( Title_ );
		if ( Type_ == Type::OK )
		{
			MakeOkMessage();
		}
        else if ( Type_ == Type::MODAL )
		{
			MakeModalMessage();
		}
		else if ( Type_ == Type::CHOICE )
		{
			MakeChoiceMessage();
		}
		updatePosition();
	}

    void Message::Close()
    {
        Win->Show(false);
        Module::Get()->QueueEvent( Event( "SCREEN_ADD_WINDOW", Win ) );
    }

	void Message::HandleEvent( Event& e )
	{
		if ( e.Is( "WINDOW_RESIZE" ) || e.Is( "TOGGLE_FULLSCREEN" ) )
		{
			updatePosition();
		}
	}

	void Message::MakeOkMessage()
	{
		Box::Ptr box = Box::Create( Box::VERTICAL, 0 );
		box->Pack( Label::Create( Message_ ));
		Box::Ptr buttonBox = Box::Create( Box::HORIZONTAL, 0 );
		{
			Box::Ptr spacer = Box::Create();
			spacer->SetRequisition ( sf::Vector2f(10, 10) );
			buttonBox->Pack ( spacer, true, true );
		}
		Button::Ptr ok = Button::Create( "OK" );
		ok->GetSignal( Button::OnLeftClick ).Connect( &Message::Ok, this );
		ok->SetRequisition ( sf::Vector2f(50, 10) );
		buttonBox->Pack ( ok, false, false );
		box->Pack( buttonBox, false, false );
		{
			Box::Ptr spacer = Box::Create();
			spacer->SetRequisition ( sf::Vector2f(10, 10) );
			buttonBox->Pack ( spacer, true, true );
		}

		Win->Add( box );
		Module::Get()->QueueEvent( Event( "SCREEN_ADD_WINDOW", Win ) );
	}

    void Message::MakeModalMessage()
    {
		Box::Ptr box = Box::Create( Box::VERTICAL, 0 );
		box->Pack( Label::Create( Message_ ));
        Box::Ptr spacer = Box::Create();
        spacer->SetRequisition ( sf::Vector2f(10, 10) );
        box->Pack ( spacer, true, true );

		Win->Add( box );
		Module::Get()->QueueEvent( Event( "SCREEN_ADD_WINDOW", Win ) );
    }

	void Message::MakeChoiceMessage()
	{
		Box::Ptr box = Box::Create( Box::VERTICAL, 0 );
		box->Pack( Label::Create( Message_ ) );
		Box::Ptr buttonBox = Box::Create( Box::HORIZONTAL, 0 );
		{
			Box::Ptr spacer = Box::Create();
			spacer->SetRequisition ( sf::Vector2f(10, 10) );
			buttonBox->Pack ( spacer, true, true );
		}
		Button::Ptr abort = Button::Create( "Abort" );
		abort->SetRequisition ( sf::Vector2f(50, 10) );
		abort->GetSignal( Button::OnLeftClick ).Connect( &Message::Abort, this );
		buttonBox->Pack( abort, false, false );
		Button::Ptr confirm = Button::Create( "Confirm" );
		confirm->SetRequisition ( sf::Vector2f(50, 10) );
		confirm->GetSignal( Button::OnLeftClick ).Connect( &Message::Confirm, this );
		buttonBox->Pack( confirm, false, false );
		{
			Box::Ptr spacer = Box::Create();
			spacer->SetRequisition ( sf::Vector2f(10, 10) );
			buttonBox->Pack ( spacer, true, true );
		}
		box->Pack( buttonBox );
		Win->Add( box );
		Module::Get()->QueueEvent( Event( "SCREEN_ADD_WINDOW", Win ) );
	}

	void Message::Ok ()
	{
		Win->Show( false );
		if ( AnswerEventName_ != "" ) Module::Get()->QueueEvent( AnswerEventName_, true );
		Handler->RemoveAndDestroyMessage( this );
	}

	void Message::Abort()
	{
		Win->Show( false );
		if ( AnswerEventName_ != "" ) Module::Get()->QueueEvent( Event( AnswerEventName_, false ), true );
		Handler->RemoveAndDestroyMessage( this );
	}

	void Message::Confirm()
	{
		Win->Show( false );
		if ( AnswerEventName_ != "" ) Module::Get()->QueueEvent( Event( AnswerEventName_, true ), true );
		Handler->RemoveAndDestroyMessage( this );
	}

	void Message::updatePosition()
	{
		sf::FloatRect Allocation = Win->GetAllocation();
		sf::Vector2u winSize =  Engine::GetApp().getSize();
		Win->SetPosition( sf::Vector2f( ( winSize.x - Allocation.width )/2 , ( winSize.y - Allocation.height )/2 ) );
	}

	void Message::SaveMessageHandler( MessageHandler* h )
	{
		Handler = h;
	}

} // namespace sbe
