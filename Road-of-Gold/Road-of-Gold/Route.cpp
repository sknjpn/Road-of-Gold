#include"Route.h"
#include"TinyCamera.h"
#include"Node.h"
#include"Urban.h"

void	Route::addVehicle()
{
	//逆ルート分
	for (auto& r : routes)
	{
		if (r.fromUrban == toUrban && r.toUrban == fromUrban && r.isSeaRoute == isSeaRoute)
		{
			r.numVehicles++;
		}
	}
	numVehicles++;
}
void	Route::pullVehicle()
{
	//逆ルート分
	for (auto& r : routes)
	{
		if (r.fromUrban == toUrban && r.toUrban == fromUrban && r.isSeaRoute == isSeaRoute)
		{
			r.numVehicles--;
		}
	}
	numVehicles--;
}
void	Route::draw()
{
	const double	width = 0.005;
	const Color		color = Palette::Red;

	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		auto* bn = &nodes[fromUrban->joinedNodeID];
		for (int j = 0; j < int(paths.size()); j++)
		{
			auto& an = nodes[paths[j]->toID];

			getLine(an.pos, bn->pos).stretched(-width / 2.0).draw(width, Palette::Red);
			Circle(an.pos.mPos, width / 2.0).draw(Palette::Red);

			bn = &an;
		}
	}
}