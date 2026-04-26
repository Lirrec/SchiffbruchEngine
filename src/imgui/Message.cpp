#include <sbe/imgui/Message.hpp>
#include <sbe/imgui/MessageHandler.hpp>

#include <sbe/gfx/Screen.hpp>
#include <sbe/Module.hpp>

#include <imgui.h>

namespace sbe
{

	Message::Message(Message::Type type_, const std::string& title_, const std::string& message_,
					 const std::string& answerEventName_, bool pause_)
			: Type_(type_), Title_(title_), Message_(message_), AnswerEventName_(answerEventName_),
			  pause(pause_), visible(false), needsOpenPopup(false), Handler(nullptr) {
	}

	void Message::ShowMessage() {
		visible = true;
		needsOpenPopup = true;
		Screen::get()->registerImGuiWidget(this);
	}

	void Message::Close() {
		visible = false;
		Screen::get()->unregisterImGuiWidget(this);
	}

	void Message::renderImGui() {
		if (!visible) return;

		if (needsOpenPopup)
		{
			ImGui::OpenPopup(Title_.c_str());
			needsOpenPopup = false;
		}

		bool began = false;
		if (Type_ == Type::MODAL)
			began = ImGui::BeginPopupModal(Title_.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		else
			began = ImGui::BeginPopup(Title_.c_str());

		if (began)
		{
			ImGui::TextUnformatted(Message_.c_str());
			ImGui::Separator();

			if (Type_ == Type::OK)
			{
				if (ImGui::Button("OK")) Ok();
			}
			else if (Type_ == Type::CHOICE)
			{
				if (ImGui::Button("Confirm")) Confirm();
				ImGui::SameLine();
				if (ImGui::Button("Abort"))   Abort();
			}

			ImGui::EndPopup();
		}
	}

	void Message::Ok() {
		ImGui::CloseCurrentPopup();
		if (AnswerEventName_ != "") Module::Get()->QueueEvent(AnswerEventName_, true);
		Handler->RemoveAndDestroyMessage(this);
	}

	void Message::Abort() {
		ImGui::CloseCurrentPopup();
		if (AnswerEventName_ != "") Module::Get()->QueueEvent(Event(AnswerEventName_, false), true);
		Handler->RemoveAndDestroyMessage(this);
	}

	void Message::Confirm() {
		ImGui::CloseCurrentPopup();
		if (AnswerEventName_ != "") Module::Get()->QueueEvent(Event(AnswerEventName_, true), true);
		Handler->RemoveAndDestroyMessage(this);
	}

	void Message::SaveMessageHandler(MessageHandler* h) {
		Handler = h;
	}

} // namespace sbe
