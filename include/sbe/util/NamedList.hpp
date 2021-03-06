#ifndef NAMEDLIST_H
#define NAMEDLIST_H

#include <sbe/Engine.hpp>

#include <memory>
#include <string>
#include <map>
#include <iostream>
#include <vector>

namespace sbe
{
	/**
		Baseclass for NamedList.
		@see NamedList
	*/
	class BaseList
	{
	public:
		virtual ~BaseList() { }

		virtual void DebugDump() = 0;
	};

	/**
		A simple templated list for stored named objects/resources in a map.
		Provides basic getters and setters.
	  */
	template<class stored_type>
	class NamedList : public BaseList
	{
		typedef std::shared_ptr<stored_type> ptr_type;
		typedef std::map<std::string, ptr_type> map_type;


	public:

		/// return an item by name or an invalid pointer
		ptr_type GetItem(std::string name) const {
			auto elem = Items.find(name);

			if (elem != Items.end())
			{
				return elem->second;
			}

			//::sbe::Engine::out() << "NamedList, Item not found: '" << name << "';" << std::endl;

			// elem not found, return an invalid smart-pointer
			return ptr_type();
		}

		/// return all items containing the name
		std::vector<ptr_type> GetItems(std::string name) const {
			std::vector<ptr_type> re;
			for ( auto& elem : Items) {
				if (elem.first.find(name) != std::string::npos)
					re.push_back(elem.second);
			}
			return re;
		}

		/// add an item by pointer
		void AddItem(std::string name, stored_type* t) {
			Items.insert(std::make_pair(name, ptr_type(t)));
		}

		/// add an item by reference
		void AddItem(std::string name, stored_type& t) {
			Items.insert(std::make_pair(name, ptr_type(t)));
		}

		/// add an item by copy
		void AddItem(std::string name, stored_type t) {
			Items.insert(std::make_pair(name, ptr_type(new stored_type(t))));
		}

		/// add an item by smart_pointer
		void AddItem(std::string name, ptr_type pt) {
			Items.insert(std::make_pair(name, pt));
		}

		/// remove an Item from the list. Item will be deleted if
		/// no other references exist.
		void RemoveItem(std::string name) {
			Items.erase(name);
		}

		/// Debug function that writes the names of the loaded items to stdout
		virtual void DebugDump() {
			for (auto& elem : Items) {
				::sbe::Engine::out() << "- " << elem.first << std::endl;
			}
		}

		map_type& GetMap() { return Items; }

	private:

		map_type Items;

	};

} // namespace sbe
#endif // NAMEDLIST_H

