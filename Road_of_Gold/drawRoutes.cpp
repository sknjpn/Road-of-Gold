#include"Node.h"
#include"Path.h"
#include"TinyCamera.h"
#include"Route.h"
#include"Urban.h"

void	drawRotues()
{

	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		for (auto& r : routes)
		{
			auto* bn = &nodes[r.fromUrban->joinedNodeID];
			for (int j = 0; j < int(r.paths.size()); j++)
			{
				auto& an = nodes[r.paths[j]->toID];

				Line(bn->pos.mPos, an.pos.mPos).draw(0.001, Palette::Red);
				bn = &an;
			}
		}
	}
}