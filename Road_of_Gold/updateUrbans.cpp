#include"Planet.h"
#include"Urban.h"
#include"UI.h"
#include"CitizenData.h"
#include"ItemData.h"
#include"Sound.h"

void	updateUrbans()
{
	for (auto& u : urbans)
	{
		if (u.sandglass.update())
		{
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
							else if (e.numEnergy >= numCitizen) je = 1;
							else je = (double)e.numEnergy / (double)numCitizen;
						}
					}
				}
			}

			//市民ログの更新
			for (auto& c : u.citizens)
			{
				c.incomeLog.push_front(c.wallet().income);
				c.incomeLog.pop_back();
				c.wallet().income = 0;	//収入をリセット
			}

			//市民の転職
			for (auto& c : u.citizens)
			{
				if (u.jobEfficiency[c.citizenType] == 0 || RandomBool(0.003))	//転職
				{
					int sum = 0;
					for (auto t : u.citizens) sum += t.avgIncome();

					sum = Random(sum);
					for (auto t : u.citizens)
					{
						sum -= t.avgIncome();
						if (sum < 0)
						{
							if (c.avgIncome() < t.avgIncome())
							{
								c.incomeLog.fill(0);
								c.citizenType = t.citizenType;

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
						}
					}
				}
			}

			//市場の更新
			for (auto i : step(int(u.baskets.size())))
			{
				auto& b = u.baskets[i];
				for (auto& r : b.rings) r.price = Max(1, int(r.price*0.95));	//価格低下

				//TradeLogの更新
				b.tradeLog.push();
			}


			//Sellerの更新
			for (auto& s : u.sellers)
			{
				int numSell = s.casket.numItem / s.timer;

				if (numSell > 0)
				{
					u.sellItem(s.casket.itemType, numSell, Max(1, int(1 + s.wallet().price*Random(1.00, 1.10))), s.walletID);
					s.casket.numItem -= numSell;
				}
				s.timer--;
			}
			u.sellers.remove_if([](const Seller& s) { return s.timer <= 0; });
		}

		//市民の更新
		for (auto& c : u.citizens)
		{
			const auto& je = u.jobEfficiency[c.citizenType];
			const auto& data = c.data();

			if (je == 0) c.jobProgress = 0;



			//if (c.wallet().sellCount == 0)
			c.jobProgress += je * planet.timeSpeed*c.jobEfficiency;

			if (c.jobProgress >= 1.0)
			{
				c.jobProgress -= 1.0;

				c.wallet().add(int(data.wage*u.productivity));

				//Itemの販売
				if (data.product.numItem > 0)
				{
					u.sellItem(data.product, Max(1, int(1 + c.wallet().price*Random(1.00, 1.10))), c.walletID);
				}

				//購買
				for (int i = 0; i < int(itemData.size()); i++)
				{
					const double h = Random(0.1,5.0);
					auto itemType = Random(int(itemData.size() - 1));
					if (!u.isSoldOut(itemType))
					{
						if (itemData[itemType].value / double(u.cost(itemType)) > h && u.cost(itemType) < c.wallet().money)
							u.buyItem(itemType, c.walletID);
					}
				}
				c.wallet().pull(c.wallet().money);

			}
		}
	}

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