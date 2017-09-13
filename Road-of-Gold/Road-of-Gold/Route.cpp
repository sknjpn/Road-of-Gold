#include"Route.h"

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