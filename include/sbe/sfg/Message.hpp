#ifndef MESSAGE_H
#define MESSAGE_H

#include <sbe/event/EventUser.hpp>

namespace sfg
{
	class Window;
}

#include <sbe/geom/Point.hpp>

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
	///
	///
	///  Type   | Data          | Description ( @see Message::Type )
	/// ========+===========
	///  OK     | no Data
	///  MODAL  | no Data
	///  CHOICE | bool ( false if abort, true if confirm )
	/// ========+===========

	class Message : public EventUser
	{
	public:

		enum Type
		{
			OK,     /// A closable Message with an "ok" button
			MODAL,  /// MODAL messages can't be closed by the user and have to be removed with a CLOSE_MESSAGE event
			CHOICE  /// Allows the User to make a Yes/No, Ok/Abort choice, the Answer event will contain a bool
		};

		Message(const Message::Type type_, const std::string& title_, const std::string& message_, const std::string& answerEventName_ = "",
				bool pause_ = false);

		~Message() { }

		void ShowMessage();

		void SaveMessageHandler(MessageHandler* h);

		bool doPause() { return pause; }

		const std::string& getTitle() { return Title_; }

		void Close();

	private:
		void MakeOkMessage();

		void MakeModalMessage();

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

		std::shared_ptr<sfg::Window> Win;

		MessageHandler* Handler;
	};

} // namespace sbe

#endif // MESSAGE_H

