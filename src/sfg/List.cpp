#include <sbe/sfg/List.hpp>

#include <sbe/event/Event.hpp>
#include <sbe/Module.hpp>

#include <SFGUI/Widget.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Viewport.hpp>

namespace sbe
{

	sfgList::sfgList(std::string ClickEventName)
			: selectedItems(0), EvtName(ClickEventName), MultiSelect(false) {
		Frame = sfg::ScrolledWindow::Create();
		ItemBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

		Frame->AddWithViewport(ItemBox);
		Frame->SetRequisition(sf::Vector2f(100.f, 200.f));
		Frame->SetPlacement(sfg::ScrolledWindow::Placement::TOP_LEFT);
		Frame->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_AUTOMATIC | sfg::ScrolledWindow::HORIZONTAL_NEVER);
	}

	void sfgList::setClickEventName(const std::string& cEvtName) {
		EvtName = cEvtName;
	}

	std::shared_ptr<sfg::Widget> sfgList::getList() {
		return Frame;
	}

	void sfgList::addItem(std::string name) {
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->text == name)
			{
				Engine::out() << "[sfgList] Duplicate Labels not allowed!" << std::endl;
			}
		}

		sfg::Label::Ptr L = sfg::Label::Create(name);
		std::shared_ptr<sfgList::item> I(new sfgList::item(*this, name, false, L));
		L->GetSignal(sfg::Label::OnLeftClick).Connect([this, name]() { LabelClicked(name); });
		L->SetAlignment(sf::Vector2f(0, 0));
		ItemBox->Pack(L, false, false);
		Items.push_back(I);
	}

	void sfgList::removeItem(std::string name) {
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->text == name)
			{
				ItemBox->Remove((*it)->label);
				Items.erase(it);
				break;
			}
		}
	}

	int sfgList::getIndex(std::string name) {
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->text == name)
			{
				return std::distance(Items.begin(), it);
			}
		}
		return -1;
	}

	void sfgList::clear() {
		ItemBox->RemoveAll();
		Items.clear();
		selectedItems = 0;
	}

	void sfgList::select(size_t idx) {
		if (idx >= Items.size()) return;

		auto it = Items.begin() + idx;

		if ((*it)->active)
		{
			(*it)->active = false;
			(*it)->label->SetText((*it)->text);
			selectedItems--;
		}
		else
		{

			if (!MultiSelect)
			{
				// deselect others
				for (auto it = Items.begin(); it != Items.end(); ++it)
				{
					if ((*it)->active)
					{
						(*it)->active = false;
						(*it)->label->SetText((*it)->text);
						selectedItems--;
					}
				}
			}

			(*it)->active = true;
			(*it)->label->SetText("* " + (*it)->text);
			selectedItems++;

			if (EvtName != "")
			{
				Module::Get()->QueueEvent(Event(EvtName, (*it)->text));
			}
		}
	}

	std::string sfgList::getSelectedItem() {
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->active)
			{
				return (*it)->text;
			}
		}
		return "";
	}

	std::vector<std::string> sfgList::getSelectedItems() {
		std::vector<std::string> re;
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->active)
				re.push_back((*it)->text);
		}
		return re;
	}

	void sfgList::LabelClicked(std::string Name) {
		for (auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->text == Name)
			{
				if ((*it)->active)
				{
					(*it)->active = false;
					(*it)->label->SetText((*it)->text);
					selectedItems--;
				}
				else
				{

					if (!MultiSelect)
					{
						// deselect others
						for (auto it = Items.begin(); it != Items.end(); ++it)
						{
							if ((*it)->active)
							{
								(*it)->active = false;
								(*it)->label->SetText((*it)->text);
								selectedItems--;
							}
						}
					}

					(*it)->active = true;
					(*it)->label->SetText("* " + (*it)->text);
					selectedItems++;

					if (EvtName != "")
					{
						Module::Get()->QueueEvent(Event(EvtName, (*it)->text));
					}
				}
				break;
			}
		}
	}

} // namespace sbe
