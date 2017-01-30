#include <sbe/util/QuadTree.hpp>

namespace sbe
{

	template<class Type>
	int QuadTreeNode<Type>::MinNodeSize(

	128);


	template<class Item>
	QuadTreeNode<Item>::QuadTreeNode(geom::irect _area) {
		Engine::out(Engine::SPAM) << "Creating QuadTree of Dimensions: " << area << std::endl;

		init(_area);
	}

	template<class Item>
	QuadTreeNode<Item>::QuadTreeNode() {
		ChildNodes = nullptr;
		ItemsInside = nullptr;
	}

	template<class Item>
	QuadTreeNode<Item>::~QuadTreeNode() {
		if (ChildNodes != nullptr) delete[] ChildNodes;
		if (ItemsInside != nullptr) delete ItemsInside;
	}

	template<class Item>
	void QuadTreeNode<Item>::init(geom::irect _area) {

		this->area = _area;


		if (geom::rcWidth(area) <= MinNodeSize || geom::rcHeight(area) <= MinNodeSize)
		{    // bottom
			bottomnode = true;
			ItemsInside = new std::forward_list<Item>();
		}
		else
		{    // subdivide
			bottomnode = false;

			ChildNodes = new QuadTreeNode[4];

			//		Engine::out() << "# NODE: " << area << std::endl;
			//		Engine::out() << "Child 1: " << geom::rcTLQuad(area) << std::endl;
			//		Engine::out() << "Child 2: " << geom::rcTRQuad(area) << std::endl;
			//		Engine::out() << "Child 3: " << geom::rcBLQuad(area) << std::endl;
			//		Engine::out() << "Child 4: " << geom::rcBRQuad(area) << std::endl;
			//		Engine::out() << "# -- #" << std::endl;
			ChildNodes[0].init(geom::rcTLQuad(area));
			ChildNodes[1].init(geom::rcTRQuad(area));
			ChildNodes[2].init(geom::rcBRQuad(area));
			ChildNodes[3].init(geom::rcBLQuad(area));
		}
	}

	/**
	Adds all Items in a given rect to the given list
	*/
	template<class Item>
	void QuadTreeNode<Item>::FindItemsInRect(const geom::irect& r, std::forward_list<Item>& result) {

		//	if (!geom::rcOverlap(area, geom::makeRect(0,0,1024,1024)))
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
			if (geom::rcOverlap(r, ChildNodes[0].area))
			{
				ChildNodes[0].FindItemsInRect(r, result);
				++cs;
			}

			if (geom::rcOverlap(r, ChildNodes[1].area))
			{
				ChildNodes[1].FindItemsInRect(r, result);
				++cs;
			}

			if (geom::rcOverlap(r, ChildNodes[2].area))
			{
				ChildNodes[2].FindItemsInRect(r, result);
				++cs;
			}

			if (geom::rcOverlap(r, ChildNodes[3].area))
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
	template<class Item>
	void QuadTreeNode<Item>::InsertItem(const glm::ipoint2& center, Item t) {
		if (!t)
		{
			Engine::out(Engine::ERROR) << "Got invalid Item to insert in QuadTree!!" << std::endl;
			return;
		}

		if (!geom::PointInRect(area, center))
		{
			Engine::out(Engine::ERROR) << "QTN:InsertItem: center " << center << " not in QT " << area << "!" << std::endl;
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


			if (geom::PointInRect(ChildNodes[0].area, center))
			{
				ChildNodes[0].InsertItem(center, t);
				return;
			}
			else if (geom::PointInRect(ChildNodes[1].area, center))
			{
				ChildNodes[1].InsertItem(center, t);
				return;
			}
			else if (geom::PointInRect(ChildNodes[2].area, center))
			{
				ChildNodes[2].InsertItem(center, t);
				return;
			}
			else if (geom::PointInRect(ChildNodes[3].area, center))
			{
				ChildNodes[3].InsertItem(center, t);
				return;
			}
			else
			{

				Engine::out(Engine::ERROR) << "ERROR" << area << " " << center << std::endl;
				Engine::out(Engine::ERROR) << "Child0 TL: " << ChildNodes[0].area << std::endl;
				Engine::out(Engine::ERROR) << "Child1 TR: " << ChildNodes[1].area << std::endl;
				Engine::out(Engine::ERROR) << "Child2 BR: " << ChildNodes[2].area << std::endl;
				Engine::out(Engine::ERROR) << "Child3 BL: " << ChildNodes[3].area << std::endl;

				Engine::out(Engine::ERROR) << geom::PointInRect(geom::makeRect(1024, 0, 2048, 1024), glm::ipoint2(1665, 36)) << std::endl;

				abort();
			}
		}
	}

	template<class Item>
	void QuadTreeNode<Item>::RemoveItem(glm::ipoint2& center, Item t) {

		if (!t)
		{
			Engine::out(Engine::ERROR) << "Got invalid Item to remove from  QuadTree!!" << std::endl;
			return;
		}

		if (!geom::PointInRect(area, center))
		{
			Engine::out(Engine::ERROR) << "QTN:RemoveItem: center " << center << " not in QT " << area << "!" << std::endl;
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


			if (geom::PointInRect(ChildNodes[0].area, center))
			{
				ChildNodes[0].RemoveItem(center, t);
				return;
			}
			else if (geom::PointInRect(ChildNodes[1].area, center))
			{
				ChildNodes[1].RemoveItem(center, t);
				return;
			}
			else if (geom::PointInRect(ChildNodes[2].area, center))
			{
				ChildNodes[2].RemoveItem(center, t);
				return;
			}
			else if (geom::PointInRect(ChildNodes[3].area, center))
			{
				ChildNodes[3].RemoveItem(center, t);
				return;
			}
			else
			{

				Engine::out(Engine::ERROR) << "ERROR" << area << " " << center << std::endl;
				Engine::out(Engine::ERROR) << "Child0 TL: " << ChildNodes[0].area << std::endl;
				Engine::out(Engine::ERROR) << "Child1 TR: " << ChildNodes[1].area << std::endl;
				Engine::out(Engine::ERROR) << "Child2 BR: " << ChildNodes[2].area << std::endl;
				Engine::out(Engine::ERROR) << "Child3 BL: " << ChildNodes[3].area << std::endl;
				Engine::out(Engine::ERROR) << geom::PointInRect(geom::makeRect(1024, 0, 2048, 1024), glm::ipoint2(1665, 36)) << std::endl;
			}
		}
	}

} // namespace sbe
