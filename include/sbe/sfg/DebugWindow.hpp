#ifndef DGB_WINDOW_H
#define DGB_WINDOW_H

#include "sbe/event/EventUser.hpp"
#include <memory>

#include <SFGUI/SharedPtr.hpp>
namespace sfg {
		class Label;
		class Box;
		class Window;
		class ScrolledWindow;
		class Entry;
}

#include "sbe/geom/Point.hpp"

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

			DebugWindow( const Geom::Point& RelativePosition = Geom::Point(0,0), const Geom::Vec2 Size = Geom::Vec2(600, 280));
			~DebugWindow() {};

			/**
				Handles the following events:
					Event			|		Data
				--------------------|-----------------
				VIEW_DBG_STRING		|	std::pair<string,string>
				EVT_FRAME			|	-
				TOGGLE_SHOW_DBGWIN	|	-
			*/
			void HandleEvent( Event& e);

		private:
			void CreateWindow(const Geom::Point& RelativePosition, const Geom::Vec2 Size);

			void EntryInput();

			void UpdateText(FilterLevel level = FilterLevel::VERBOSE);
			void AddLogText( std::string& newtext, int labelTextLimit );

			sfg::SharedPtr<sfg::Label> LogText;
			sfg::SharedPtr<sfg::Label> DbgText;
			sfg::SharedPtr<sfg::Label> DbgLabels;
			sfg::SharedPtr<sfg::Box>	LogBox;
			sfg::SharedPtr<sfg::Window> Win;
			sfg::SharedPtr<sfg::ScrolledWindow> scrolledwindow;
            sfg::SharedPtr<sfg::Entry> ConsoleInput;
			unsigned int currentlabeltext;
			unsigned int updateCounter;

			// a list of debugging strings which are set by event
			std::map<std::string, std::string> DebugStrings;
	};
} // namespace sbe
#endif // DGB_WINDOW_H
