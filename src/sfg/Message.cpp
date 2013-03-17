#include "sbe/sfg/Message.hpp"

#include "sbe/sfg/MessageHandler.hpp"

#include <SFGUI/Window.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Button.hpp>

using namespace sfg;

namespace sbe
{

	Message::Message ( Message::Type type_, std::string title_, std::string message_, std::string answerEventName_ )
	{
		RegisterForEvent( "WINDOW_RESIZE" );
		RegisterForEvent( "TOGGLE_FULLSCREEN" );
		Type_ = type_;
		Title_ = title_;
		Message_ = message_;
		AnswerEventName_ = answerEventName_;
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
		else if ( Type_ == Type::CHOICE )
		{
			MakeChoiceMessage();
		}
		updatePosition();
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
		box->Pack( Label::Create( Message_ ) );
		Button::Ptr ok = Button::Create( "OK" );
		ok->GetSignal( Button::OnLeftClick ).Connect( &Message::Ok, this );
		box->Pack( ok, false, false );
		Win->Add( box );
		Module::Get()->QueueEvent( Event( "SCREEN_ADD_WINDOW", Win ) );
	}

	void Message::MakeChoiceMessage()
	{
		Box::Ptr box = Box::Create( Box::VERTICAL, 0 );
		box->Pack( Label::Create( Message_ ) );
		Box::Ptr buttonBox = Box::Create( Box::HORIZONTAL, 0 );
		Button::Ptr abort = Button::Create( "Abort" );
		abort->GetSignal( Button::OnLeftClick ).Connect( &Message::Ok, this );
		buttonBox->Pack( abort, false, false );
		Button::Ptr confirm = Button::Create( "Confirm" );
		confirm->GetSignal( Button::OnLeftClick ).Connect( &Message::Ok, this );
		buttonBox->Pack( confirm, false, false );
		box->Add( buttonBox );
		Win->Add( box );
		Module::Get()->QueueEvent( Event( "SCREEN_ADD_WINDOW", Win ) );
	}

	void Message::Ok ()
	{
		Win->Show( false );
		Module::Get()->QueueEvent( AnswerEventName_, true );
		Handler->RemoveAndDestroyMessage( this );
	}

	void Message::Abort()
	{
		Win->Show( false );
		Module::Get()->QueueEvent( Event( AnswerEventName_, false ), true );
		Handler->RemoveAndDestroyMessage( this );
	}

	void Message::Confirm()
	{
		Win->Show( false );
		Module::Get()->QueueEvent( Event( AnswerEventName_, true ), true );
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
