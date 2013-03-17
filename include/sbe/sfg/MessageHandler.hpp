#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "sbe/event/EventUser.hpp"
#include "Message.hpp"

class MessageHandler : public EventUser
{
	public:
		MessageHandler();
		~MessageHandler() {}

		void RemoveAndDestroyMessage( const Message* m );
		void HandleEvent( Event& e);
	private:
		void SaveAndShowMessage( const std::shared_ptr<Message> m );
		std::list<std::shared_ptr<Message>> MessageList;
};
#endif // MESSAGEHANDLER_H
