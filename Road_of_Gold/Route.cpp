#include"Node.h"
#include"Route.h"
#include"Urban.h"

Node* nodeTemp[1000000];

Array<Route> routes;
Route::Route(int _id)
	: id(_id)
	, destinationUrbanID(-1)
	, originUrbanID(-1)
	, totalLength(0.0)
	, isSeaRoute(false)
{}
Urban&	Route::getDestinationUrban() const { return urbans[destinationUrbanID]; }
Urban&	Route::getOriginUrban() const { return urbans[originUrbanID]; }
void	Route::draw(const Color& _color) const
{
	const double width = isSeaRoute ? 0.01 : 0.005;
	for (const auto& pID : pathIDs)
	{
		const auto& p = paths[pID];
		Circle(p->getChildNode().pos.mPos, width / 2).draw(_color);
		p->getLine().stretched(-width / 2).draw(width, _color);
	}
}
void makeRoute()
{
	for (auto& n : nodeTemp) n = nullptr;
	routes.clear();
	for (auto& u : urbans)
	{
		int wPos = 0;
		nodeTemp[wPos] = &nodes[u.joinedNodeID]; wPos++;
		nodes[u.joinedNodeID].isScaned = true;

		for (int i = 0; i < wPos; i++)
		{
			auto& n1 = nodeTemp[i];

			if (n1->fromNodeID != -1 && !n1->isSea() && nodes[n1->fromNodeID].isSea()) continue;

			n1->isInQueue = false;
			for (auto& p : n1->paths)
			{
				auto& n2 = nodes[p.childNodeID];

				if (n1->ownUrbanID != u.id && n2.ownUrbanID == -1 && (n1->isSea() != n2.isSea())) continue;

				if (!n2.isScaned || n2.cost > n1->cost + p.length)
				{
					if (!n2.isInQueue) { nodeTemp[wPos] = &n2; wPos++; }
					n2.isScaned = true;
					n2.isInQueue = true;
					n2.cost = n1->cost + p.length;
					n2.fromNodeID = n1->id;
				}
			}
		}

		for (auto& ut : urbans)
		{
			if (&u != &ut && nodes[ut.joinedNodeID].fromNodeID != -1)
			{
				routes.push_back(int(routes.size()));
				auto& r = routes.back();

				auto* n = &nodes[ut.joinedNodeID];
				for (;;)
				{
					for (auto& p : n->paths)
					{
						if (n->fromNodeID == p.childNodeID)
						{
							r.pathIDs.push_back(p.id);
							r.totalLength += p.length;
							n = &nodes[n->fromNodeID];
							break;
						}
					}
					if (n->id == u.joinedNodeID) break;
				}

				r.destinationUrbanID = u.id;
				r.originUrbanID = ut.id;
				r.isSeaRoute = nodes[paths[r.pathIDs.front()]->childNodeID].isSea();
			}
		}

		for (int i = 0; i < wPos; i++)
		{
			auto& n = nodeTemp[i];
			n->cost = 0.0;
			n->isInQueue = false;
			n->isScaned = false;
			n->fromNodeID = -1;
		}
	}
}
Array<Route*>	Urban::getRoutesToUrban(int _urbanID) const
{
	auto& ut = urbans[_urbanID];
	for (auto& n : nodeTemp) n = nullptr;
	Array<Route*> rs;
	int wPos = 0;
	nodeTemp[wPos] = &nodes[ut.joinedNodeID]; wPos++;
	nodes[ut.joinedNodeID].isScaned = true;

	for (int i = 0; i < wPos; i++)
	{
		auto& n1 = nodeTemp[i];

		if (n1->fromNodeID != -1 && !n1->isSea() && nodes[n1->fromNodeID].isSea()) continue;

		n1->isInQueue = false;

		for (auto& r : urbans[n1->ownUrbanID].getRoutes())
		{
			auto& n2 = nodes[r->getDestinationUrban().joinedNodeID];

			if (!n2.isScaned || n2.cost > n1->cost + r->totalLength)
			{
				if (!n2.isInQueue) { nodeTemp[wPos] = &n2; wPos++; }
				n2.isScaned = true;
				n2.isInQueue = true;
				n2.cost = n1->cost + r->totalLength;
				n2.fromNodeID = n1->id;
			}
		}
	}

	if (nodes[joinedNodeID].fromNodeID != -1)
	{
		auto* n = &nodes[ut.joinedNodeID];
		for (;;)
		{
			for (auto& r : urbans[n->ownUrbanID].getRoutes())
			{
				if (n->fromNodeID == r->getDestinationUrban().joinedNodeID)
				{
					rs.emplace_back(r);
					n = &nodes[n->fromNodeID];
					break;
				}
			}
			if (n->id == ut.joinedNodeID) break;
		}
	}

	for (int i = 0; i < wPos; i++)
	{
		auto& n = nodeTemp[i];
		n->cost = 0.0;
		n->isInQueue = false;
		n->isScaned = false;
		n->fromNodeID = -1;
	}
}