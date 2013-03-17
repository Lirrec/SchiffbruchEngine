#ifndef SFGLIST_HPP
#define SFGLIST_HPP


#include <SFGUI/SharedPtr.hpp>
namespace sfg {
		class Widget;
		class Label;
		class Box;
		class ScrolledWindow;
}

#include <string>
#include <vector>
#include <memory>

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
	sfgList ( std::string ClickEventName = "" );

	sfg::SharedPtr<sfg::Widget> getList();

	/// set single or multiple selects
	void setMultiSelect( bool multi ) { MultiSelect = multi; }
	bool isMultiSelect() { return MultiSelect; }
	/// add an item to the list
	void addItem( std::string name );
	/// remove an item from the list
	void removeItem( std::string name);

	/// return the selected item or the first selected item if multiselect is allowed
	std::string getSelectedItem();
	/// return all selected items ( only makes sense in multi mode )
	std::vector<std::string> getSelectedItems();

	void LabelClicked( std::string Name );

private:

	struct item
	{
		item ( sfgList& l, std::string t, bool b, sfg::SharedPtr<sfg::Label> p )
		: text(t), active(b), list(l), label(p) {}
		std::string text;
		bool active;
		sfg::SharedPtr<sfg::Label> label;
		sfgList& list;
		void click() { list.LabelClicked(text); }
	};

	std::vector<std::shared_ptr<item>> Items;
	unsigned int selectedItems;

	std::string EvTName;

	sfg::SharedPtr<sfg::ScrolledWindow> Frame;
	sfg::SharedPtr<sfg::Box> ItemBox;
	bool MultiSelect;
};

#endif // LIST_HPP

