#include"Urban.h"
#include"Node.h"
#include"JSON.h"
#include"Route.h"
#include"GlobalVariables.h"

Array<Urban> urbans;
Urban* selectedUrban;

Circle	Urban::getShape() const { return Circle(nodes[joinedNodeID].pos.mPos, 0.010*(1 + numCitizens / 500)); }
void	Urban::draw() const
{
	const Circle circle = getShape();
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
			{
				if (cData[c.citizenType].needResourceID != -1)
					usedResource[cData[c.citizenType].needResourceID]++;
			}
			for (auto i : step(int(cData.size())))
			{
				if (cData[i].needResourceID == -1) jobEfficiency[i] = 1.0;
				else if (resource[cData[i].needResourceID] == 0) jobEfficiency[i] = 0.0;
				else if (resource[cData[i].needResourceID] < usedResource[cData[i].needResourceID]) jobEfficiency[i] = double(resource[cData[i].needResourceID]) / double(usedResource[cData[i].needResourceID]);
				else jobEfficiency[i] = 1.0;
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
	}

	//市民の更新
	for (auto& c : citizens)
		c.update();
}