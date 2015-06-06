#ifndef SFGLIST_HPP
#define SFGLIST_HPP


namespace sfg
{
	class Widget;

	class Label;

	class Box;

	class ScrolledWindow;
}

#include <string>
#include <vector>
#include <memory>

namespace sbe
{
	/**
		A simple class managing a list of objects and allowing single or multiple select
	*/
	class sfgList
	{
	public:

		/**
			Construct a new List
			@param ClickEventName if not empty an event with the given name will be sent if an item is clicked ( data will be a string with the name of the item )
		*/
		sfgList(std::string ClickEventName = "");

		std::shared_ptr<sfg::Widget> getList();

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

		/// remove all Items
		void clear();

		/// select or deselect an item, sends "selected" event
		void select(size_t idx);

		/// return the selected item or the first selected item if multiselect is allowed
		/// @return the label of the item oder an empty string
		std::string getSelectedItem();

		/// return all selected items ( only makes sense in multi mode )
		std::vector<std::string> getSelectedItems();

		void LabelClicked(std::string Name);

	private:

		struct item
		{
			item(sfgList& l, std::string t, bool b, std::shared_ptr<sfg::Label> p)
					: text(t), active(b), label(p) { }

			std::string text;
			bool active;
			std::shared_ptr<sfg::Label> label;
		};

		std::vector<std::shared_ptr<item>> Items;
		unsigned int selectedItems;

		std::string EvtName;

		std::shared_ptr<sfg::ScrolledWindow> Frame;
		std::shared_ptr<sfg::Box> ItemBox;
		bool MultiSelect;
	};

} // namespace sbe
#endif // LIST_HPP
