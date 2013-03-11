#include "sbe/sfg/List.hpp"

#include "sbe/event/Event.hpp"
#include "sbe/Module.hpp"
#include "sbe/Engine.hpp"

#include <functional>

#include <SFGUI/ViewPort.hpp>

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
				if ( selectedItems == 0 || MultiSelect)
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
						Event e(EvTName);
						e.SetData( (*it)->text );
						Module::Get()->QueueEvent(e);
					}
				}
			}
			break;
		}
	}
}

