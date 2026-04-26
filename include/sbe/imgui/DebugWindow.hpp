#ifndef IMGUI_DBG_WINDOW_H
#define IMGUI_DBG_WINDOW_H

#include <sbe/event/EventUser.hpp>
#include <sbe/imgui/ImGuiWidget.hpp>
#include <sbe/geom/Point.hpp>

#include <map>
#include <string>

namespace sbe
{
	/**
		A simple window showing VIEW_DEBUG_STRING events and the log.
		On construction it registers itself with Screen for ImGui rendering.
		Toggle visibility with the TOGGLE_SHOW_DBGWIN event.
	*/
	class DebugWindow : public EventUser, public ImGuiWidget
	{
		enum FilterLevel
		{
			DEFAULT, VERBOSE, PEDANTIC
		};

	public:

		DebugWindow(const glm::ipoint2& RelativePosition = {0, 0}, const glm::ivec2 Size = {600, 280});

		~DebugWindow();

		void renderImGui() override;

	protected:
		/**
			Handles the following events:
				Event			|		Data
			--------------------|-----------------
			VIEW_DBG_STRING		|	std::pair<string,string>
			EVT_FRAME			|	-
			TOGGLE_SHOW_DBGWIN	|	-
		*/
		void HandleEvent(Event& e) override;

	private:
		void UpdateText(FilterLevel level = FilterLevel::VERBOSE);

		bool visible;

		std::string logBuffer;
		char consoleInputBuf[256];

		unsigned int updateCounter;

		// a list of debugging strings which are set by event
		std::map<std::string, std::string> DebugStrings;
	};
} // namespace sbe
#endif // IMGUI_DBG_WINDOW_H
