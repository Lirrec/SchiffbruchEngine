//
// Created by gfwl on 07.12.2015.
//

#ifndef SCHIFFBRUCHENGINE_META_H
#define SCHIFFBRUCHENGINE_META_H

#include <tuple>
#include <utility>

namespace sbe {

	/**
	 * Taken from http://en.cppreference.com/w/cpp/utility/integer_sequence
	 */

	template<typename Func, typename Tup, std::size_t... index>
	decltype(auto) invoke_helper(Func&& func, Tup&& tup, std::index_sequence<index...>)
	{
		return func(std::get<index>(std::forward<Tup>(tup))...);
	}

	template<typename Func, typename Tup>
	decltype(auto) invoke(Func&& func, Tup&& tup)
	{
		constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
		return invoke_helper(std::forward<Func>(func),
		                     std::forward<Tup>(tup),
		                     std::make_index_sequence<Size>{});
	}

	template<typename Base, typename Func, typename Tup, std::size_t... index>
	decltype(auto) invoke_member_helper(Base&& base, Func&& func, Tup&& tup, std::index_sequence<index...>)
	{
		return (base->*func)(std::get<index>(std::forward<Tup>(tup))...);
	}

	template<typename Base, typename Func, typename Tup, std::size_t... index>
	decltype(auto) invoke_member_helper(Base&& base, Func&& func, Tup&& tup, std::index_sequence<index...>, typename std::enable_if_t<!std::is_member_function_pointer<Func>::value>)
	{
		return (base.*func)(std::get<index>(std::forward<Tup>(tup))...);
	}

	template<typename Base, typename Func, typename Tup>
	decltype(auto) invoke_member(Base&& base, Func&& func, Tup&& tup)
	{
		constexpr auto Size = std::tuple_size<typename std::decay<Tup>::type>::value;
		return invoke_member_helper(std::forward<Base>(base),
							 std::forward<Func>(func),
		                     std::forward<Tup>(tup),
		                     std::make_index_sequence<Size>{});
	}



	/**
	 * http://stackoverflow.com/questions/12742877/remove-reference-from-stdtuple-members
	 */
	template <typename... T>
	using tuple_with_removed_refs = std::tuple<typename std::remove_reference<T>::type...>;
}

#endif //SCHIFFBRUCHENGINE_META_H
