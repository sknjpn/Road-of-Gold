#include "Planet.h"
#include "Node.h"
#include "Pi.h"
#include "Urban.h"
#include "Route.h"
#include "CData.h"
#include "Group.h"
#include "GlobalVariables.h"

double timeSpeed = 0.01;
double worldTimer = 0.0;
int selectedBasket = 0;
int selectedCitizen = 0;

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
	const Texture ship(L"Assets/Ship.png");
	const Texture wagon(L"Assets/Wagon.png");
	Node* nearestNode = nullptr;

	enum struct DrawingType
	{
		Market, Towner, News, Vehicle
	}
	drawingType = DrawingType::Market;


	if (!loadEconomicData() || !loadNodeMap() || !planet.loadBiome() || !planet.loadVoronoiMap()) return;
	planet.setRegions();

	//Urbanの生成
	auto numUrbans = int(nodes.count_if([](const auto& n) {return !n.isSea(); })) / 100;
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

	while (System::Update())
	{
		if (KeyF1.down()) timeSpeed = 0.0001;
		if (KeyF2.down()) timeSpeed = Max(0.0001, timeSpeed*0.5);
		if (KeyF3.down()) timeSpeed = Min(1.0000, timeSpeed*2.0);
		if (KeyF4.down()) timeSpeed = 1.0000;

		worldTimer = fmod(worldTimer + timeSpeed, 1.0);

		//UrbanUpdate
		for (auto& u : urbans)
			u.update();

		//Vehicleの更新
		for (auto& g : groups)
			g.update();

		planet.updateTransform();
		
		//マップの描画
		for (int i = 0; i < 2; i++) {
			const auto t1 = planet.createTransformer(i);
			planet.mapTexture.resize(TwoPi, Pi).drawAt(0, 0);
		}


		if (MouseL.down() && (selectedUrban == nullptr || !Rect(32, 32, 320, 640).mouseOver()))
		{
			selectedUrban = nullptr;
			for (int i = 0; i < 2; i++)
			{
				const auto t1 = planet.createTransformer(i);
				for (auto& u : urbans)
					if (Circle(u.getPos().mPos, 0.01).mouseOver()) selectedUrban = &u;
			}
		}

		{
			auto mp = planet.getCursorPos();
			for (auto& n : nodes)
				if (nearestNode == nullptr || (n.pos.ePos - mp.ePos).length() < (nearestNode->pos.ePos - mp.ePos).length()) nearestNode = &n;
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
		/*
		if (timeSpeed < 0.1)
		{
			const auto t1 = planet.createTransformer();
			RectF((0.25 - worldTimer)*TwoPi - TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
			RectF((0.25 - worldTimer)*TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
			RectF((0.25 - worldTimer)*TwoPi + TwoPi, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
			RectF((0.25 - worldTimer)*TwoPi + TwoPi * 2, -HalfPi, Pi, Pi).draw(ColorF(Palette::Black, 0.5));
		}*/
		//Interface
		if (selectedUrban != nullptr)
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
				auto& bs = u.baskets[selectedBasket];

				//都市の販売物の描画
				Rect(32, 92, 128, 24).drawFrame(2, fColor);
				font16(L"販売物").drawAt(96, 104);

				//商品一覧
				for (int i = 0; i < 36; i++)
				{
					const auto rect = Rect(32, 116 + i * 16, 128, 16);
					if (i < int(iData.size()))
					{
						auto& b = u.baskets[i];
						if (rect.leftClicked()) selectedBasket = b.itemType;
						const Color color = selectedBasket == b.itemType ? Palette::Red : (rect.mouseOver() ? Palette::Orange : Color(0, 0));
						rect.draw(color).drawFrame(2, fColor);
						font12(iData[i].name).draw(32, 116 + i * 16);
						font12(Format(b.chart.front()).lpad(5, '0'), L"G").draw(108, 116 + i * 16);
					}
					else
					{
						const Color color = rect.mouseOver() ? Color(Palette::Orange, 128) : Color(0, 0);
						rect.draw(color).drawFrame(2, fColor);
						font12(L"---").drawAt(96, 124 + i * 16);
					}
				}

				//基本情報
				Rect(160, 92, 192, 600).drawFrame(2, fColor);
				Rect(160, 92, 192, 24).drawFrame(2, fColor);
				font16(bs.getItemName()).drawAt(252, 104);


				//チャート
				Rect(160, 180, 192, 64).drawFrame(2, fColor);
				int max = 1; for (int i = 0; i < 191; i++) max = Max(max, bs.chart[i]);
				for (int i = 0; i < 191; i++)
					Line(191 - i, 63 - bs.chart[i] * 62 / max, 190 - i, 63 - bs.chart[i + 1] * 62 / max).movedBy(160, 180).draw(1, Palette::Yellow);

				break;
			}
			case DrawingType::Towner:
			{
				//都市の販売物の描画
				Rect(32, 92, 128, 24).drawFrame(2, fColor);
				font16(L"市民").drawAt(96, 104);

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
					Rect(192, 72).drawFrame(2, fColor);
					font16(cd.name).draw(16, 0);
					const int numCitizen = int(u.citizens.count_if([&](const Citizen& c) {return c.citizenType == selectedCitizen; }));
					font16(L"人口　　　:", numCitizen).draw(16, 24);
					int sumHapiness = 0;
					for (const auto& c : u.citizens) if (c.citizenType == selectedCitizen) sumHapiness += c.hapiness;
					font16(L"幸福:", int(sumHapiness / double(numCitizen))).draw(16, 48);
					int sumBhs = 0;
					for (const auto& c : u.citizens) if (c.citizenType == selectedCitizen) sumBhs += c.bhs;
					font16(L"BHS:", int(sumBhs / double(numCitizen) / 30.0)).draw(16 + 96, 48);


				}
				//各市民情報
				{
					int j = 0;
					for (int i = 0; i < 33; i++)
					{
						for (; j<int(u.citizens.size()); j++)
						{
							const auto& c = u.citizens[j];
							if (c.citizenType == selectedCitizen)
							{
								Rect(160, 164 + 16 * i, 192, 16).drawFrame(2, fColor);
								font12(L"財産:", c.money, L"G").draw(176, 164 + 16 * i);
								font12(L"幸福:", c.hapiness).draw(176 + 96, 164 + 16 * i);
								j++;
								break;
							}
						}
					}
				}
				break;
			}
			case DrawingType::News:
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
					Rect(192, 72).drawFrame(2, fColor);
					font16(cd.name).draw(16, 0);
					const int numCitizen = int(u.citizens.count_if([&](const Citizen& c) {return c.citizenType == selectedCitizen; }));
					font16(L"人口　　　:", numCitizen).draw(16, 24);
					int sumHapiness = 0;
					for (const auto& c : u.citizens) if (c.citizenType == selectedCitizen) sumHapiness += c.hapiness;
					font16(L"幸福:", sumHapiness / double(numCitizen)).draw(16, 48);
					int sumBhs = 0;
					for (const auto& c : u.citizens) if (c.citizenType == selectedCitizen) sumBhs += c.bhs;
					font16(L"BHS:", int(sumBhs / double(numCitizen) / 30.0)).draw(16 + 96, 48);

					font12(cd.job.description).draw(4, 72);
					font16(L"維持費:", 50 + cd.job.cost).draw(4, 72 + 16);
					font16(L"賃金　:", cd.job.wage).draw(4, 72 + 40);
					int p = 0;
					font12(L"消費↓").draw(4, 72 + 64);
					for (auto& c : cd.job.consume)
					{
						font12(L" ", iData[c.itemID].name, L"x", c.numConsume).draw(4, 72 + 80 + p * 16);
						p++;
					}
					font12(L"生産↓").draw(4, 72 + 80 + p * 16);
					p++;
					for (auto& c : cd.job.product)
					{
						font12(L" ", iData[c.itemID].name, L"x", c.numProduct).draw(4, 72 + 80 + p * 16);
						p++;
					}
				}
				break;
			}
			}
		}
		Rect(256, 24).draw(Palette::White);
		font16(bData[nearestNode->biomeType].name).draw(0, 0, Palette::Black);

		font16(L"F1～F3キーで倍速設定が出来ます。赤い都市アイコンをクリックで詳細が見れます。").draw(256, 0);

		planet.updateViewPointSliding();
	}
}