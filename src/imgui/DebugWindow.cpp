#include <sbe/imgui/DebugWindow.hpp>

#include <sbe/util/console/CommandParser.hpp>
#include <sbe/gfx/Screen.hpp>
#include <sbe/Module.hpp>

#include <imgui.h>

namespace sbe
{
	DebugWindow::DebugWindow(const glm::ipoint2& /*RelativePosition*/, const glm::ivec2 /*Size*/)
			: visible(true), updateCounter(0) {
		consoleInputBuf[0] = '\0';

		RegisterForEvent("VIEW_DBG_STRING");
		RegisterForEvent("EVT_FRAME");
		RegisterForEvent("TOGGLE_SHOW_DBGWIN");

		Screen::get()->registerImGuiWidget(this);

		Engine::out() << "[DebugWindow] Initialized." << std::endl;
	}

	DebugWindow::~DebugWindow() {
		Screen::get()->unregisterImGuiWidget(this);
	}

	void DebugWindow::HandleEvent(Event& e) {
		if (e.Is("VIEW_DBG_STRING", typeid(std::pair<std::string, std::string>)))
		{
			auto D = boost::any_cast<std::pair<std::string, std::string>>(e.Data());
			DebugStrings[D.first] = D.second;
		}
		else if (e.Is("EVT_FRAME"))
		{
			if (updateCounter > 20)
			{
				updateCounter = 0;
				UpdateText();
			}
			updateCounter++;
		}
		else if (e.Is("TOGGLE_SHOW_DBGWIN"))
		{
			visible = !visible;
		}
	}

	void DebugWindow::renderImGui() {
		if (!visible) return;

		ImGui::Begin("Debug Information [F3]", &visible);

		// Two-column debug string view
		if (ImGui::BeginTable("dbgstrings", 2, ImGuiTableFlags_SizingFixedFit))
		{
			for (auto& [k, v] : DebugStrings)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(k.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::TextUnformatted(v.c_str());
			}
			ImGui::EndTable();
		}

		ImGui::Separator();

		// Scrolling log area
		ImGui::BeginChild("log", {0, 120}, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::TextUnformatted(logBuffer.c_str());
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
		ImGui::EndChild();

		// Console input with TAB completion and command history
		auto completionCb = [](ImGuiInputTextCallbackData* d) -> int {
			if (d->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
			{
				std::string s = Engine::GetCmdParser()->Complete(d->Buf);
				d->DeleteChars(0, d->BufTextLen);
				d->InsertChars(0, s.c_str());
			}
			else if (d->EventFlag == ImGuiInputTextFlags_CallbackHistory)
			{
				std::string cur(d->Buf, d->BufTextLen);
				int dir = (d->EventKey == ImGuiKey_UpArrow) ? 1 : -1;
				std::string s = Engine::GetCmdParser()->FetchHistory(cur, dir);
				d->DeleteChars(0, d->BufTextLen);
				d->InsertChars(0, s.c_str());
			}
			return 0;
		};

		bool enterPressed = ImGui::InputText(
			"##cmd", consoleInputBuf, sizeof(consoleInputBuf),
			ImGuiInputTextFlags_EnterReturnsTrue |
			ImGuiInputTextFlags_CallbackCompletion |
			ImGuiInputTextFlags_CallbackHistory,
			completionCb);

		if (enterPressed && consoleInputBuf[0])
		{
			Engine::GetCmdParser()->Execute(consoleInputBuf);
			consoleInputBuf[0] = '\0';
			ImGui::SetKeyboardFocusHere(-1);
		}

		ImGui::End();
	}

	void DebugWindow::UpdateText(FilterLevel level) {
		const int maxTextPerFrame = 1000;

		std::string newtext;

		switch (level)
		{
			case FilterLevel::DEFAULT:
				newtext += Engine::GetLogger()->GetLog();
				break;
			case FilterLevel::VERBOSE:
				newtext = Engine::GetLogger(Engine::INFO)->GetLog().substr(0, maxTextPerFrame);
				Engine::GetLogger(Engine::INFO)->ClearCache(newtext.size());

				newtext += Engine::GetLogger(Engine::WARNING)->GetLog().substr(0, maxTextPerFrame);
				Engine::GetLogger(Engine::WARNING)->ClearCache(newtext.size());

				newtext += Engine::GetLogger(Engine::ERROR)->GetLog().substr(0, maxTextPerFrame);
				Engine::GetLogger(Engine::ERROR)->ClearCache(newtext.size());

				Engine::GetLogger(Engine::SPAM)->ClearCache();
				break;
			case FilterLevel::PEDANTIC:
				break;
		}

		if (!newtext.empty())
			logBuffer += newtext;

		// cap buffer size to avoid unbounded growth
		const size_t maxLogBuffer = 32 * 1024;
		if (logBuffer.size() > maxLogBuffer)
			logBuffer.erase(0, logBuffer.size() - maxLogBuffer);
	}

} // namespace sbe
