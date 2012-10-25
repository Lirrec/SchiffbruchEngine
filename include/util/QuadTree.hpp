#ifndef QUADTREE_H
#define QUADTREE_H

#include "Geom.hpp"

#include <forward_list>

/**
	A simple quadtree intended for culling.
*/
template< class Item >
class QuadTreeNode
{
	public:
		/**
		Recursive creation of a quadtree
		*/
		QuadTreeNode(Geom::Rect area);
		/**
		for late construction with init()
		*/
		QuadTreeNode();
		void init(Geom::Rect area);


		~QuadTreeNode();

		/**
		Adds all Items in a given rect to the given list
		*/
		void FindItemsInRect(const Geom::Rect& r, std::forward_list<Item>& result);

		/**
			Insert a Item with the given center into the tree
		*/
		void InsertItem(const Geom::Point& center, Item t);

		/**
			Remove a Item from the tree, to avoid some comparisions, center must
			be given here too
		*/
		void RemoveItem(Geom::Point& center, Item t);

		Geom::Rect area;

		static void setMinNodeSize(int m) {MinNodeSize = m; }

	private:

		/// size smallest quadtree nodes, these are the bottom nodes
		static int MinNodeSize;

		bool bottomnode;
		union {
			std::forward_list<Item>* ItemsInside;
			QuadTreeNode* ChildNodes;
		};


};

#include "QuadTree.cpp"

#endif // QUADTREE_H

