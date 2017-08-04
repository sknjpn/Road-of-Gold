#include"Node.h"
#include"Route.h"
#include"Urban.h"

Array<Route> routes;
Route::Route(int _id)
	: id(_id)
	, destinationUrbanID(-1)
	, originUrbanID(-1)
	, totalCost(0)
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
		//Circle(p->getChildNode().pos.mPos, width / 2).draw(_color);
		p->getLine().stretched(-width / 2).draw(width, _color);
	}
}
void makeRoute()
{
	Array<Node*> temp;
	for (auto& u : urbans)
	{
		temp.emplace_back(&nodes[u.joinedNodeID]);
		nodes[u.joinedNodeID].isScaned = true;

		for (int i = 0; i < int(temp.size()); ++i)
		{
			auto& n1 = temp[i];

			if (n1->fromNodeID != -1 && !n1->isSea() && nodes[n1->fromNodeID].isSea()) continue;

			n1->isInQueue = false;
			for (auto& p : n1->paths)
			{
				auto& n2 = nodes[p.childNodeID];

				if (n1->ownUrbanID != u.id && n2.ownUrbanID == -1 && (n1->isSea() != n2.isSea())) continue;

				if (!n2.isScaned || n2.cost > n1->cost + p.cost)
				{
					if (!n2.isInQueue) temp.emplace_back(&n2);
					n2.isScaned = true;
					n2.isInQueue = true;
					n2.cost = n1->cost + p.cost;
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
							r.totalCost += p.cost;
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

		for (auto& n : temp)
		{
			n->cost = 0.0;
			n->isInQueue = false;
			n->isScaned = false;
			n->fromNodeID = -1;
		}
		temp.clear();
	}

	//“sŽs‚ÉRoute‚Ì“o˜^
	for (auto& r : routes)
		r.getOriginUrban().routeIDs.emplace_back(r.id);
}
Array<Route*>	Urban::getRoutesToUrban(int _urbanID, double _maxRange, bool _isSeaRoute) const
{
	const auto& ut = urbans[_urbanID];
	const double stopCost = 1 / 24.0;	//’“—¯Žž‚ÌƒRƒXƒg
	Array<Route*> rs;

	Array<Node*> temp;
	temp.emplace_back(&nodes[ut.joinedNodeID]);
	nodes[ut.joinedNodeID].isScaned = true;

	for (int i = 0; i < int(temp.size()); ++i)
	{
		auto& n1 = temp[i];

		if (n1->fromNodeID != -1 && !n1->isSea() && nodes[n1->fromNodeID].isSea()) continue;

		n1->isInQueue = false;

		for (auto& rID : urbans[n1->ownUrbanID].routeIDs)
		{
			auto& r = routes[rID];
			if (r.totalCost > _maxRange || r.isSeaRoute != _isSeaRoute) continue;

			auto& n2 = nodes[r.getDestinationUrban().joinedNodeID];

			if (!n2.isScaned || n2.cost > n1->cost + routes[rID].totalCost + stopCost)
			{
				if (!n2.isInQueue)  temp.emplace_back(&n2);
				n2.isScaned = true;
				n2.isInQueue = true;
				n2.cost = n1->cost + routes[rID].totalCost + stopCost;
				n2.fromNodeID = n1->id;
			}
		}
	}

	if (nodes[joinedNodeID].fromNodeID != -1)
	{
		auto* n = &nodes[joinedNodeID];

		for (;;)
		{
			for (auto& rID : urbans[n->ownUrbanID].routeIDs)
			{
				if (n->fromNodeID == routes[rID].getDestinationUrban().joinedNodeID)
				{
					rs.emplace_back(&routes[rID]);
					n = &nodes[n->fromNodeID];
					break;
				}
			}
			if (n->id == ut.joinedNodeID) break;
		}
	}

	for(auto& n : temp)
	{
		n->cost = 0.0;
		n->isInQueue = false;
		n->isScaned = false;
		n->fromNodeID = -1;
	}
	return rs;
}