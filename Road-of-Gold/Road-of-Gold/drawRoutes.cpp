#include"Node.h"
#include"Path.h"
#include"TinyCamera.h"
#include"Route.h"
#include"Urban.h"
#include"Display.h"

void	drawRotue(const Route& _r, const Color& _color, double _width)
{
	LineString lineString;
	{
		auto* bn = &nodes[_r.fromUrban->joinedNodeID];
		lineString.emplace_back(bn->pos.mPos);
		for (int j = 0; j < int(_r.paths.size()); j++)
		{
			auto& an = nodes[_r.paths[j]->toID];

			lineString.emplace_back(an.pos.mPos);
			bn = &an;
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);
		lineString.draw(_width, _color);
	}
}