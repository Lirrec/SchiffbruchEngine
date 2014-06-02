#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "sbe/event/EventUser.hpp"
#include "Message.hpp"

namespace sbe
{
    /**
        TODO: document this
        This class handles new message events and adds/removes them from the screen.
        An instantiation of this class is required to show messages.

        Messages are sent via Events ADD_MESSAGE(
    */
	class MessageHandler : public EventUser
	{
		public:
			MessageHandler();
			~MessageHandler() {}

			void RemoveAndDestroyMessage( const Message* m );
			void RemoveAndDestroyMessage( const std::string& title );

			void setPauseEvent( const std::string& _pauseEvent) { pauseEvent = _pauseEvent;}
		protected:
			/**
				Handles the following events:
					Name		| Datatype
				----------------+------------------------------------------
				NEW_MESSAGE		| std::shared_ptr<Message>
				CLOSE_MESSAGE	| std::shared_ptr<Message>  OR std::string
			*/
			void HandleEvent( Event& e) override;
		private:
			void SaveAndShowMessage( const std::shared_ptr<Message> m );
			std::list<std::shared_ptr<Message>> MessageList;
			/// the name of the event to send if a message wants to interrupt the current process
			std::string pauseEvent;
	};

} // namespace sbe
#endif // MESSAGEHANDLER_H
