#include"River.h"
#include"Node.h"
#include"BiomeData.h"


void	River::draw() const
{
	auto f = [](Node* parent, Path* p) {
		auto p1 = parent->pos.mPos;
		auto p2 = nodes[p->toID].pos.mPos;
		if (abs(p1.x - p2.x) > 180_deg)
		{
			if (p1.x > 0) p1.x -= 360_deg;
			else p2.x -= 360_deg;
		}
		return Line(p1, p2);
	};
	Color color = biomeData.front().color;
	for (int i = 0; i < int(riverNodes.size() - 1); i++)
	{
		auto* n1 = riverNodes[i];
		auto* n2 = riverNodes[i + 1];
		for (auto& p : n1->paths)
		{
			if (p.toID == n2->id())
			{
				f(n1, &p).stretched(-width / 2.0).draw(width, color);
			}
		}
	}
	for (auto* n : riverNodes)
	{
		Circle(n->pos.mPos, width / 2.0).draw(color);
	}
}
River::River(const JSONValue _json)
	: width(_json[U"Width"].get<double>())
{
	for (auto j : _json[U"Paths"].arrayView())
	{
		riverNodes.emplace_back(&nodes[j.get<int>()]);
	}
	for (int i = 0; i < int(riverNodes.size() - 1); i++)
	{
		auto* n1 = riverNodes[i];
		auto* n2 = riverNodes[i + 1];
		for (auto& p : n1->paths)
		{
			if (p.toID == n2->id()) p.riverWidth = Max(p.riverWidth, width);	//êÏïùÇÃìoò^
		}
		for (auto& p : n2->paths)
		{
			if (p.toID == n1->id()) p.riverWidth = Max(p.riverWidth, width);	//êÏïùÇÃìoò^
		}
	}
}