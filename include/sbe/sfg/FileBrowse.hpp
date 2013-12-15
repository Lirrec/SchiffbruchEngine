#ifndef FILE_BROWSE_HPP
#define FILE_BROWSE_HPP

#include "sbe/event/EventUser.hpp"
#include "sbe/geom/Point.hpp"

#include <functional>


namespace sfg {
        class Window;
        class Entry;
}

#include <boost/filesystem/path.hpp>


namespace sbe
{
    class sfgList;


    /// FileBrowse
    class FileBrowse : public EventUser
    {
        public:
            FileBrowse( const Geom::Vec2 Size = Geom::Vec2(600, 400), const std::string &title="Select File/Folder",
                        const std::string &okEvent="FB_OK", const std::string &cancelEvent="FB_CANCEL");
            virtual ~FileBrowse() = default;

            virtual void HandleEvent( Event& e);

            void setTitle(const std::string &t);

            void setOkEvt(const std::string &e);
            void setCancelEvt(const std::string &e);

            void setOkAction(const std::function<void(const std::string&)> &f);
            void setCancelAction(const std::function<void()> &f);

            void show();
            void hide();

        private:
            void CreateWindow(const Geom::Vec2 Size, const std::string &title);
            void onOkClicked();
            void onCancelClicked();
            void updatePosition();
            void updateFolderList(const std::string &path);

            std::shared_ptr<sfg::Window> Win;
            std::shared_ptr<sfg::Entry> etyLoc;
            std::string okEvt;
            std::string cancelEvt;
            std::function<void(const std::string&)> okAction;
            std::function<void()> cancelAction;



            std::shared_ptr<sfgList> fldrLst;
            boost::filesystem::path lastPath;
    };
} // namespace sbe

#endif //FILE_BROWSE_HPP
