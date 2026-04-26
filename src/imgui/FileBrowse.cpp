#include <sbe/imgui/FileBrowse.hpp>

#include <sbe/Config.hpp>
#include <sbe/imgui/ImGuiCommon.hpp>
#include <sbe/gfx/Screen.hpp>
#include <sbe/Module.hpp>

#include <boost/filesystem.hpp>

#include "sbe/imgui/List.hpp"


namespace sbe
{

	FileBrowse::FileBrowse(const glm::ivec2 Size, const select_type t, const std::string& title,
						   const std::string& okEvent, const std::string& cancelEvent)
			: selection_type(t), size(Size), windowTitle(title), visible(false),
			  currentPath(boost::filesystem::current_path().string()),
			  okEvt(okEvent), cancelEvt(cancelEvent),
			  okAction([](const std::string&) {}), cancelAction([] {}) {
		RegisterForEvent("FB_SEL_CHANGED");

		fldrLst = std::make_shared<ImGuiList>("FB_SEL_CHANGED");

		updateFolderList(currentPath);

		Screen::get()->registerImGuiWidget(this);
	}

	FileBrowse::~FileBrowse() {
		Screen::get()->unregisterImGuiWidget(this);
	}

	void FileBrowse::HandleEvent(Event& e) {
		if (e.Is("FB_SEL_CHANGED", typeid(std::string)))
			updateFolderList(boost::any_cast<std::string>(e.Data()));
	}

	void FileBrowse::show() {
		visible = true;
	}

	void FileBrowse::hide() {
		errorText.clear();
		visible = false;
	}

	void FileBrowse::renderImGui() {
		if (!visible) return;

		ImGui::SetNextWindowSize({(float)size.x, (float)size.y}, ImGuiCond_Once);
		bool open = true;
		if (ImGui::Begin(windowTitle.c_str(), &open))
		{
			if (ImGui::InputText("Location", &currentPath, ImGuiInputTextFlags_EnterReturnsTrue))
				updateFolderList(currentPath);

			fldrLst->render("##files", {0, (float)size.y - 100});

			if (!errorText.empty())
				ImGui::TextColored({1, 0, 0, 1}, "%s", errorText.c_str());

			if (ImGui::Button("OK"))     onOkClicked();
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) onCancelClicked();
		}
		ImGui::End();

		if (!open) onCancelClicked();
	}

	void FileBrowse::onOkClicked() {
		using boost::filesystem::path;
		path file(currentPath);
		if (selection_type == select_type::File && !is_regular_file(file))
		{
			errorText = "Selected element is no file!";
			Engine::out() << "[FileBrowse] " << errorText << std::endl;
			return;
		}
		if (selection_type == select_type::Directory && !is_directory(file))
		{
			errorText = "Selected element is no directory!";
			Engine::out() << "[FileBrowse] " << errorText << std::endl;
			return;
		}

		okAction(currentPath);
		Module::Get()->QueueEvent(Event(okEvt, currentPath), true);
		hide();
	}

	void FileBrowse::onCancelClicked() {
		cancelAction();
		Module::Get()->QueueEvent(Event(cancelEvt));
		hide();
	}

	void FileBrowse::updateFolderList(const std::string& p) {
		using boost::filesystem::path;
		using boost::filesystem::directory_iterator;
		fldrLst->clear();

		path bp;

		if (p == "..")
			bp = lastPath.parent_path();
		else
		{
			bp = path(p);
			if (!bp.is_absolute())
				bp = lastPath / bp;
		}

		if (exists(bp) && is_directory(bp))
		{
			lastPath = bp;
			currentPath = bp.string();
			fldrLst->addItem("..");
			for (auto dit = directory_iterator(bp); dit != directory_iterator(); ++dit)
			{
				if (selection_type == select_type::File || is_directory(dit->path()))
					fldrLst->addItem(dit->path().filename().string());
			}
		}
		else if (selection_type == select_type::File)
		{
			lastPath = bp;
			currentPath = bp.string();
			fldrLst->addItem("..");
		}
	}

} // namespace sbe
