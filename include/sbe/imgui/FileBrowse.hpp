#ifndef IMGUI_FILE_BROWSE_HPP
#define IMGUI_FILE_BROWSE_HPP

#include <sbe/event/EventUser.hpp>
#include <sbe/imgui/ImGuiWidget.hpp>
#include <sbe/geom/Point.hpp>

#include <functional>
#include <memory>
#include <string>

#include <boost/filesystem/path.hpp>

namespace sbe
{
	class ImGuiList;

	/**
		A file browser to select a directory or file (default is directory).
		Sends an Event with the filename and/or calls a given callback with the selected filename.
	*/
	class FileBrowse : public EventUser, public ImGuiWidget
	{
	public:
		enum class select_type
		{
			Directory, File
		} selection_type;

		FileBrowse(const glm::ivec2 Size = glm::ivec2(600, 400), const select_type t = select_type::Directory,
				   const std::string& title = "Select File/Folder",
				   const std::string& okEvent = "FB_OK", const std::string& cancelEvent = "FB_CANCEL");

		virtual ~FileBrowse();

		/** Set the file browser's window title */
		void setTitle(const std::string& t) { windowTitle = t; }

		/** Choose whether to select a file or a directory */
		void setSelectionType(select_type t) { selection_type = t; }

		/** Set the name of the event sent when OK is pressed (data: std::string path) */
		void setOkEvt(const std::string& e) { okEvt = e; }

		/** Set the name of the event sent when Cancel is pressed */
		void setCancelEvt(const std::string& e) { cancelEvt = e; }

		/** Set a callback invoked when OK is pressed */
		void setOkAction(const std::function<void(const std::string&)>& f) { okAction = f; }

		/** Set a callback invoked when Cancel is pressed */
		void setCancelAction(const std::function<void()>& f) { cancelAction = f; }

		/** Show the file browser window */
		void show();

		/** Hide the file browser window */
		void hide();

		void renderImGui() override;

	protected:
		void HandleEvent(Event& e) override;

	private:
		void onOkClicked();

		void onCancelClicked();

		void updateFolderList(const std::string& path);

		glm::ivec2 size;
		std::string windowTitle;
		bool visible;

		std::string currentPath;
		std::string errorText;

		std::string okEvt;
		std::string cancelEvt;
		std::function<void(const std::string&)> okAction;
		std::function<void()> cancelAction;

		std::shared_ptr<ImGuiList> fldrLst;
		boost::filesystem::path lastPath;
	};
} // namespace sbe

#endif // IMGUI_FILE_BROWSE_HPP
