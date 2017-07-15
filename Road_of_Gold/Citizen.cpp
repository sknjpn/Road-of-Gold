#include"Urban.h"
#include"GlobalVariables.h"
#include"CData.h"


Citizen::Citizen(int _id, int _citizenType, double _timer)
	: citizenType(_citizenType)
	, money(00)
	, timer(_timer)
	, id(_id)
	, price(100)
	, hapiness(0)
	, ths(0)
	, bhs(0)
{}
void	Citizen::update()
{
	/*
	timer += timeSpeed;
	if (timer >= 1.0)
	{
		timer -= 1.0;
		money -= 50;	//生活費の支払い
		auto& cJob = cData[citizenType].job;
		//仕事が達成可能かどうか判定
		int totalCost = cJob.cost - cJob.wage;
		bool flag = true;
		for (auto& p : cJob.consume)
		{
			if (u.baskets[p.itemID].getNumItem() < p.numConsume) { flag = false; break; }
			totalCost += u.baskets[p.itemID].getCost(p.numConsume);
		}
		//仕事の実行
		if (totalCost < money)
		{
			if (flag)
			{
				for (auto& p : cJob.product)
				{
					auto& b = u.baskets[p.itemID];
					const int price = 1 + int(price*Random(1.1, 1.2));
					b.addRing(price, p.numProduct, &c);
				}
				for (auto& p : cJob.consume)
					u.baskets[p.itemID].buyItem(p.numConsume);

				money -= totalCost;
			}

			//購買
			Array<Basket*> buyList;	//購買履歴
			for (;;)
			{
				Basket* best = NULL;
				double	earn = 0.0;
				for (int i = 0; i < int(iData.size()); i++)
				{
					auto& b = u.baskets[i];
					if (!b.rings.isEmpty() && !buyList.any([&b](const Basket* t) {return t == &b; }) && (best == NULL || iData[i].value / double(b.rings.front().price) > earn))
					{
						earn = iData[i].value / double(b.rings.front().price);
						best = &b;
					}
				}
				if (best != NULL && best->rings.front().price <= money) {
					money -= best->rings.front().price;
					buyList.emplace_back(best);
					best->buyItem(1);
				}
				else break;
			}
			hapiness = 0;
			for (auto& b : buyList)
			{
				hapiness += iData[b->itemType].value;
				ths += iData[b->itemType].value;
			}
		}
		else money += 100;	//労働者として働く
	}*/
}
