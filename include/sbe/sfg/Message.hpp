#ifndef MESSAGE_H
#define MESSAGE_H

#include "sbe/event/EventUser.hpp"
#include <SFGUI/SharedPtr.hpp>
namespace sfg { class Window; }
#include "sbe/geom/Point.hpp"

namespace sbe
{

	class MessageHandler;

	/// The class Message is used in constant touch of the MessageHandler.
	/// To display a Message, create an instance of Message with the parameters:
	/// @param Type        type_
	/// @param std::string title_
	/// @param std::string message_
	/// @param std::string answerEventName_ in the format "MESSAGE_ANSWER_<classname>"
	/// and send an Event named "NEW_MESSAGE" with an std::shared_ptr<Message> of the Message object as Data.
	/// Dont forget to listen for the same string as @p answerEventName_, which will give you some kind of Data depending on the Type of Message:
	///
	///  Type   | Data
	/// ========+===========
	///  OK     | no Data
	///  CHOICE | bool ( false if abort, true if confirm )
	/// ========+===========

	class Message : public EventUser
	{
		public:

			enum Type { OK, CHOICE };
			Message( const Message::Type type_, const std::string& title_, const std::string& message_, const std::string& answerEventName_ = "", bool pause_ = false );
			~Message() {}
			void ShowMessage();
			void SaveMessageHandler( MessageHandler* h );
			bool doPause() { return pause; }

			void HandleEvent( Event& e);
		private:
			void MakeOkMessage();
			void MakeChoiceMessage();
			void Ok();
			void Abort();
			void Confirm();
			void updatePosition();

			Message::Type Type_;
			std::string Title_;
			std::string Message_;
			std::string AnswerEventName_;
			bool pause;

			sfg::SharedPtr<sfg::Window> Win;

			MessageHandler* Handler;
	};

} // namespace sbe

#endif // MESSAGE_H
