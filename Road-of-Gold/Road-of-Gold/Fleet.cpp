#include"Fleet.h"
#include"Wallet.h"
#include"Node.h"
#include"Route.h"
#include"Urban.h"
#include"Vehicle.h"
#include"BiomeData.h"

Fleet::Fleet(int _ownVehicleType, Urban* _nowUrban)
	: name(Format(L"‘D’c"))
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

		Output << L"ˆÙí" << progress;
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

		Output << L"ˆÙí" << progress;
	}

	return Vec2(0, 0);
}
Wallet&	Fleet::wallet() const { return wallets[walletID]; }
int		Fleet::id() const { return int(this - &fleets.front()); }
bool	Fleet::canMoveTo(const Urban& _u) const
{
	if (nowUrban == &_u) return true;

	Array<bool>	urbanFlags(urbans.size(), false);
	auto f = [this](Route* r) {
		return r->isSeaRoute == data.isShip && r->movingCost < data.range;
	};

	urbanFlags[nowUrban->id()] = true;

	for (;;)
	{
		bool flag = true;

		for (auto& u : urbans)
		{
			if (urbanFlags[u.id()]) continue;

			for (auto* r : u.ownRoutes)
			{
				if (f(r) && urbanFlags[r->toUrban->id()])
				{
					if (&u == &_u) return true;
					urbanFlags[u.id()] = true;
					flag = false;
				}
			}
		}
		if (flag) break;
	}

	return false;
}
void	Fleet::addMoveTo(Urban* _from, Urban* _to)
{
	if (_to == _from) return;

	struct VirtualUrban
	{
		int		fromUrbanID;
		double	cost;

		VirtualUrban()
			: fromUrbanID(-1)
			, cost(0)
		{}
	};

	Array<VirtualUrban>	virtualUrbans(urbans.size());
	auto f = [this](Route* r) {
		return r->isSeaRoute == data.isShip && r->movingCost < data.range;
	};

	virtualUrbans[_to->id()].fromUrbanID = _to->id();

	for (;;)
	{
		bool flag = true;

		for (auto& u : urbans)
		{
			auto& vu = virtualUrbans[u.id()];
			if (vu.fromUrbanID == -1)
			{
				for (auto* r : u.ownRoutes)
				{
					if (f(r) && virtualUrbans[r->toUrban->id()].fromUrbanID != -1)
					{
						vu.fromUrbanID = r->toUrban->id();
						vu.cost = virtualUrbans[r->toUrban->id()].cost + r->movingCost;
						flag = false;
						break;
					}
				}
			}
			else
			{
				for (auto* r : u.ownRoutes)
				{
					if (f(r) && virtualUrbans[r->toUrban->id()].fromUrbanID != -1 && vu.cost > virtualUrbans[r->toUrban->id()].cost + r->movingCost)
					{
						vu.fromUrbanID = r->toUrban->id();
						vu.cost = virtualUrbans[r->toUrban->id()].cost + r->movingCost;
						flag = false;
					}
				}
			}
		}
		if (flag) break;
	}
	if (virtualUrbans[_from->id()].fromUrbanID != -1)
	{
		int now = _from->id();
		for (;;)
		{
			now = virtualUrbans[now].fromUrbanID;
			chain.rings.emplace_back(Code::Move, now);
			if (now == _to->id()) break;
		}
	}
}
void	Fleet::setMoveTo(const Urban& _u)
{
	chain.clear();
	if (nowUrban == &_u) return;

	struct VirtualUrban
	{
		int		fromUrbanID;
		double	cost;

		VirtualUrban()
			: fromUrbanID(-1)
			, cost(0)
		{}
	};

	Array<VirtualUrban>	virtualUrbans(urbans.size());
	auto f = [this](Route* r) {
		return r->isSeaRoute == data.isShip && r->movingCost < data.range;
	};

	Urban* startUrban = nowUrban;
	if (route != nullptr) startUrban = route->toUrban;
	chain.rings.emplace_back(Code::Move, startUrban->id());

	virtualUrbans[_u.id()].fromUrbanID = _u.id();

	for (;;)
	{
		bool flag = true;

		for (auto& u : urbans)
		{
			auto& vu = virtualUrbans[u.id()];
			if (vu.fromUrbanID == -1)
			{
				for (auto* r : u.ownRoutes)
				{
					if (f(r) && virtualUrbans[r->toUrban->id()].fromUrbanID != -1)
					{
						vu.fromUrbanID = r->toUrban->id();
						vu.cost = virtualUrbans[r->toUrban->id()].cost + r->movingCost;
						flag = false;
						break;
					}
				}
			}
			else
			{
				for (auto* r : u.ownRoutes)
				{
					if (f(r) && virtualUrbans[r->toUrban->id()].fromUrbanID != -1 && vu.cost > virtualUrbans[r->toUrban->id()].cost + r->movingCost)
					{
						vu.fromUrbanID = r->toUrban->id();
						vu.cost = virtualUrbans[r->toUrban->id()].cost + r->movingCost;
						flag = false;
					}
				}
			}
		}
		if (flag) break;
	}
	if (virtualUrbans[startUrban->id()].fromUrbanID != -1)
	{
		int now = startUrban->id();
		for (;;)
		{
			now = virtualUrbans[now].fromUrbanID;
			chain.rings.emplace_back(Code::Move, now);
			if (now == _u.id()) break;
		}
	}
}