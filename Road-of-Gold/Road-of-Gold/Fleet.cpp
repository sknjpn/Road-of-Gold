#include"Fleet.h"
#include"Wallet.h"
#include"Node.h"
#include"Route.h"
#include"Urban.h"
#include"Vehicle.h"
#include"BiomeData.h"

Fleet::Fleet(int _ownVehicleType, Urban* _nowUrban)
	: name(Format(L"‘æ", id() + 1, L"‘D’c"))
	, chain()
	, walletID(getNewWalletID())
	, sleepTimer(0)
	, routeProgress(0)
	, nowUrban(_nowUrban)
	, route(nullptr)
	, timer(0)
	, period(0)
	, exportLog()
	, stopFlag(false)
	, planFixed(false)
	, data()
{
	wallet().owner = Owner::Vehicle;
	vehicles.emplace_back(_ownVehicleType);
	add(&vehicles.back());
}
void	Fleet::add(Vehicle* _v)
{
	ownVehicles.emplace_back(_v);
	data.range = _v->data().range;
	data.speed = _v->data().speed;
	data.volume = 0;
	data.isShip = _v->data().isShip;
	for (auto* v : ownVehicles)
	{
		data.range = Min(data.range, v->data().range);
		data.speed = Min(data.speed, v->data().speed);
		data.volume += v->data().volume;
	}
}
double	Fleet::angle() const
{
	if (route == nullptr) return 0.0;
	else
	{
		double progress = routeProgress*data.speed;
		auto* bn = &nodes[route->fromUrban->joinedNodeID];

		for (int i = 0; i < int(route->paths.size()); i++)
		{
			auto* p = route->paths[i];
			auto* an = &nodes[route->paths[i]->toID];
			double movingCost = p->length * (biomeData[bn->biomeType].movingCost + biomeData[an->biomeType].movingCost) / 2.0;

			if (progress <= movingCost)
			{
				auto angle = (an->pos.mPos - bn->pos.mPos).normalized();
				return atan2(angle.y, angle.x);
			}
			else progress -= movingCost;

			bn = an;
		}

		Output << L"ˆÙí", progress;
	}

	return 0.0;
}

Vec2	Fleet::pos() const
{
	if (route == nullptr) return nodes[nowUrban->joinedNodeID].pos.mPos;
	else
	{
		double progress = routeProgress*data.speed;
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

		Output << L"ˆÙí", progress;
	}

	return Vec2(0, 0);
}

bool	Fleet::mouseOver() const
{
	return Circle(pos(), 0.005).mouseOver();
}

Wallet&	Fleet::wallet() const { return wallets[walletID]; }
int		Fleet::id() const { return int(this - &fleets.front()); }