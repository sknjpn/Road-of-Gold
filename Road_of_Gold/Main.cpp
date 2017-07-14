#include "Planet.h"
#include "Node.h"
#include "Pi.h"
#include "Urban.h"
#include "Route.h"
#include "CData.h"
#include "Group.h"

Planet planet;

void Main()
{
	Window::Resize(1280, 720);
	Window::SetTitle(L"Road of Gold");

	const Font font12(12, Typeface::Bold);
	const Font font16(16);
	const Font font24(24);
	const Font font36(36);
	const Font font48(48);

	enum struct DrawingType
	{
		Market, Towner, News, Vehicle
	}
	drawingType = DrawingType::Market;

	double timeSpeed = 0.01;
	double worldTimer = 0.0;
	int selectedBasket = 0;
	int selectedCitizen = 0;

	loadEconomicData();

	planet.makeNewWorld();
	loadNodeMap(L"authcode.bin");
	setPlanetToNodes();

	//Urbanの生成
	auto numUrbans = int(nodes.count_if([](const auto& n) {return !n.isSea; })) / 100;
	for (auto& r : regions)
	{
		if (r.numNodes == 0) continue;
		for (;;)
		{
			auto& n = nodes[Random(int(nodes.size() - 1))];
			if (n.joinedRegionID == r.id && n.isCoast() && setUrban(n)) break;
		}
	}
	numUrbans -= int(regions.size());
	while (numUrbans > 0)
		if (setUrban(nodes[Random(int(nodes.size() - 1))])) numUrbans--;

	makeRoute();

	makeGroupsRandom();

	//planet.makeVoronoiMap();

	while (System::Update())
	{
		if (KeyF1.down()) timeSpeed = Max(0.0001, timeSpeed*0.5);
		if (KeyF2.down()) timeSpeed = Min(1.0000, timeSpeed*2.0);
		worldTimer = fmod(worldTimer + timeSpeed, 1.0);


		//UrbanUpdate
		for (auto& u : urbans)
		{
			u.timer += timeSpeed;
			if (u.timer >= 1.0)
			{
				u.timer -= 1.0;
				for (auto& b : u.baskets)
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
			for (auto& c : u.citizens)
			{
				c.timer += timeSpeed;
				if (c.timer >= 1.0)
				{
					c.timer -= 1.0;
					c.money -= 50;	//生活費の支払い
					auto& cJob = cData[c.citizenType].job;
					//仕事が達成可能かどうか判定
					int totalCost = cJob.cost - cJob.wage;
					bool flag = true;
					for (auto& p : cJob.consume)
					{
						if (u.baskets[p.itemID].getNumItem() < p.numConsume) { flag = false; break; }
						totalCost += u.baskets[p.itemID].getCost(p.numConsume);
					}
					//仕事の実行
					if (totalCost < c.money)
					{
						if (flag)
						{
							for (auto& p : cJob.product)
							{
								auto& b = u.baskets[p.itemID];
								const int price = 1 + int(c.price*Random(1.1, 1.2));
								b.addRing(price, p.numProduct, &c);
							}
							for (auto& p : cJob.consume)
								u.baskets[p.itemID].buyItem(p.numConsume);

							c.money -= totalCost;
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
								if (!b.rings.isEmpty() && (best == NULL || iData[i].value / double(b.rings.front().price) > earn))
								{
									earn = iData[i].value / double(b.rings.front().price);
									best = &b;
								}
							}
							if (best != NULL && best->rings.front().price <= c.money) {
								c.money -= best->rings.front().price;
								best->buyItem(1);
							}
							else break;
						}
					}
					else c.money += 100;	//労働者として働く
				}
			}
		}

		//Vehicleの更新
		for (auto& g : groups)
		{
			for (auto& v : g.vehicles)
			{
				if (v.inRoute())
				{
					v.routeProgress += timeSpeed;
					if (v.routeProgress >= v.getRoute().totalLength)
					{
						v.nowUrbanID = v.getRoute().destinationUrbanID;
						v.routeID = -1;
					}
				}
				else
				{
					v.routeProgress += timeSpeed;
					if (v.routeProgress >= 0.0)
					{
						v.routeProgress = 0.0;
						const auto rs = v.getNowUrban().getRoutes();
						if (!rs.isEmpty()) v.routeID = rs[Random(int(rs.size() - 1))]->id;
						else v.routeProgress = -100.0;
					}
				}
			}
		}

		planet.updateTransform();
		planet.draw();


		if (MouseL.down() && (selectedUrban == NULL || !Rect(32, 32, 320, 640).mouseOver()))
		{
			selectedUrban = NULL;
			for (int i = 0; i < 2; i++)
			{
				const auto t1 = planet.createTransformer(i);
				for (auto& u : urbans)
					if (Circle(u.getPos().mPos, 0.01).mouseOver()) selectedUrban = &u;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			const auto t1 = planet.createTransformer(i);

			//Node
			if (KeyT.pressed())
				for (const auto& n : nodes) n.draw(n.joinedRegionID == -1 ? Color(Palette::White, 64) : n.getJoinedRegion().color);

			//Route
			if (KeyY.pressed())
				for (const auto& r : routes)
					if (r.isSeaRoute) r.draw(Palette::Red);

			//Vehicle
			for (const auto& g : groups)
				for (const auto& v : g.vehicles)
					v.draw();

			//Urban
			for (const auto& u : urbans) u.draw();

		}
		//影
		if (timeSpeed < 0.1)
		{
			const auto t1 = planet.createTransformer();
			RectF((0.25 - worldTimer)*TwoPi - TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
			RectF((0.25 - worldTimer)*TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
			RectF((0.25 - worldTimer)*TwoPi + TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
			RectF((0.25 - worldTimer)*TwoPi + TwoPi * 2, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
		}
		//Interface
		if (selectedUrban != NULL)
		{
			const Color fColor = Palette::Skyblue;
			const Color bColor = Color(Palette::Darkcyan, 192);

			auto& u = *selectedUrban;

			//全体の枠
			Rect(32, 32, 320, 660).draw(bColor).drawFrame(2, fColor);

			//都市の名前の描画
			Rect(32, 32, 240, 36).drawFrame(2, fColor);
			font24(selectedUrban->name).drawAt(152, 50);

			//都市の時刻の描画
			Rect(272, 32, 80, 36).drawFrame(2, fColor);
			font24(selectedUrban->getTimeAsString()).drawAt(312, 50);


			//メニュー選択
			{
				const Array<String> ns = { L"💹",L"👪",L"📰",L"🚢", };
				const Array<DrawingType> ts = { DrawingType::Market,DrawingType::Towner,DrawingType::News,DrawingType::Vehicle, };
				for (int i = 0; i<int(ns.size()); i++)
				{
					const RectF rect(32 + i * 320 / double(ns.size()), 68, 320 / double(ns.size()), 24);
					rect.draw(drawingType == ts[i] ? Palette::Red : rect.mouseOver() ? Palette::Orange : Color(0, 0)).drawFrame(2, fColor);
					if (rect.leftClicked()) drawingType = ts[i];
					font16(ns[i]).drawAt(rect.center());
				}
			}

			switch (drawingType)
			{
			case DrawingType::Market:
			{
				//都市の販売物の描画
				{
					const Transformer2D t1(Mat3x2::Translate(32, 92));
					Rect(128, 24).drawFrame(2, fColor);
					font16(L"販売物").drawAt(64, 12);
				}
				for (int i = 0; i < 36; i++)
				{
					if (i < int(iData.size()))
					{
						auto& b = u.baskets[i];
						const Transformer2D t1(Mat3x2::Translate(32, 116 + i * 16), true);
						const auto rect = Rect(128, 16);
						if (rect.leftClicked()) selectedBasket = b.itemType;
						const Color color = selectedBasket == b.itemType ? Palette::Red : (rect.mouseOver() ? Palette::Orange : Color(Palette::White, 0));
						rect.draw(color).drawFrame(2, fColor);
						font12(iData[i].name).draw(0, 0);
						font12(Format(b.getNumItem()).lpad(4, '0'), L":", Format(b.chart.front()).lpad(5, '0')).draw(48, 0);
					}
					else
					{
						const Transformer2D t1(Mat3x2::Translate(32, 116 + i * 16), true);
						const auto rect = Rect(128, 16);
						const Color color = rect.mouseOver() ? Color(Palette::Orange, 128) : Color(Palette::White, 0);
						rect.draw(color).drawFrame(2, fColor);
						font12(L"---").drawAt(64, 8);
					}
				}

				auto& b = u.baskets[selectedBasket];
				//基本情報
				{
					const Transformer2D t1(Mat3x2::Translate(160, 92));
					Rect(192, 600).drawFrame(2, fColor);
					Rect(192, 24).drawFrame(2, fColor);
					font16(b.getItemName()).draw(16, 0);
					//font16(!b.rings.isEmpty() ? Format(b.rings.front().price) : L"").draw(16, 0);
				}
				//チャートの描画
				{
					const Transformer2D t1(Mat3x2::Translate(160, 116));
					Rect(192, 64).drawFrame(2, fColor);
					int max = 1; for (int i = 0; i < 191; i++) max = Max(max, b.chart[i]);
					for (int i = 0; i < 191; i++)
						Line(191 - i, 63 - b.chart[i] * 62 / max, 190 - i, 63 - b.chart[i + 1] * 62 / max).draw(1, Palette::Yellow);
				}

				break;
			}
			case DrawingType::Towner:
			{
				//都市の販売物の描画
				{
					const Transformer2D t1(Mat3x2::Translate(32, 92));
					Rect(128, 24).drawFrame(2, fColor);
					font16(L"市民").drawAt(64, 12);
				}

				//市民リスト
				for (int i = 0; i < 24; i++)
				{
					if (i < int(cData.size()))
					{
						const Transformer2D t1(Mat3x2::Translate(32, 116 + i * 24), true);
						const auto rect = Rect(128, 24);
						if (rect.leftClicked()) selectedCitizen = i;
						const Color color = selectedCitizen == i ? Palette::Red : (rect.mouseOver() ? Palette::Orange : Color(Palette::White, 0));
						rect.draw(color).drawFrame(2, fColor);
						font16(cData[i].name).draw(0, 0);
					}
					else
					{
						const Transformer2D t1(Mat3x2::Translate(32, 116 + i * 24), true);
						const auto rect = Rect(128, 24);
						const Color color = rect.mouseOver() ? Color(Palette::Orange, 128) : Color(Palette::White, 0);
						rect.draw(color).drawFrame(2, fColor);
						font16(L"---").drawAt(64, 12);
					}
				}

				auto& cd = cData[selectedCitizen];

				//市民情報
				{
					const Transformer2D t1(Mat3x2::Translate(160, 92));
					Rect(192, 48).drawFrame(2, fColor);
					font16(cd.name).draw(16, 0);
					font16(L"人口:", u.citizens.count_if([&](const Citizen& c) {return c.citizenType == selectedCitizen; })).draw(16, 24);
				}
				/*
				//Need
				if (!cd.need.isEmpty())
				{
					const Transformer2D t1(Mat3x2::Translate(160, 140));
					Rect(192, 16 + 16 * int(cd.need.size())).drawFrame(2, fColor);
					Rect(12, 16, 180, 16 * int(cd.need.size())).drawFrame(2, fColor);
					font12(L"消費").draw(4, 0);
					for (int i = 0; i<int(cd.need.size()); i++)
					{
						auto& n = cd.need[i];
						font12(iData[n.itemType].name).draw(16, 16 + i * 16);
					}
				}

				//Product
				if (cd.product.itemType != -1)
				{
					const Transformer2D t1(Mat3x2::Translate(160, !cd.need.isEmpty() ? 156 : 140 + 16 * int(cd.need.size())));
					Rect(192, 16 + 16 * int(cd.product.material.size())).drawFrame(2, fColor);
					Rect(12, 16, 180, 16 * int(cd.product.material.size())).drawFrame(2, fColor);
					font12(L"生産:", iData[cd.product.itemType].name, L"x", cd.product.num).draw(4, 0);
					for (int i = 0; i<int(cd.product.material.size()); i++)
					{
						auto& m = cd.product.material[i];
						font12(iData[m.itemtype].name, L"x", m.num).draw(16, 16 + i * 16);
					}
				}*/
				break;
			}
			}
		}

		planet.updateViewPointSliding();
	}
}