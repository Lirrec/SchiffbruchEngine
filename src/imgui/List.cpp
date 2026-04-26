#include <sbe/imgui/List.hpp>

#include <sbe/event/Event.hpp>
#include <sbe/Module.hpp>

namespace sbe
{

	ImGuiList::ImGuiList(std::string ClickEventName)
			: selectedItems(0), EvtName(ClickEventName), MultiSelect(false) {
	}

	void ImGuiList::setClickEventName(const std::string& cEvtName) {
		EvtName = cEvtName;
	}

	void ImGuiList::addItem(std::string name) {
		for (auto& i : Items)
		{
			if (i->text == name)
			{
				Engine::out() << "[ImGuiList] Duplicate labels not allowed!" << std::endl;
				return;
			}
		}
		Items.push_back(std::make_shared<item>(name, false));
	}

	void ImGuiList::removeItem(std::string name) {
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->text == name)
			{
				Items.erase(it);
				break;
			}
		}
	}

	int ImGuiList::getIndex(std::string name) {
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->text == name)
				return std::distance(Items.begin(), it);
		}
		return -1;
	}

	void ImGuiList::clear() {
		Items.clear();
		selectedItems = 0;
	}

	void ImGuiList::select(size_t idx) {
		if (idx >= Items.size()) return;

		auto& it = Items[idx];

		if (it->active)
		{
			it->active = false;
			selectedItems--;
		}
		else
		{
			if (!MultiSelect)
			{
				for (auto& other : Items)
				{
					if (other->active)
					{
						other->active = false;
						selectedItems--;
					}
				}
			}

			it->active = true;
			selectedItems++;

			if (EvtName != "")
				Module::Get()->QueueEvent(Event(EvtName, it->text));
		}
	}

	std::string ImGuiList::getSelectedItem() {
		for (auto& i : Items)
		{
			if (i->active) return i->text;
		}
		return "";
	}

	std::vector<std::string> ImGuiList::getSelectedItems() {
		std::vector<std::string> result;
		for (auto& i : Items)
		{
			if (i->active) result.push_back(i->text);
		}
		return result;
	}

	void ImGuiList::LabelClicked(std::string Name) {
		for (auto& i : Items)
		{
			if (i->text == Name)
			{
				if (i->active)
				{
					i->active = false;
					selectedItems--;
				}
				else
				{
					if (!MultiSelect)
					{
						for (auto& other : Items)
						{
							if (other->active)
							{
								other->active = false;
								selectedItems--;
							}
						}
					}

					i->active = true;
					selectedItems++;

					if (EvtName != "")
						Module::Get()->QueueEvent(Event(EvtName, i->text));
				}
				break;
			}
		}
	}

	void ImGuiList::render(const char* id, ImVec2 size) {
		ImGui::BeginChild(id, size, ImGuiChildFlags_Borders);
		for (auto& i : Items)
		{
			if (ImGui::Selectable(i->text.c_str(), i->active))
				LabelClicked(i->text);
		}
		ImGui::EndChild();
	}

} // namespace sbe
