#include "sbe/sfg/MessageHandler.hpp"
#include "sbe/Module.hpp"

namespace sbe
{

	MessageHandler::MessageHandler()
			: pauseEvent("") {
		RegisterForEvent("NEW_MESSAGE");
		RegisterForEvent("CLOSE_MESSAGE");
		MessageList = std::list<std::shared_ptr<Message>>();
	}

	void MessageHandler::HandleEvent(Event& e) {
		if (e.Is("NEW_MESSAGE", typeid(std::shared_ptr<Message>)))
		{
			std::shared_ptr<Message> m = boost::any_cast<std::shared_ptr<Message>>(e.Data());
			SaveAndShowMessage(m);
		}
		else if (e.Is("CLOSE_MESSAGE", typeid(std::shared_ptr<Message>)))
		{
			std::shared_ptr<Message> m = boost::any_cast<std::shared_ptr<Message>>(e.Data());
			RemoveAndDestroyMessage(m.get());
		}
		else if (e.Is("CLOSE_MESSAGE", typeid(std::string)))
		{
			std::string title = boost::any_cast<std::string>(e.Data());
			RemoveAndDestroyMessage(title);
		}
	}

	void MessageHandler::SaveAndShowMessage(std::shared_ptr<Message> m) {
		m->SaveMessageHandler(this);
		MessageList.push_front(m);
		m->ShowMessage();
		if (m->doPause() && pauseEvent != "") Module::Get()->QueueEvent(pauseEvent);
	}

	void MessageHandler::RemoveAndDestroyMessage(const Message* m) {
		auto it = std::find_if(MessageList.begin(), MessageList.end(), [m](const std::shared_ptr<Message>& p) { return p.get() == m; });
		if (it != MessageList.end())
		{
			(*it)->Close();
			MessageList.erase(it);
		}
	}

	void MessageHandler::RemoveAndDestroyMessage(const std::string& title) {
		auto it = std::find_if(MessageList.begin(), MessageList.end(), [title](const std::shared_ptr<Message>& p) { return p->getTitle() == title; });
		if (it != MessageList.end())
		{
			(*it)->Close();
			MessageList.erase(it);
		}
	}

} // namespace sbe
