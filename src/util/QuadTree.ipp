#include "sbe/util/QuadTree.hpp"

#include "sbe/Engine.hpp"
#include "sbe/Graphic.hpp"

template<class Type> int QuadTreeNode<Type>::MinNodeSize(128);


template < class Item >
QuadTreeNode<Item>::QuadTreeNode(Geom::Rect area)
{
	Engine::out() << "Creating QuadTree of Dimensions: " << area << std::endl;

	init(area);
}

template < class Item >
QuadTreeNode<Item>::QuadTreeNode()
{
	ChildNodes = nullptr;
	ItemsInside = nullptr;
}

template < class Item >
QuadTreeNode<Item>::~QuadTreeNode()
{
	if (ChildNodes != nullptr) delete[] ChildNodes;
	if (ItemsInside != nullptr) delete ItemsInside;
}

template < class Item >
void QuadTreeNode<Item>::init(Geom::Rect area)
{

	this->area = area;


	if (Geom::rcWidth(area) <= MinNodeSize  || Geom::rcHeight(area) <= MinNodeSize)
	{	// bottom
		bottomnode = true;
		ItemsInside = new std::forward_list<Item>();
	}
	else
	{	// subdivide
		bottomnode = false;

		ChildNodes = new QuadTreeNode[4];

//		Engine::out() << "# NODE: " << area << std::endl;
//		Engine::out() << "Child 1: " << Geom::rcTLQuad(area) << std::endl;
//		Engine::out() << "Child 2: " << Geom::rcTRQuad(area) << std::endl;
//		Engine::out() << "Child 3: " << Geom::rcBLQuad(area) << std::endl;
//		Engine::out() << "Child 4: " << Geom::rcBRQuad(area) << std::endl;
//		Engine::out() << "# -- #" << std::endl;
		ChildNodes[0].init(Geom::rcTLQuad(area));
		ChildNodes[1].init(Geom::rcTRQuad(area));
		ChildNodes[2].init(Geom::rcBRQuad(area));
		ChildNodes[3].init(Geom::rcBLQuad(area));
	}
}

/**
Adds all Items in a given rect to the given list
*/
template < class Item >
void QuadTreeNode<Item>::FindItemsInRect(const Geom::Rect& r, std::forward_list<Item>& result)
{

//	if (!Geom::rcOverlap(area, Geom::makeRect(0,0,1024,1024)))
//	{
//		Engine::out() << "ERROR: Node not in quadtree??" << std::endl;
//	}

	//Engine::Graphic()->AddDebugRect( area );

	if (bottomnode)
	{
		for (Item t : *ItemsInside)
		{
			result.push_front(t);
		}
	}
	else
	{
		int cs = 0;
		if (Geom::rcOverlap(r, ChildNodes[0].area  ))
		{
			ChildNodes[0].FindItemsInRect(r, result);
			++cs;
		}

		if (Geom::rcOverlap(r, ChildNodes[1].area  ))
		{
			ChildNodes[1].FindItemsInRect(r, result);
			++cs;
		}

		if (Geom::rcOverlap(r, ChildNodes[2].area  ))
		{
			ChildNodes[2].FindItemsInRect(r, result);
			++cs;
		}

		if (Geom::rcOverlap(r, ChildNodes[3].area  ))
		{
			ChildNodes[3].FindItemsInRect(r, result);
			++cs;
		}

		if (cs == 0)
		{
			//Engine::out() << "Searchrect doesn't overlap any child?? Should only occur on the rootnode" << std::endl;
		}

	}
}


/**

*/
template < class Item >
void QuadTreeNode<Item>::InsertItem(const Geom::Point& center, Item t)
{
	if (!t)
	{
		Engine::out() << "Got invalid Item to insert in QuadTree!!" << std::endl;
		return;
	}

	if (!Geom::PointInRect(area, center))
	{
		Engine::out() << "QTN:InsertItem: center " << center << " not in QT "<< area <<"!" << std::endl;
		abort();
	}



	if (bottomnode)
	{
		//Engine::out()<< "QTN: Inserted Item " << t->Position << " at: " << center << std::endl;;

		ItemsInside->push_front(t);
	}
	else
	{
		//ChildNodes[0].InsertItem(center, t);


		if (Geom::PointInRect(ChildNodes[0].area , center))
		{
			ChildNodes[0].InsertItem(center, t);
			return;
		}
		else
		if (Geom::PointInRect(ChildNodes[1].area , center))
		{
			ChildNodes[1].InsertItem(center, t);
			return;
		}
		else
		if (Geom::PointInRect(ChildNodes[2].area , center))
		{
			ChildNodes[2].InsertItem(center, t);
			return;
		}
		else
		if (Geom::PointInRect(ChildNodes[3].area , center))
		{
			ChildNodes[3].InsertItem(center, t);
			return;
		}
		else
		{

			Engine::out() << "ERROR" <<  area << " " << center <<std::endl;
			Engine::out() << "Child0 TL: " <<  ChildNodes[0].area << std::endl;
			Engine::out() << "Child1 TR: " <<  ChildNodes[1].area << std::endl;
			Engine::out() << "Child2 BR: " <<  ChildNodes[2].area << std::endl;
			Engine::out() << "Child3 BL: " <<  ChildNodes[3].area << std::endl;

			Engine::out() << Geom::PointInRect(Geom::makeRect(1024,0, 2048, 1024),  Geom::Point(1665,36)) << std::endl;

			abort();
		}
	}
}

template < class Item >
void QuadTreeNode<Item>::RemoveItem(Geom::Point& center, Item t)
{

	if (!t)
	{
		Engine::out() << "Got invalid Item to remove from  QuadTree!!" << std::endl;
		return;
	}

	if (!Geom::PointInRect(area, center))
	{
		Engine::out() << "QTN:RemoveItem: center " << center << " not in QT "<< area <<"!" << std::endl;
		return;
	}



	if (bottomnode)
	{
		//Engine::out()<< "QTN: Inserted Item " << t->Position << " at: " << center << std::endl;;

		ItemsInside->remove(t);
	}
	else
	{
		//ChildNodes[0].RemoveItem(center, t);


		if (Geom::PointInRect(ChildNodes[0].area , center))
		{
			ChildNodes[0].RemoveItem(center, t);
			return;
		}
		else
		if (Geom::PointInRect(ChildNodes[1].area , center))
		{
			ChildNodes[1].RemoveItem(center, t);
			return;
		}
		else
		if (Geom::PointInRect(ChildNodes[2].area , center))
		{
			ChildNodes[2].RemoveItem(center, t);
			return;
		}
		else
		if (Geom::PointInRect(ChildNodes[3].area , center))
		{
			ChildNodes[3].RemoveItem(center, t);
			return;
		}
		else
		{

			Engine::out() << "ERROR" <<  area << " " << center <<std::endl;
			Engine::out() << "Child0 TL: " <<  ChildNodes[0].area << std::endl;
			Engine::out() << "Child1 TR: " <<  ChildNodes[1].area << std::endl;
			Engine::out() << "Child2 BR: " <<  ChildNodes[2].area << std::endl;
			Engine::out() << "Child3 BL: " <<  ChildNodes[3].area << std::endl;
			Engine::out() << Geom::PointInRect(Geom::makeRect(1024,0, 2048, 1024),  Geom::Point(1665,36)) << std::endl;
		}
	}
}
