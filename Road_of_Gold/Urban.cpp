#include"Urban.h"
#include"Node.h"
#include"JSON.h"
#include"Route.h"
#include"GlobalVariables.h"

Array<Urban> urbans;
Urban* selectedUrban;

Urban::Urban(int _joinedNodeID)
	: id(int(urbans.size()))
	, name(UrbanName.choice())
	, joinedNodeID(_joinedNodeID)
	, timer(0.0)
	, day(0)
{
	resource.resize(rData.size());
	avgBhs.resize(cData.size());
}
Array<Route*>	Urban::getRoutes() const
{
	Array<Route*> rArray;
	for (auto& r : routeIDs)
		rArray.emplace_back(&routes[r]);
	return rArray;
}
String	Urban::getTimeAsString() const { return  Format(int(timer * 24)).lpad(2, '0') + L":" + Format(int(timer * 24 * 60) % 60).lpad(2, '0'); }
bool	setUrban(Node& _node)
{
	if (_node.isSea() || _node.ownUrbanID != -1) return false;
	for (const auto& p : _node.paths) if (p.getChildNode().ownUrbanID != -1) return false;
	_node.ownUrbanID = int(urbans.size());
	urbans.emplace_back(_node.id);
	auto& u = urbans.back();
	u.timer = 0.5 + u.getPos().mPos.x / TwoPi;

	const Array<int> numCitizen = {
		100,//労働者
		10,	//木こり
		10,	//陶芸職人
		20,	//狩人
		10,	//仕立て屋
		5,	//漁師
		5,	//漁師
		5,
		5,
	};

	for (int i = 0; i < int(iData.size()); i++) u.baskets.emplace_back(i, u.id);
	for (int i = 0; i < int(Min(cData.size(), numCitizen.size())); i++)
		for (int j = 0; j < numCitizen[i]; j++) u.citizens.emplace_back(int(u.citizens.size()), i, u.id);
	return true;
}
void	Urban::update()
{
	timer += timeSpeed;
	if (timer >= 1.0)
	{
		timer -= 1.0;
		day++;
		//一か月が経過
		if (day > 100)
		{
			day = 0;

			//BHSの更新
			avgBhs.fill(0);
			for (auto& c : citizens) { c.bhs = c.ths; c.ths = 0; avgBhs[c.citizenType] += c.bhs; }
			for (int i = 0; i<int(cData.size()); i++)
				avgBhs[i] = int(avgBhs[i] / citizens.count_if([&i](const Citizen& c) {return c.citizenType == i; }));
		}
		for (auto& b : baskets)
		{
			//価格低下
			for (auto& r : b.rings) r.price = Max(1, int(r.price*0.95));

			//チャートの更新
			b.chart.push_front(b.tradeLog.isEmpty() ? b.chart.front() : int(b.tradeLog.sum() / double(b.tradeLog.size())));
			b.tradeLog.clear();
			b.chart.pop_back();
		}
	}

	//市民の更新
	for (auto& c : citizens)
		c.update();
}