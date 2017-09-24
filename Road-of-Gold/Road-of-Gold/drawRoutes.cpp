#include"Node.h"
#include"Path.h"
#include"TinyCamera.h"
#include"Route.h"
#include"Urban.h"
#include"Display.h"

void	drawRotues()
{
	if (display.selectedUrban != nullptr)
	{
		for (auto* r : display.selectedUrban->ownRoutes) drawRotue(*r, Color(Palette::White, 128));

		if (display.selectedRoute != nullptr) drawRotue(*display.selectedRoute, Palette::Red);
	}
}
void	drawRotue(const Route& _r, const Color& _color)
{
	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);
		auto* bn = &nodes[_r.fromUrban->joinedNodeID];

		for (int j = 0; j < int(_r.paths.size()); j++)
		{
			auto& an = nodes[_r.paths[j]->toID];

			Line(bn->pos.mPos, an.pos.mPos).draw(0.002, _color);
			bn = &an;
		}
	}
}