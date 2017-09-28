#include"River.h"
#include"Node.h"

Array<River> rivers;

void	River::draw() const
{
	Color color = isMouseOver ? Palette::Red : Color(L"#0F285A");
	for (auto* p : riverPaths) p->getLine().stretched(-width / 2.0).draw(width, color);
	for (auto* p : riverPaths)
	{
		Circle(p->getLine().begin, width / 2.0).draw(color);
		Circle(p->getLine().end, width / 2.0).draw(color);
	}
}
River::River(const JSONValue _json)
	: width(_json[L"Width"].get<double>())
	, isMouseOver(false)
{
	Node* n = nullptr;
	for (auto j : _json[L"Paths"].arrayView())
	{
		if (n == nullptr) n = &nodes[j.get<int>()];
		else
		{
			for (auto& p : n->paths)
			{
				if (p.childNodeID == j.get<int>())
				{
					riverPaths.emplace_back(&p);
					n = &nodes[j.get<int>()];
					break;
				}
			}
		}
	}
}