#include"Display.h"
#include"Urban.h"
#include"CitizenData.h"
#include<numeric>
#include"VehicleData.h"
#include"ItemData.h"

void	DisplayUrban::update()
{
	if (selecter.selectedUrban != nullptr)
	{
		if (KeyRight.down())
		{
			if (selecter.selectedUrban == &urbans.back()) selecter.selectedUrban = &urbans.front();
			else selecter.selectedUrban++;
			openElapsedTime.restart();
		}
		if (KeyLeft.down())
		{
			if (selecter.selectedUrban == &urbans.front()) selecter.selectedUrban = &urbans.back();
			else selecter.selectedUrban--;
			openElapsedTime.restart();
		}
	}

	const auto frameColor = Color(20);
	const auto baseColor = Color(40);
	const auto fontColor = Color(255);
	const auto mouseOverColor = Color(70);
	const auto font16 = (*globalFonts[16]);
	const auto font24 = (*globalFonts[24]);
	const auto font36 = (*globalFonts[36]);
	const auto thickness = 4;
	auto* su = selecter.selectedUrban;
	auto conv = [](int value) {
		if (value < 1000) return Format(value);
		if (value < 1000000) return Format(value / 1000, L"K");
		if (value < 1000000000) return Format(value / 1000000, L"M");
		else return String(L"HugeValue");
	};
	Transformer2D t1;
	if (selecter.selectedUrban == nullptr)  t1 = Transformer2D(Mat3x2::Translate(EaseOut(Easing::Expo, 0.0, -480.0, Min(1.0, closeElapsedTime.ms() / 500.0)), 0));
	else t1 = Transformer2D(Mat3x2::Translate(EaseOut(Easing::Expo, -480.0, 0.0, Min(1.0, openElapsedTime.ms() / 500.0)), 0));


	//全体枠の表示
	{
		const Rect(0, 0, 480, 720).draw(baseColor).drawFrame(thickness, frameColor);
	}

	if (selecter.selectedUrban == nullptr) return;

	//都市名の表示
	{
		const Rect rect(0, 0, 480, 48);
		font36(su->name).drawAt(rect.center(), fontColor);
	}

	//選択
	{
		const Array<String>	list = { L"労働", L"市場", L"造船", L"政治" };
		const auto w = int(448 / list.size());
		{
			const auto backgroundColor = Color(60);
			for (int i = 0; i < int(list.size()); i++)
			{
				const Rect rect1(w*i + 16, 48, w, 36);
				const Polygon rect2{
					{ 12 + w*i, 84 },
					{ 20 + w*i, 48 },
					{ 12 + w*i + w, 48 },
					{ 20 + w*i + w, 84 },
				};
				if (rect2.leftClicked()) urbanInfoState = UrbanInfoState(i);
				rect2.draw(rect1.mouseOver() ? mouseOverColor : backgroundColor).drawFrame(thickness, frameColor);
				font24(list[i]).drawAt(rect1.center(), fontColor);
			}
		}
		{
			const auto backgroundColor = Color(80);
			const Rect rect1(w*int(urbanInfoState) + 16, 48, w, 36);
			Polygon{
				{ 12 + w*int(urbanInfoState), 84 },
				{ 20 + w*int(urbanInfoState), 48 },
				{ 12 + w*int(urbanInfoState) + w, 48 },
				{ 20 + w*int(urbanInfoState) + w, 84 },
				{ 472, 84 },
				{ 472, 712 },
				{ 8, 712 },
				{ 8, 84 },
			}.draw(backgroundColor).drawFrame(thickness, frameColor);
			font24(list[int(urbanInfoState)]).drawAt(rect1.center(), fontColor);
		}
	}
	switch (urbanInfoState)
	{
	case UrbanInfoState::Worker:
	{
		//基礎情報の描画
		{
			Rect(16, 100, 160, 72).draw(Color(100));
			{
				const Rect rect(16, 100, 160, 24);
				rect.drawFrame(thickness, frameColor);
				font16(L"基礎情報").drawAt(rect.center(), fontColor);
			}
			//人口の描画
			{
				const Rect rect1(16, 124, 80, 24);
				const Rect rect2(96, 124, 80, 24);
				rect1.drawFrame(thickness, frameColor);
				rect2.drawFrame(thickness, frameColor);
				const String string1(L"総人口");
				const String string2(Format(conv(int(su->citizens.size())), L"人"));
				font16(string1).drawAt(rect1.center(), fontColor);
				auto w = (int)font16(string2).region().w;
				font16(string2).draw(rect2.tr().movedBy(-4 - w, 0), fontColor);
			}
			//市民収入の描画
			{
				const Rect rect1(16, 148, 80, 24);
				const Rect rect2(96, 148, 80, 24);
				rect1.drawFrame(thickness, frameColor);
				rect2.drawFrame(thickness, frameColor);
				const String string1(L"市民収入");
				const String string2 = Format(conv(su->averageIncome), L"G");
				font16(string1).drawAt(rect1.center(), fontColor);
				auto w = (int)font16(string2).region().w;
				font16(string2).draw(rect2.tr().movedBy(-4 - w, 0), fontColor);
			}
		}

		//労働人口グラフ
		{
			typedef std::pair<CitizenData*, double> List;

			Rect rect(16, 180, 320, 320);
			Circle circle(rect.center(), 130);
			Array<List> list;

			rect.drawFrame(thickness, frameColor);

			for (auto& cd : citizenData)
			{
				list.emplace_back(&cd, 360_deg*double(su->numCitizens(cd.id())) / double(su->citizens.size()));
			}
			list.sort_by([](List& x, List& y) { return x.second > y.second; });
			for (auto& l : list)
			{
				if (l.second < 10_deg) l.first = nullptr;
			}
			{
				circle.drawFrame(thickness, frameColor);
				for (auto& l : list)
				{
					const double startAngle = std::accumulate(list.begin(), list.begin() + int(&l - &list.front()), 0.0, [](double sum, List& l) { return sum + l.second; });
					const auto color = l.first == nullptr ? Color(80) : HSV(360 * double(l.first->id()) / double(citizenData.size()), 0.8, 0.8);
					const auto centerAngle = startAngle + l.second*0.5;
					if (l.first != nullptr)
					{
						circle.drawPie(startAngle, l.second, color);
						circle.drawArc(startAngle, l.second, thickness / 2.0, thickness / 2.0, frameColor);
						Line(circle.center, circle.center.movedBy(Vec2(0, -circle.r).rotated(startAngle))).draw(thickness, frameColor);
						Line(circle.center, circle.center.movedBy(Vec2(0, -circle.r).rotated(startAngle + l.second))).draw(thickness, frameColor);
						if (circle.mouseOver() && Abs(-Vec2(Cursor::Pos() - circle.center).getAngle(Vec2::Down()) + 180_deg - centerAngle) < l.second*0.5)
						{
							double add = 20;
							circle.drawArc(startAngle, l.second, 10, add, color);
							circle.drawArc(startAngle, l.second, -20 + thickness / 2.0, add + thickness / 2.0, frameColor);
							Line(circle.center, circle.center.movedBy(Vec2(0, -circle.r - add).rotated(startAngle))).draw(thickness, frameColor);
							Line(circle.center, circle.center.movedBy(Vec2(0, -circle.r - add).rotated(startAngle + l.second))).draw(thickness, frameColor);
						}
					}
				}
				Circle(circle.center, thickness / 2.0).draw(Palette::Black);
				for (auto& l : list)
				{
					const double startAngle = std::accumulate(list.begin(), list.begin() + int(&l - &list.front()), 0.0, [](double sum, List& l) { return sum + l.second; });
					if (l.first != nullptr)
					{
						Vec2 pos = rect.center().movedBy(Vec2(0, -70).rotated(startAngle + l.second / 2.0));
						for (double d = 0; d < 360_deg; d += 10_deg) font24(l.first->name).drawAt(pos + Vec2(2, 0).rotated(d), Palette::White);
						font24(l.first->name).drawAt(pos, Palette::Black);
					}
				}
			}
		}
	}

	break;

	case UrbanInfoState::Shelves:
	{

	}
	break;

	case UrbanInfoState::Docks:
	{
		//チケット
		for (int i = 0; i < int(su->tickets.size()); i++)
		{
			auto& st = su->tickets[i];
			Rect rect(248, 100 + i * 28, 200, 24);

			rect.draw(Color(60)).drawFrame(2, Color(40));
			vehicleData[st.vehicleType].icon.resize(24, 24).draw(rect.pos.movedBy(4,0));
		}

		//ドック
		for (int i = 0; i < int(su->docks.size()); i++)
		{
			auto& sd = su->docks[i];

			Rect rect(32, 100 + i * 48, 200, 32);
			rect.draw(Color(60));
			
			if (sd.inProcessTicket != nullptr)
			{
				auto* t = sd.inProcessTicket;
				Rect(rect.pos, int(rect.size.x*sd.progress / t->data().constructionCost), rect.size.y).draw(Palette::Green);
				t->data().icon.resize(24, 24).drawAt(rect.center());
				Line(rect.tr().movedBy(0, 16), Vec2(248, 100 + (t - &su->tickets.front()) * 28 + 12)).draw(2, Palette::Red);
			}

			font24(L"Tier", sd.tier).draw(rect.pos.movedBy(4, 0));
			rect.drawFrame(2, Color(40));
		}
	}
	break;

	default:
		break;
	}
	{

	}
}