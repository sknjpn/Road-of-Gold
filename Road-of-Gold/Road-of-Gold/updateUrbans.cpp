#include"Planet.h"
#include"Urban.h"
#include"UI.h"
#include"CitizenData.h"
#include"ItemData.h"
#include"Sound.h"
#include<thread>
#include<numeric>
#include<boost\range\numeric.hpp>

void	updateUrban(Urban& u)
{

	if (u.sandglass.update())
	{
		//市民ログの更新
		for (auto& c : u.citizens)
		{
			c.incomeLog.push_front(c.wallet().income);
			c.incomeLog.pop_back();
			c.avgIncome = int(boost::accumulate(c.incomeLog, 0, [](int sum, int i) { return sum + i; }) / c.incomeLog.size());
			c.wallet().income = 0;	//収入をリセット
		}

		//市民の転職
		for (auto& c : u.citizens)
		{
			if (u.jobEfficiency[c.citizenType] == 0 || RandomBool(0.003))	//転職
			{
				int sum = boost::accumulate(u.citizens, 0, [](int sum, const Citizen& c) {return sum + c.avgIncome; });
				//for (auto& t : u.citizens) sum += t.avgIncome;

				sum = Random(sum);
				for (auto& t : u.citizens)
				{
					sum -= t.avgIncome;
					if (sum < 0)
					{
						if (c.avgIncome < t.avgIncome)
						{
							c.incomeLog.fill(0);
							c.citizenType = t.citizenType;
							//目標利益を労働者の収益を元に設定
							c.targetRevenue = 0;// Random(int(citizenData.front().wage*u.productivity));

							//販売価格のコピー
							c.wallet().price = t.wallet().price;
						}
						break;
					}
				}
				for (int i = 0; i < int(citizenData.size()); i++)
				{
					if (u.jobEfficiency[i] != 0 && !u.citizens.any([&i](const Citizen& t) { return t.citizenType == i; }))
					{
						c.citizenType = i;
						//目標利益を労働者の収益を元に設定
						c.targetRevenue = 0;// Random(int(citizenData.front().wage*u.productivity));
					}
				}
			}
		}
		/*
		//目標利益の設定
		for (int i = 0; i < int(citizenData.size()); i++)
		{
			int sum = 0;
			for(auto&)
		}
		*/
		//効率の更新
		for (auto i : step(int(citizenData.size())))
		{
			auto& je = u.jobEfficiency[i];
			auto& data = citizenData[i];
			auto numCitizen = int(u.citizens.count_if([&i](const Citizen& c) { return c.citizenType == i; }));

			if (data.needEnergyType == -1) je = 1;
			else if (!u.energies.any([&data](const Energy& e) { return e.energyType == data.needEnergyType; })) je = 0;
			else
			{
				for (auto j : step(int(u.energies.size())))
				{
					auto& e = u.energies[j];
					if (e.energyType == data.needEnergyType)
					{
						if (e.numEnergy == 0) je = 0;
						else if (numCitizen == 0) je = 1.0;
						else je = Min(1.0, sqrt((double)e.numEnergy / (double)numCitizen)*u.productivity);
					}
				}
			}
		}


		//Sellerの更新
		for (auto& s : u.sellers)
		{
			if (s.progress < s.period) s.progress++;
			int numSell = s.casket.numItem - int(s.target*(1 - s.progress / double(s.period)));

			if (numSell > 0)
			{
				u.sellItem(s.casket.itemType, numSell, Max(1, int(1 + s.wallet().price*Random(1.00, 1.10))), s.walletID);
				s.casket.numItem -= numSell;
			}
		}

		//Buyerの更新
		for (auto& b : u.buyers)
		{
			if (b.progress < b.period) b.progress++;

			int numBuy = Min(u.numItem(b.casket.itemType), int(b.target*(b.progress / double(b.period))) - b.casket.numItem);
			if (numBuy <= 0) continue;

			u.buyItem(b.casket.itemType, b.walletID, numBuy);
			b.casket.numItem += numBuy;
		}
		u.sellers.remove_if([](const Seller& s) { return s.progress == s.period; });

		//Customerの更新
		int sum = std::accumulate(u.citizens.begin(), u.citizens.end(), 0, [](int _sum, Citizen& _c) { return _sum + _c.wallet().money; });
		for (auto& c : u.citizens) c.wallet().money = 0;
		for (auto& c : u.customers)
		{
			c.wallet().money = int(c.rate*sum);

			//購買
			for (int i = 0; i < int(itemData.size()); i++)
			{
				const double h = Random(0.1, 5.0);
				auto itemType = itemData.choice().id();

				if (!u.isSoldOut(itemType))
				{
					if (itemData[itemType].value / double(u.cost(itemType)) > h && u.cost(itemType) < c.wallet().money)
					{
						u.buyItem(itemType, c.walletID);
					}
				}
			}
			c.wallet().pull(c.wallet().money / 10);
		}
		//市場の更新
		for (auto i : step(int(u.baskets.size())))
		{
			auto& b = u.baskets[i];

			for (auto& r : b.rings) r.price = Max(1, int(r.price*0.95));	//価格低下

			b.tradeLog.push(); //TradeLogの更新
		}
	}

	//市民の更新
	for (auto& c : u.citizens)
	{
		const auto& je = u.jobEfficiency[c.citizenType];
		const auto& data = c.data();

		if (je == 0) c.jobProgress = 0;

		c.jobProgress += je*planet.timeSpeed;

		//生産
		while (c.jobProgress >= 1.0)
		{
			c.jobProgress -= 1.0;

			//賃金の加算
			c.wallet().add(data.wage);

			//Itemの販売
			if (data.product.numItem > 0)
			{
				//目標利益を達成できる場合のみ生産
				if (u.isSoldOut(data.product.itemType) || u.cost(data.product.itemType) > c.targetRevenue / data.product.numItem)
				{
					u.sellItem(data.product, Max(1, int(1 + c.wallet().price*Random(1.00, 1.10))), c.walletID);
				}
			}
		}
	}
}
void	updateUrbans()
{

	//マルチスレッドによる処理を行う
	{
		Array<std::thread> threads;

		for (auto& u : urbans) threads.emplace_back(updateUrban, std::ref(u));
		for (auto& t : threads)  t.join();
	}

	//Urbanの選択
	if (MouseL.down())
	{
		ui.selectedUrbanID = -1;

		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);

			for (auto j : step(int(urbans.size())))
			{
				if (urbans[j].mouseOver())
				{
					ui.selectedVehicleID = -1;
					ui.selectedUrbanID = j;
					sounds.at(0).play();
				}
			}
		}
	}
}