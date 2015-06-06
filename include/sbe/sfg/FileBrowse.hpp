#ifndef FILE_BROWSE_HPP
#define FILE_BROWSE_HPP

#include "sbe/event/EventUser.hpp"
#include "sbe/geom/Point.hpp"

#include <functional>


namespace sfg
{
	class Window;

	class Entry;

	class Label;
}

#include <boost/filesystem/path.hpp>

namespace sbe
{
	class sfgList;


	/**
		A FileBrowses to select a directory or file ( default is directory ).
		Sends an Event with the filename and/or calls a given callback with the selected filename.
	*/
	class FileBrowse : public EventUser
	{
	public:
		enum class select_type
		{
			Directory, File
		} selection_type;

		FileBrowse(const glm::ivec2 Size = glm::ivec2(600, 400), const select_type t = select_type::Directory, const std::string& title = "Select File/Folder",
				   const std::string& okEvent = "FB_OK", const std::string& cancelEvent = "FB_CANCEL");

		virtual ~FileBrowse() = default;


		/** Set the FileBrowsers window title */
		void setTitle(const std::string& t);

		/** choose wether to select a file or a directory.
		*/
		void setSelectionType(select_type t) { selection_type = t; };

		/** Set the name of the event which is send when ok is pressed
			The Data of the Event will be std::string an contains the selected file/directory.
		*/
		void setOkEvt(const std::string& e);

		/** Set the name of the event which is send when cancel is pressed */
		void setCancelEvt(const std::string& e);

		/** Set a callback which is send when ok is pressed */
		void setOkAction(const std::function<void(const std::string&)>& f);

		/** Set a callback which is send when cancel is pressed */
		void setCancelAction(const std::function<void()>& f);

		/** Show the filebrowsers window */
		void show();

		/** hide the filebrowsers window */
		void hide();


	protected:
		void HandleEvent(Event& e) override;


	private:
		void CreateWin(const glm::ivec2 Size, const std::string& title);

		void onOkClicked();

		void onCancelClicked();

		void updatePosition();

		void updateFolderList(const std::string& path);

		std::shared_ptr<sfg::Window> Win;
		std::shared_ptr<sfg::Entry> etyLoc;
		std::shared_ptr<sfg::Label> errorLabel;
		std::string okEvt;
		std::string cancelEvt;
		std::function<void(const std::string&)> okAction;
		std::function<void()> cancelAction;


		std::shared_ptr<sfgList> fldrLst;
		boost::filesystem::path lastPath;
	};
} // namespace sbe

#endif //FILE_BROWSE_HPP
