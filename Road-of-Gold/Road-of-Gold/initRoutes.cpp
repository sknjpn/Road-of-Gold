#include"Urban.h"
#include"Route.h"
#include"Node.h"
#include"Path.h"
#include"BiomeData.h"

void	initRoutes()
{
	struct VirtualNode
	{
		bool	isAdded;
		int		fromNodeID;
		double	cost;

		VirtualNode()
			: isAdded(false)
			, fromNodeID(-1)
			, cost(0)
		{}
	};

	Stopwatch	stopwatch(true);
	Array<int>	tempNodeIDs;
	Array<VirtualNode>	virtualNodes;

	virtualNodes.resize(nodes.size());

	for (int k = 0; k < 2; k++)
	{
		for (auto& u : urbans)
		{
			nodes[u.joinedNodeID].id();
			virtualNodes.fill(VirtualNode());
			tempNodeIDs.clear();
			tempNodeIDs.emplace_back(u.joinedNodeID);
			virtualNodes[u.joinedNodeID].cost = 1;

			for (int readID = 0; readID < int(tempNodeIDs.size()); readID++)
			{
				auto& n1 = nodes[tempNodeIDs[readID]];
				auto& vn1 = virtualNodes[tempNodeIDs[readID]];

				vn1.isAdded = false;

				for (auto& p : n1.paths)
				{
					auto& n2 = nodes[p.toID];
					auto& vn2 = virtualNodes[p.toID];

					//移動の制限
					if (k == 0)
					{
						if (!n2.isSea() && !n2.hasUrban()) continue;
					}
					else
					{
						if (n2.isSea()) continue;
					}

					double movingCost = p.length * (biomeData[n1.biomeType].movingCost + biomeData[n2.biomeType].movingCost) / 2.0;

					if (vn2.cost == 0 || vn2.cost > vn1.cost + movingCost)
					{
						vn2.cost = vn1.cost + movingCost;
						vn2.fromNodeID = tempNodeIDs[readID];
						if (!vn2.isAdded) tempNodeIDs.emplace_back(p.toID);
						vn2.isAdded = true;
					}
				}
			}

			//Routeの適用開始
			for (auto& t : urbans)
			{
				if (virtualNodes[t.joinedNodeID].cost != 0 && &t != &u)
				{
					Array<Path*> paths;
					int	nID = t.joinedNodeID;
					double movingCost = 0;
					for (;;)
					{
						if (virtualNodes[nID].fromNodeID == -1) break;	//目的地

						for (auto& p : nodes[nID].paths)
						{
							if (p.toID == virtualNodes[nID].fromNodeID)
							{
								movingCost += p.length * (biomeData[nodes[nID].biomeType].movingCost + biomeData[nodes[virtualNodes[nID].fromNodeID].biomeType].movingCost) / 2.0;
								paths.emplace_back(&p);
								break;
							}
						}
						nID = virtualNodes[nID].fromNodeID;
					}

					routes.emplace_back(&t, &u, paths, k == 0, movingCost);
				}
			}
		}
	}

	//Urbanに登録
	for (auto& u : urbans)
	{
		for (auto& r : routes)
		{
			if (r.fromUrban == &u) u.ownRoutes.emplace_back(&r);
		}
	}
	Output << L"ルート生成時間:" << stopwatch.ms() << L"ms";
}