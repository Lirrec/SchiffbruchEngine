#ifndef NAMEDLIST_H
#define NAMEDLIST_H

#include "Engine.hpp"

#include <memory>
#include <string>
#include <map>
#include <iostream>


/**
  * This is a simple templated list for stored named objects/resources in a map
  */
template< class stored_type >
class NamedList
{
	typedef std::shared_ptr<stored_type> ptr_type;
	typedef std::map<std::string, ptr_type > map_type;


public:

	/// return an item by name or an invalid pointer
	ptr_type GetItem(std::string name) const
	{
		auto elem = Items.find( name );

		if (elem != Items.end())
		{
			return elem->second;
		}

		Engine::out() << "NamedList, Item not found: '" << name << "';" << std::endl;

		// elem not found, return an invalid smart-pointer
		return ptr_type();
	}

	/// add an item by pointer
	void AddItem(std::string name, stored_type* t)
	{
		Items.insert(std::make_pair( name, ptr_type(t) ) );
	}

	/// add an item by reference
	void AddItem(std::string name, stored_type& t)
	{
		Items.insert(std::make_pair( name, ptr_type(t) ) );
	}

	/// add an item by copy
	void AddItem(std::string name, stored_type t)
	{
		Items.insert(std::make_pair( name, ptr_type( new stored_type(t) ) ) );
	}

	/// add an item by smart_pointer
	void AddItem(std::string name, ptr_type pt)
	{
		Items.insert(std::make_pair( name, pt ) );
	}

	/// remove an Item from the list. Item will be deleted if
	/// no other references exist.
	void RemoveItem(std::string name)
	{
		Items.erase(name);
	}

	/// Debug function that writes the names of the loaded items to stdout
	void DebugDump()
	{
		for ( auto it = Items.begin(); it != Items.end(); ++it)
		{
			Engine::out() << "Item: " << it->first << std::endl;
		}
	}

	private:

		map_type Items;

};


#endif // NAMEDLIST_H
