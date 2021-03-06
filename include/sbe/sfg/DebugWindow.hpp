#ifndef DGB_WINDOW_H
#define DGB_WINDOW_H

#include <sbe/event/EventUser.hpp>
#include <memory>

namespace sfg
{
	class Label;

	class Box;

	class Window;

	class ScrolledWindow;

	class Entry;
}

#include <sbe/geom/Point.hpp>

namespace sbe
{
	/**
		A Simple Window showing the VIEW_DEBUG_STRING events and the log.
		On construction it will automatically register itself to the screen with the SCREEN_ADD_WINDOW event.
	*/
	class DebugWindow : public EventUser
	{
		enum FilterLevel
		{
			DEFAULT, VERBOSE, PEDANTIC
		};

	public:

		DebugWindow(const glm::ipoint2& RelativePosition = {0, 0}, const glm::ivec2 Size = {600, 280});

		~DebugWindow() { };

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
		void CreateWin(const glm::ipoint2& RelativePosition, const glm::ivec2 Size);

		bool ListenToActionKeys;

		void UpdateText(FilterLevel level = FilterLevel::VERBOSE);

		void AddLogText(std::string& newtext, unsigned int labelTextLimit);

		std::shared_ptr<sfg::Label> LogText;
		std::shared_ptr<sfg::Label> DbgText;
		std::shared_ptr<sfg::Label> DbgLabels;
		std::shared_ptr<sfg::Box> LogBox;
		std::shared_ptr<sfg::Window> Win;
		std::shared_ptr<sfg::ScrolledWindow> scrolledwindow;
		std::shared_ptr<sfg::Entry> ConsoleInput;
		unsigned int currentlabeltext;
		unsigned int updateCounter;

		// a list of debugging strings which are set by event
		std::map<std::string, std::string> DebugStrings;
	};
} // namespace sbe
#endif // DGB_WINDOW_H
