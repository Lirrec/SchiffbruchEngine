#include "sbe/sfg/MessageHandler.hpp"
#include <algorithm>

namespace sbe
{

	MessageHandler::MessageHandler()
	{
		RegisterForEvent( "NEW_MESSAGE" );
		MessageList = std::list<std::shared_ptr<Message>>();
	}

	void MessageHandler::HandleEvent( Event& e )
	{
		if ( e.Is( "NEW_MESSAGE", typeid( std::shared_ptr<Message> ) ) )
		{
			std::shared_ptr<Message> m = boost::any_cast<std::shared_ptr<Message>>( e.Data() );
			SaveAndShowMessage( m );
		}
	}

	void MessageHandler::SaveAndShowMessage( std::shared_ptr<Message> m )
	{
		m->SaveMessageHandler( this );
		MessageList.push_front( m );
		m->ShowMessage();
	}

	void MessageHandler::RemoveAndDestroyMessage( const Message* m )
	{
		auto it = std::find_if( MessageList.begin(), MessageList.end(), [m]( const std::shared_ptr<Message>& p ){ return p.get() == m; } );
		if ( it != MessageList.end() )
		{
			MessageList.erase( it );
		}
	}

} // namespace sbe
