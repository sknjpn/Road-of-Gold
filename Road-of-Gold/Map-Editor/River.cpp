#include"River.h"
#include"Node.h"

Array<River> rivers;

void	River::draw() const
{
	Color color = Color(L"#0F285A");
	for (auto* p : riverPaths) p->getLine().stretched(-width / 2.0).draw(width, color);
	for (auto* p : riverPaths)
	{
		Circle(p->getLine().begin, width / 2.0).draw(color);
		Circle(p->getLine().end, width / 2.0).draw(color);
	}
}