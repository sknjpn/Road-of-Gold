#include"Urban.h"
#include"Node.h"
#include"JSON.h"
#include"Route.h"
#include"GlobalVariables.h"

Array<Urban> urbans;
Urban* selectedUrban;

Urban::Urban()
	: id(int(urbans.size()))
	, name(L"")
	, joinedNodeID(-1)
	, timer(0.5 + nodes[joinedNodeID].pos.mPos.x / TwoPi)
	, day(0)
{
	resource.resize(rData.size());
	jobEfficiency.resize(cData.size());
	avgIncome.resize(cData.size());

	const int numCitizens = 100;

	for (int i = 0; i < int(iData.size()); ++i) baskets.emplace_back(i, id);
	for (int i = 0; i < int(cData.size()); ++i)
		for (int j = 0; j < 3; ++j) citizens.emplace_back(int(citizens.size()), i, id);
	for (int i = 0; i < Max(0, int(numCitizens - cData.size() * 3)); ++i)
		citizens.emplace_back(int(citizens.size()), 0, id);
}
void	Urban::draw() const
{
	const Circle circle(nodes[joinedNodeID].pos.mPos, 0.015);
	const Color color = selectedUrban == this ? Palette::Yellow : (circle.mouseOver() ? Palette::Orange : Palette::Red);
	circle.draw(color).drawFrame(0.005, Palette::Black);
}
String	Urban::getTimeAsString() const { return  Format(int(timer * 24)).lpad(2, '0') + L":" + Format(int(timer * 24 * 60) % 60).lpad(2, '0'); }
void	Urban::update()
{
	timer += timeSpeed;
	if (timer >= 1.0)
	{
		timer -= 1.0;
		day++;

		//Efficiencyの更新
		{
			Array<int>	usedResource(rData.size());
			for (auto c : citizens)
				for (auto rID : cData[c.citizenType].job.needResourceID) usedResource[rID]++;

			for (auto i : step(int(cData.size())))
			{
				double efficiency = 1.0;
				for (auto j : cData[i].job.needResourceID)
				{
					if (resource[j] == 0) efficiency = 0.0;
					else if (resource[j] < usedResource[j]) efficiency *= (double(resource[j]) / double(usedResource[j]));
				}
				jobEfficiency[i] = efficiency;
			}
		}

		//AvgIncomeの修正
		for (auto i : step(int(cData.size())))
		{
			int num = 0;
			int sum = 0;
			for (auto& c : citizens)
			{
				if (c.citizenType == i)
				{
					num++;
					sum += c.avgIncome();
				}
			}
			if (num > 0) avgIncome[i] = sum / num;
		}
		for (auto& c : citizens)
		{
			c.incomeLog.push_front(0);
			c.incomeLog.pop_back();
		}

		//市場操作
		for (auto& b : baskets)
		{
			//価格低下
			for (auto& r : b.rings) r.price = Max(1, int(r.price*0.95));

			//チャートの更新
			b.chart.push_front(b.tradeLog.isEmpty() ? b.chart.front() : int(b.tradeLog.sum() / double(b.tradeLog.size())));
			b.tradeLog.clear();
			b.chart.pop_back();
		}
		for (auto i : step(int(cData.size())))
		{
			int sum = 0;
			int num = 0;
			for (auto& c : citizens)
			{
				if (c.citizenType == i)
				{
					num++;
					sum += c.money / 10;
					c.money -= c.money / 10;
				}
			}
			if (num > 0)
			{
				for (auto& c : citizens)
					if (c.citizenType == i)
						c.money += sum / num;
			}
		}
	}

	//市民の更新
	for (auto& c : citizens)
		c.update();
}