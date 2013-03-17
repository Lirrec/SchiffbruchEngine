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
		private:
			void SaveAndShowMessage( const std::shared_ptr<Message> m );
			std::list<std::shared_ptr<Message>> MessageList;
	};

} // namespace sbe
#endif // MESSAGEHANDLER_H
