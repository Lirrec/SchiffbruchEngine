#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "sbe/event/EventUser.hpp"
#include "Message.hpp"

namespace sbe
{

	class MessageHandler : public EventUser
	{
		public:
			MessageHandler();
			~MessageHandler() {}

			void RemoveAndDestroyMessage( const Message* m );
			void HandleEvent( Event& e);
			void setPauseEvent( const std::string& _pauseEvent) { pauseEvent = _pauseEvent;}
		private:
			void SaveAndShowMessage( const std::shared_ptr<Message> m );
			std::list<std::shared_ptr<Message>> MessageList;
			/// the name of the event to send if a message wants to interrupt the current process
			std::string pauseEvent;
	};

} // namespace sbe
#endif // MESSAGEHANDLER_H
