#ifndef QUADTREE_H
#define QUADTREE_H

#include <sbe/geom/RectHelpers.hpp>

#include <forward_list>

namespace sbe
{
	/**
		A simple quadtree intended for culling.
		It uses only objects centers for most checks, no bounding rectangles.
	*/
	template<class Item>
	class QuadTreeNode
	{
	public:

		/**
			Recursive creation of a quadtree
		*/
		QuadTreeNode(geom::irect _area);

		/**
			for late construction with init()
		*/
		QuadTreeNode();

		void init(geom::irect _area);


		~QuadTreeNode();

		/**
			Finds all Items in a rectangle.
			@param r the rectangle
			@param result a list containing all found items
		*/
		void FindItemsInRect(const geom::irect& r, std::forward_list<Item>& result);

		/**
			Insert a Item with the given center into the tree
			@param center the center of the item
			@param t the new item
		*/
		void InsertItem(const glm::ipoint2& center, Item t);

		/**
			Remove a Item from the tree, to avoid some comparisions, center must
			be given here too
			@param center the center of the item
			@param t the item itself
		*/
		void RemoveItem(glm::ipoint2& center, Item t);

		/**
			Defines the area spanned by this quadtree
		*/
		geom::irect area;

		/**
			Defines the minimal size of an individual node and thus the maximal subdivision granularity.
			Default value is 128.
		*/
		static void setMinNodeSize(int m) { MinNodeSize = m; }

	private:

		/// size smallest quadtree nodes, these are the bottom nodes
		static int MinNodeSize;

		bool bottomnode;
		union
		{
			std::forward_list<Item>* ItemsInside;
			QuadTreeNode* ChildNodes;
		};


	};

} // namespace sbe

#include "QuadTree.ipp"

#endif // QUADTREE_H

