# include "Planet.h"
# include "Node.h"
# include "Pi.h"
# include "Urban.h"
# include "Route.h"
# include "Item.h"
# include "CData.h"

Planet planet;

void Main()
{

	Window::Resize(1280, 720);
	Window::SetTitle(L"Marketplace Game");

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
					for (auto& r : b.rings) r.price = int(r.price*0.95);
					//商品追加
					const int sell = Min(100 - u.baskets[b.itemType].getNumItem(), u.ItemStock[b.itemType]);
					if (sell > 0)
						b.addRing(1 + int(b.minimumPrice*1.05*Random(1.0, 1.2)), sell);
					u.ItemStock[b.itemType] = 0;

					if (!b.rings.isEmpty()) b.minimumPrice = b.rings.front().price;

					//チャートの更新
					b.chart.push_front(b.minimumPrice);
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
					auto& cd = cData[c.citizenType];

					if (cd.product.itemType != -1)
					{
						int cost = Random(cd.product.costMin, cd.product.costMax);
						for (auto& m : cd.product.material)
						{
							if (m.num > u.baskets[m.itemtype].getNumItem()) { cost = -1; break; }
							cost += u.baskets[m.itemtype].getCost(m.num);
						}
						if (cost != -1 && cost <= u.baskets[cd.product.itemType].minimumPrice)
						{
							u.ItemStock[cd.product.itemType] += cd.product.num;
							for (auto& m : cd.product.material)
							{
								auto& b = u.baskets[m.itemtype];
								int num = m.num;
								for (;;)
								{
									auto& r = b.rings.front();
									if (r.num < num) { num -= r.num; b.rings.pop_front(); }
									else if (r.num == num) { b.rings.pop_front(); break; }
									else { r.num -= num; break; }
								}
							}
						}
					}

					for (auto& n : cd.need)
					{
						auto& b = u.baskets[n.itemType];
						if (!b.rings.isEmpty() && b.rings.front().price <= Random(n.priceMin, n.priceMax))
						{
							auto& r = b.rings.front();
							r.num--;
							if (r.num == 0) b.rings.pop_front();
						}
					}
				}
			}
		}

		planet.updateTransform();

		{
			const Transformer2D t1(planet.getTransform(), true);
			planet.draw();
		}

		if (MouseL.down() && (selectedUrban == NULL || !Rect(32, 32, 320, 640).mouseOver()))
		{
			selectedUrban = NULL;
			for (int i = 0; i < 2; i++)
			{
				const Transformer2D t1(planet.getTransform(i), true);
				for (auto& u : urbans)
					if (Circle(u.getPos().mPos, 0.01).mouseOver()) selectedUrban = &u;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			const Transformer2D t1(planet.getTransform(i), true);

			//Node
			if (KeyT.pressed())
				for (const auto& n : nodes) n.draw(n.joinedRegionID == -1 ? Color(Palette::White, 64) : n.getJoinedRegion().color);

			//Route
			if (KeyY.pressed())
				for (const auto& r : routes)
					if (r.isSeaRoute) r.draw(Palette::Red);

			//Urban
			for (const auto& u : urbans) u.draw();

		}
		//影
		if (timeSpeed < 0.1)
		{
			const Transformer2D t1(planet.getTransform());
			RectF((0.25 - worldTimer)*TwoPi - TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
			RectF((0.25 - worldTimer)*TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
			RectF((0.25 - worldTimer)*TwoPi + TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
		}
		//Interface
		if (selectedUrban != NULL)
		{
			const Color fColor = Palette::Skyblue;
			const Color bColor = Color(Palette::Darkcyan, 192);

			auto& u = *selectedUrban;

			//全体の枠
			Rect(32, 32, 320, 660).drawFrame(2, fColor);

			//都市の名前の描画
			Rect(32, 32, 240, 36).draw(bColor).drawFrame(2, fColor);
			font24(selectedUrban->name).drawAt(152, 50);

			//都市の時刻の描画
			{
				const Transformer2D t1(Mat3x2::Translate(272, 32));
				Rect(0, 0, 80, 36).draw(bColor).drawFrame(2, fColor);
				font24(selectedUrban->getTimeAsString()).drawAt(40, 18);
			}

			//メニュー選択
			{
				auto t1 = Transformer2D::Transformer2D(Mat3x2::Translate(32, 68), true);
				Rect(0, 0, 80, 24).draw(drawingType == DrawingType::Market ? Palette::Red : Rect(0, 0, 80, 24).mouseOver() ? Palette::Orange : bColor).drawFrame(2, fColor);
				if (Rect(0, 0, 80, 24).leftClicked()) drawingType = DrawingType::Market;
				font16(L"💹").drawAt(40, 12);

				Rect(80, 0, 80, 24).draw(drawingType == DrawingType::Towner ? Palette::Red : Rect(80, 0, 80, 24).mouseOver() ? Palette::Orange : bColor).drawFrame(2, fColor);
				if (Rect(80, 0, 80, 24).leftClicked()) drawingType = DrawingType::Towner;
				font16(L"👪").drawAt(120, 12);

				Rect(160, 0, 80, 24).draw(drawingType == DrawingType::News ? Palette::Red : Rect(160, 0, 80, 24).mouseOver() ? Palette::Orange : bColor).drawFrame(2, fColor);
				if (Rect(160, 0, 80, 24).leftClicked()) drawingType = DrawingType::News;
				font16(L"📰").drawAt(200, 12);

				Rect(240, 0, 80, 24).draw(drawingType == DrawingType::Vehicle ? Palette::Red : Rect(240, 0, 80, 24).mouseOver() ? Palette::Orange : bColor).drawFrame(2, fColor);
				if (Rect(240, 0, 80, 24).leftClicked()) drawingType = DrawingType::Vehicle;
				font16(L"🚢").drawAt(280, 12);
			}

			switch (drawingType)
			{
			case DrawingType::Market:
			{
				//都市の販売物の描画
				{
					const Transformer2D t1(Mat3x2::Translate(32, 92));
					Rect(0, 0, 128, 24).draw(bColor).drawFrame(2, fColor);
					font16(L"販売物").drawAt(64, 12);
				}

				{
					const Transformer2D t1(Mat3x2::Translate(32, 116));
					Rect(0, 0, 128, 36 * 16).draw(bColor).drawFrame(2, fColor);
				}
				for (int i = 0; i < 36; i++)
				{
					if (i < int(iData.size()))
					{
						auto& b = u.baskets[i];
						const Transformer2D t1(Mat3x2::Translate(32, 116 + i * 16), true);
						const auto rect = Rect(0, 0, 128, 16);
						if (rect.leftClicked()) selectedBasket = b.itemType;
						const Color color = selectedBasket == b.itemType ? Palette::Red : (rect.mouseOver() ? Palette::Orange : Color(Palette::White, 0));
						rect.draw(color).drawFrame(2, fColor);
						font12(iData[i].name).draw(0, 0);
						font12(Format(b.getNumItem()).lpad(4, '0'), L":", Format(b.minimumPrice).lpad(5, '0')).draw(48, 0);
					}
					else
					{
						const Transformer2D t1(Mat3x2::Translate(32, 116 + i * 16), true);
						const auto rect = Rect(0, 0, 128, 16);
						const Color color = rect.mouseOver() ? Color(Palette::Orange, 128) : Color(Palette::White, 0);
						rect.draw(color).drawFrame(2, fColor);
						font12(L"---").drawAt(64, 8);
					}
				}

				auto& b = u.baskets[selectedBasket];
				//基本情報
				{
					const Transformer2D t1(Mat3x2::Translate(160, 92));
					Rect(0, 0, 192, 600).draw(bColor).drawFrame(2, fColor);
					Rect(0, 0, 192, 24).drawFrame(2, fColor);
					font16(b.getItemName(), L" ストック数:", u.ItemStock[b.itemType]).draw(16, 0);
					//font16(!b.rings.isEmpty() ? Format(b.rings.front().price) : L"").draw(16, 0);
				}
				//チャートの描画
				{
					const Transformer2D t1(Mat3x2::Translate(160, 116));
					Rect(0, 0, 192, 64).drawFrame(2, fColor);
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
					Rect(0, 0, 128, 24).draw(bColor).drawFrame(2, fColor);
					font16(L"市民").drawAt(64, 12);
				}

				//市民リスト
				{
					const Transformer2D t1(Mat3x2::Translate(32, 116));
					Rect(0, 0, 128, 24 * 24).draw(bColor).drawFrame(2, fColor);
				}

				for (int i = 0; i < 24; i++)
				{
					if (i < int(cData.size()))
					{
						const Transformer2D t1(Mat3x2::Translate(32, 116 + i * 24), true);
						const auto rect = Rect(0, 0, 128, 24);
						if (rect.leftClicked()) selectedCitizen = i;
						const Color color = selectedCitizen == i ? Palette::Red : (rect.mouseOver() ? Palette::Orange : Color(Palette::White, 0));
						rect.draw(color).drawFrame(2, fColor);
						font16(cData[i].name).draw(0, 0);
					}
					else
					{
						const Transformer2D t1(Mat3x2::Translate(32, 116 + i * 24), true);
						const auto rect = Rect(0, 0, 128, 24);
						const Color color = rect.mouseOver() ? Color(Palette::Orange, 128) : Color(Palette::White, 0);
						rect.draw(color).drawFrame(2, fColor);
						font16(L"---").drawAt(64, 12);
					}
				}

				auto& cd = cData[selectedCitizen];

				//市民情報
				{
					const Transformer2D t1(Mat3x2::Translate(160, 92));
					Rect(0, 0, 192, 600).draw(bColor).drawFrame(2, fColor);
					Rect(0, 0, 192, 48).drawFrame(2, fColor);
					font16(cd.name).draw(16, 0);
					font16(L"人口:", u.citizens.count_if([&](const Citizen& c) {return c.citizenType == selectedCitizen; })).draw(16, 24);
				}

				//Need
				if (!cd.need.isEmpty())
				{
					const Transformer2D t1(Mat3x2::Translate(160, 140));
					Rect(0, 0, 192, 16 + 16 * int(cd.need.size())).drawFrame(2, fColor);
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
					Rect(0, 0, 192, 16 + 16 * int(cd.product.material.size())).drawFrame(2, fColor);
					Rect(12, 16, 180, 16 * int(cd.product.material.size())).drawFrame(2, fColor);
					font12(L"生産:", iData[cd.product.itemType].name, L"x", cd.product.num).draw(4, 0);
					for (int i = 0; i<int(cd.product.material.size()); i++)
					{
						auto& m = cd.product.material[i];
						font12(iData[m.itemtype].name, L"x", m.num).draw(16, 16 + i * 16);
					}
				}
				break;
			}

			}
		}

		planet.updateViewPointSliding();
	}
}