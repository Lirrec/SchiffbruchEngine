#include "sbe/sfg/List.hpp"

#include "sbe/event/Event.hpp"
#include "sbe/Module.hpp"
#include "sbe/Engine.hpp"

#include <SFGUI/Widget.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Viewport.hpp>

#include <functional>

namespace sbe
{

	sfgList::sfgList(std::string ClickEventName)
	 : selectedItems(0), EvTName ( ClickEventName ), MultiSelect(false)
	{
		Frame = sfg::ScrolledWindow::Create();
		ItemBox = sfg::Box::Create(sfg::Box::VERTICAL);

		Frame->AddWithViewport( ItemBox );
		Frame->SetRequisition( sf::Vector2f( 100.f, 200.f ) );
		Frame->SetPlacement( sfg::ScrolledWindow::Placement::TOP_LEFT );
		Frame->SetScrollbarPolicy ( sfg::ScrolledWindow::VERTICAL_AUTOMATIC | sfg::ScrolledWindow::HORIZONTAL_NEVER );
	}

	sfg::SharedPtr<sfg::Widget> sfgList::getList()
	{
		return Frame;
	}

	void sfgList::addItem(std::string name)
	{
		for ( auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ( (*it)->text == name )
			{
				Engine::out() << "[sfgList] Duplicate Labels not allowed!" << std::endl;
			}
		}

		sfg::Label::Ptr L = sfg::Label::Create( name );
		std::shared_ptr<sfgList::item> I( new sfgList::item( *this, name, false, L ) );
		L->GetSignal( sfg::Label::OnLeftClick ).Connect( &item::click, I.get() );
		L->SetAlignment( sf::Vector2f( 0, 0 ) );
		ItemBox->Pack( L, false, false );
		Items.push_back(I);
	}

	void sfgList::removeItem(std::string name)
	{
		for ( auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ( (*it)->text == name )
			{
				ItemBox->Remove( (*it)->label );
				Items.erase(it);
				break;
			}
		}
	}

	void sfgList::clear()
	{
		ItemBox->RemoveAll();
		Items.clear();
		selectedItems = 0;
	}

	std::string sfgList::getSelectedItem()
	{
		for ( auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->active)
			{
				return (*it)->text;
			}
		}
		return "";
	}

	std::vector<std::string> sfgList::getSelectedItems()
	{
		std::vector<std::string> re;
		for ( auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ((*it)->active)
				re.push_back( (*it)->text );
		}
		return re;
	}

	void sfgList::LabelClicked(std::string Name)
	{
		for ( auto it = Items.begin(); it != Items.end(); ++it)
		{
			if ( (*it)->text == Name )
			{
				if ( (*it)->active )
				{
					(*it)->active = false;
					(*it)->label->SetText((*it)->text);
					selectedItems--;
				}
				else
				{

					if ( !MultiSelect )
					{
						// deselect others
						for ( auto it = Items.begin(); it != Items.end(); ++it)
						{
							if ( (*it)->active )
							{
								(*it)->active = false;
								(*it)->label->SetText((*it)->text);
								selectedItems--;
							}
						}
					}

					(*it)->active = true;
					(*it)->label->SetText( "* " + (*it)->text);
					selectedItems++;

					if (EvTName != "")
					{
						Module::Get()->QueueEvent( Event(EvTName, (*it)->text) );
					}
				}
				break;
			}
		}
	}

} // namespace sbe
