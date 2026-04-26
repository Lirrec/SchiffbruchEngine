#ifndef IMGUILIST_HPP
#define IMGUILIST_HPP

#include "ImGuiCommon.hpp"

#include <string>
#include <vector>
#include <memory>

namespace sbe
{
	/**
		A simple class managing a list of items and allowing single or multiple select.
		Not an ImGuiWidget itself — call render() inline from within your own ImGui window.
	*/
	class ImGuiList
	{
	public:

		/**
			Construct a new List.
			@param ClickEventName if not empty an event with the given name will be sent when an item is clicked
			                      (data will be a std::string with the name of the item)
		*/
		ImGuiList(std::string ClickEventName = "");

		/// change the event sent on click
		void setClickEventName(const std::string& cEvtName);

		/// set single or multiple selects
		void setMultiSelect(bool multi) { MultiSelect = multi; }

		bool isMultiSelect() { return MultiSelect; }

		/// add an item to the list
		void addItem(std::string name);

		/// remove an item from the list
		void removeItem(std::string name);

		int getIndex(std::string name);

		/// remove all items
		void clear();

		/// select or deselect an item, sends click event
		void select(size_t idx);

		/// return the selected item or the first selected item if multiselect is allowed
		/// @return the label of the item or an empty string
		std::string getSelectedItem();

		/// return all selected items (only makes sense in multi mode)
		std::vector<std::string> getSelectedItems();

		void LabelClicked(std::string Name);

		/**
			Render the list as an ImGui child window.
			Call this inside an active ImGui::Begin/End block.
			@param id  ImGui child window id (should be unique within the parent window)
			@param size  desired size; pass {0,0} to auto-fit
		*/
		void render(const char* id, ImVec2 size = {0, 0});

	private:

		struct item
		{
			item(std::string t, bool b) : text(t), active(b) { }

			std::string text;
			bool active;
		};

		std::vector<std::shared_ptr<item>> Items;
		unsigned int selectedItems;

		std::string EvtName;
		bool MultiSelect;
	};

} // namespace sbe
#endif // IMGUILIST_HPP
