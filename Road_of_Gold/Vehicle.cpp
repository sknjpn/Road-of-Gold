#include"Vehicle.h"
#include"VehicleData.h"
#include"Wallet.h"
#include"Pos.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"BiomeData.h"

VehicleData&	Vehicle::data() const { return vehicleData.at(vehicleType); }


Vehicle::Vehicle(int _vehicleType, Urban* _nowUrban)
	: vehicleType(_vehicleType)
	, cargo()
	, walletID(getNewWalletID())
	, sleepTimer(0)
	, routeProgress(0)
	, nowUrban(_nowUrban)
	, reader(0)
	, route(nullptr)
	, timer(0)
	, period(0)
	, exportLog()
{
	maxVolume = data().volume;
}
Vec2	Vehicle::pos() const
{
	if (route == nullptr) return nodes[nowUrban->joinedNodeID].pos.mPos;
	else
	{
		double progress = routeProgress*data().speed;
		auto* bn = &nodes[route->fromUrban->joinedNodeID];
		for (int i = 0; i < int(route->paths.size()); i++)
		{
			auto* p = route->paths[i];
			auto* an = &nodes[route->paths[i]->toID];

			double movingCost = p->length * (biomeData[bn->biomeType].movingCost + biomeData[an->biomeType].movingCost) / 2.0;
			if (progress <= movingCost)
			{
				return bn->pos.mPos.lerp(an->pos.mPos, progress / movingCost);
			}
			else progress -= movingCost;
			bn = an;
		}
		LOG_ERROR(Format(L"àŸèÌ", progress));
	}
	return Vec2(0, 0);
}
bool	Vehicle::mouseOver() const
{
	return Circle(pos(), 0.005).mouseOver();
}
Wallet&	Vehicle::wallet() const
{
	return wallets[walletID];
}