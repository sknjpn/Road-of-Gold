#include"UI.h"
#include"Planet.h"
#include"Urban.h"
#include"ItemData.h"
#include"CitizenData.h"
#include"Route.h"
#include"Vehicle.h"
#include"Export.h"

void	drawUI()
{
	if (useMulthThread)
	{
		(*ui.fonts[16])(L"マルチスレッドは有効です。無効化:Mキー").draw(560, 0, Palette::Red);
	}
	else
	{
		(*ui.fonts[16])(L"マルチスレッドは無効です。有効化:Mキー").draw(560, 0, Palette::Red);
	}

	//Export
	if (KeyE.down()) ui.drawExportLineEnabled = !ui.drawExportLineEnabled;
	if (KeyR.down()) ui.drawExportImportPowerEnabled = !ui.drawExportImportPowerEnabled;
	if (ui.drawExportImportPowerEnabled)
	{
		for (int i = 0; i < 2; i++)
		{
			auto t = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				double sum = 0;
				for (auto& e : exports)
					if (e.from == &u || e.to == &u) sum += e.numItemPerDay;
				Circle(u.pos().mPos, 0.0060*pow(sum, 0.5)).draw(Palette::Red);
			}
		}
	}

	//矢印
	for (int i = 0; i < 2; i++)
	{
		auto t = tinyCamera.createTransformer(i);

		for (auto& e : exports)
		{
			if ((ui.drawExportLineEnabled && (ui.selectedItemType == -1 || ui.selectedItemType == e.itemType)) || (ui.selectedUrbanID != -1 && (e.from->id() == ui.selectedUrbanID || e.to->id() == ui.selectedUrbanID)))
			{
				double width = 0.05*e.numItemPerDay;

				auto p = (e.to->pos().mPos - e.from->pos().mPos).normalized();
				auto p1 = e.from->pos().mPos + p*0.02;
				auto p2 = e.to->pos().mPos - p*0.02;
				Triangle(p1, p1.lerp(p2, 0.8) + width*p.rotated(90_deg)*0.01, p1.lerp(p2, 0.8) - width*p.rotated(90_deg)*0.01).draw(ColorF(itemData[e.itemType].color, 0.8));
				Triangle(p2, p1.lerp(p2, 0.8) + width*p.rotated(90_deg)*0.03, p1.lerp(p2, 0.8) - width*p.rotated(90_deg)*0.03).draw(ColorF(itemData[e.itemType].color, 0.8));

			}
		}
	}

	//ItemSelect
	if (ui.selectedUrbanID == -1 && ui.selectedVehicleID == -1)
	{
		const auto fColor = Palette::Skyblue;
		const auto bColor = Color(Palette::Darkcyan, 192);

		if (MouseL.down()) ui.selectedItemType = -1;

		for (int i = 0; i<int(itemData.size()); i++)
		{
			auto& data = itemData[i];
			{
				Rect rect(32 + i * 64, 8, 64, 24);
				rect.draw(bColor).drawFrame(2, fColor);
				(*ui.fonts[16])(data.name).drawAt(rect.center());
			}
			{
				Rect rect(32 + i * 64, 32, 64, 64);
				if (rect.leftClicked()) ui.selectedItemType = i;
				rect.draw(rect.mouseOver() ? Palette::Orange : ui.selectedItemType == i ? Palette::Red : bColor).drawFrame(2, fColor);
				data.icon.resize(64).drawAt(rect.center());
			}
		}

	}

	//基本UI
	if (ui.selectedVehicleID != -1)
	{
		tinyCamera.gazePoint.emplace(vehicles.at(ui.selectedVehicleID).pos());
	}
	if (ui.selectedUrbanID != -1)
	{
		auto& su = urbans[ui.selectedUrbanID];

		const auto fColor = Palette::Skyblue;
		const auto bColor = Color(Palette::Darkcyan, 192);

		//全体枠
		{
			Rect rect(240, Window::Size().y);
			rect.draw(bColor).drawFrame(2, fColor);
		}

		//都市名
		{
			Rect rect(240, 32);
			rect.drawFrame(2, fColor);
			(*ui.fonts[24])(su.name).drawAt(rect.center());
		}

		//Route
		{
			int p = 200;
			//海路
			{
				{
					Rect rect(0, p, 240, 24);
					rect.drawFrame(2, fColor);
					(*ui.fonts[16])(L"海路の一覧").drawAt(rect.center());
					p += 24;
				}

				Array<Route*> list = su.ownRoutes.filter([](const Route* r) { return r->isSeaRoute; });
				list.sort_by([](const Route* r1, const Route* r2) { return  r1->movingCost < r2->movingCost; });

				for (auto i : step(int(list.size())))
				{
					Rect rect(0, p + i * 17, 240, 17);
					rect.drawFrame(2, fColor);
					auto* r = list[i];

					const int width = int((*ui.fonts[12])(int(r->movingCost * 1000), L"mt").region().w);
					(*ui.fonts[12])(int(r->movingCost * 1000), L"mt").draw(rect.pos.movedBy(64 - width, 0));
					(*ui.fonts[12])(r->toUrban->name).draw(rect.pos.movedBy(96, 0));
				}
				p += int(list.size()) * 17;
			}

			p += 24;

			//陸路
			{
				{
					Rect rect(0, p, 240, 24);
					rect.drawFrame(2, fColor);
					(*ui.fonts[16])(L"陸路の一覧").drawAt(rect.center());
					p += 24;
				}

				Array<Route*> list = su.ownRoutes.filter([](const Route* r) { return !r->isSeaRoute; });
				list.sort_by([](const Route* r1, const Route* r2) { return  r1->movingCost < r2->movingCost; });

				for (auto i : step(int(list.size())))
				{
					Rect rect(0, p + i * 17, 240, 17);
					rect.drawFrame(2, fColor);
					auto* r = list[i];

					const int width = int((*ui.fonts[12])(int(r->movingCost * 1000), L"mt").region().w);
					(*ui.fonts[12])(int(r->movingCost * 1000), L"mt").draw(rect.pos.movedBy(64 - width, 0));
					(*ui.fonts[12])(r->toUrban->name).draw(rect.pos.movedBy(96, 0));
				}
			}

		}

		//効率
		{
			for (auto i : step(int(citizenData.size())))
			{
				auto& data = citizenData[i];
				auto& je = su.jobEfficiency[i];
				{
					Rect rect(0, 32 + 17 * i, 80, 17);
					rect.drawFrame();
					(*ui.fonts[12])(data.name).draw(rect.pos.movedBy(4, 0));
				}
				{
					Rect rect(80, 32 + 17 * i, 48, 17);
					rect.drawFrame();
					const int width = int((*ui.fonts[12])(su.citizens.count_if([&i](const Citizen& c) { return c.citizenType == i; }), L"人").region().size.x);
					(*ui.fonts[12])(su.citizens.count_if([&i](const Citizen& c) { return c.citizenType == i; }), L"人").draw(rect.pos.movedBy(44 - width, 0));
				}
				{
					Rect rect(128, 32 + 17 * i, 40, 17);
					rect.drawFrame();
					const int width = int((*ui.fonts[12])(int(je * 100), L"%").region().size.x);
					(*ui.fonts[12])(int(je * 100), L"%").draw(rect.pos.movedBy(36 - width, 0));
				}
				{
					int sum = 0, num = 0;
					for (auto c : su.citizens)
					{
						if (c.citizenType == i)
						{
							sum += c.avgIncome();
							num++;
						}
					}
					if (num > 0)
					{
						Rect rect(168, 32 + 17 * i, 52, 17);
						rect.drawFrame();
						const int width = int((*ui.fonts[12])(sum / num, L"G").region().size.x);
						(*ui.fonts[12])(sum / num, L"G").draw(rect.pos.movedBy(48 - width, 0));
					}
				}
			}
		}
		//Basket
		{
			for (auto i : step(int(su.baskets.size())))
			{
				const auto& b = su.baskets[i];
				const auto& data = b.data();

				{
					Rect rect(240, i * 88, 240, 24);
					rect.draw(bColor).drawFrame(2, fColor);
					(*ui.fonts[16])(data.name).draw(rect.pos.movedBy(4, 0));
					if (!b.rings.isEmpty())
						(*ui.fonts[16])(b.rings.front().price).draw(rect.pos.movedBy(64 + 4, 0));
					(*ui.fonts[16])(b.numItem).draw(rect.pos.movedBy(128 + 4, 0));
				}
				{
					Rect rect(240, 24 + i * 88, 240, 64);
					rect.draw(bColor).drawFrame(2, fColor);
					const int timeScale = 1; //b.tradeLog.time / 120
					
					int max =Max(1, b.tradeLog.numConsumption.take(rect.size.x).sorted().back());
					max = Max(max, b.tradeLog.numExport.take(rect.size.x).sorted().back());
					max = Max(max, b.tradeLog.numImport.take(rect.size.x).sorted().back());
					max = Max(max, b.tradeLog.numProduction.take(rect.size.x).sorted().back());

					drawGraph(rect, b.tradeLog.numExport, timeScale, Palette::Blue, max);
					drawGraph(rect, b.tradeLog.numImport, timeScale, Palette::Purple, max);
					drawGraph(rect, b.tradeLog.numConsumption, timeScale, Palette::Yellowgreen, max);
					drawGraph(rect, b.tradeLog.numProduction, timeScale, Palette::Red, max);

					drawGraph(rect, b.tradeLog.price, timeScale, Palette::Yellow);
					data.icon.draw(rect.pos);
				}
				/*
				{
					for (auto j : step(int(b.rings.size())))
					{
						const auto& r = b.rings[j];
						Rect rect(320 + i * 120, 152 + j * 16, 120, 16);

						rect.drawFrame(2, fColor);
						(*ui.fonts[12])(r.price, L"G ", r.casket.numItem, L"個").draw(rect.pos.movedBy(4, 0));
					}
				}
				*/
			}
		}
	}

	//タイムスケールの変更
	{
		const auto fColor = Palette::Skyblue;
		const auto bColor = Palette::Greenyellow;
		const auto& font = *ui.fonts[24];
		const Rect rect(Window::Size().x - 300, 32, 300, 32);
		rect.draw(bColor).drawFrame(2, fColor);
		font(L"速度").draw(rect.pos, Palette::Black);
		const auto t1 = Triangle(24).rotated(30_deg).movedBy(rect.pos + Vec2(64, 16));
		const auto t2 = Triangle(24).rotated(90_deg).movedBy(rect.pos + Vec2(284, 16));
		const auto c1 = Circle(rect.pos + Vec2(64, 16), 14);
		const auto c2 = Circle(rect.pos + Vec2(284, 16), 14);
		c1.draw(Palette::Gray);
		c2.draw(Palette::Gray);
		t1.draw(c1.mouseOver() ? Palette::Orange : Palette::Yellow);
		t2.draw(c2.mouseOver() ? Palette::Orange : Palette::Yellow);
		if (c1.leftClicked())
		{
			planet.timeSpeed = planet.timeSpeed / 2;
			if (planet.timeSpeed < pow(0.5, 12)) planet.timeSpeed = 0;
		}
		if (c2.leftClicked())
		{
			planet.timeSpeed = Min(1.0, planet.timeSpeed * 2);
			if (planet.timeSpeed == 0) planet.timeSpeed = pow(0.5, 12);
		}
		Rect(rect.pos + Point(82, 8), Point(182, 16)).draw(Palette::Red);
		if (planet.timeSpeed > 0) Rect(rect.pos + Point(82, 8), Point(182 - int(14 * log2(1 / planet.timeSpeed)), 16)).draw(Palette::Green);
		for (int i = 0; i < 13; i++) Rect(rect.pos + Point(82 + 14 * i, 8), Point(14, 16)).drawFrame(2, Palette::Black);
	}

	//グローバル時間の表示
	{
		const Point pos(Window::Size().x - 300, 0);
		const auto& s = planet.sandglass;
		const auto& font = *ui.fonts[24];
		const auto fColor = Palette::Skyblue;
		const auto bColor = Palette::Greenyellow;

		//年表示
		{
			const auto rect = Rect(0, 0, 84, 32).movedBy(pos);
			const String text = Format(int(s.timer / 360), L"年");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(2, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0), Palette::Black);
		}

		//月表示
		{
			const auto rect = Rect(84, 0, 54, 32).movedBy(pos);
			const String text = Format(int(s.timer / 30) % 12 + 1, L"月");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(2, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0), Palette::Black);
		}

		//日表示
		{
			const auto rect = Rect(138, 0, 54, 32).movedBy(pos);
			const String text = Format(int(s.timer) % 30 + 1, L"日");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(2, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0), Palette::Black);
		}

		//時表示
		{
			const auto rect = Rect(192, 0, 54, 32).movedBy(pos);
			const String text = Format(int(s.timer * 24) % 24, L"時");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(2, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0), Palette::Black);
		}

		//分表示
		{
			const auto rect = Rect(246, 0, 54, 32).movedBy(pos);
			const String text = Format(int(s.timer * 24 * 60) % 60, L"分");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(2, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0), Palette::Black);
		}
	}


}
void	drawGraph(const Rect& _rect, Array<int> _log, int _timeScale, Color _color)
{
	int max = 1;
	int before = 0;
	int now = 0;

	for (int i = 0; i < _rect.size.x*_timeScale; i++) if (i < int(_log.size()) && max < _log[i]) max = _log[i];

	for (int x = 0; x < _rect.size.x; x++)
	{
		if ((x + 1)*_timeScale < int(_log.size()))
		{
			int sum = 0;

			for (int i = x*_timeScale; i < (x + 1)*_timeScale; i++) sum += _log[i];
			now = sum * _rect.size.y / max / _timeScale;
			Line(_rect.size.x - x, _rect.size.y - before, _rect.size.x - x - 1, _rect.size.y - now).movedBy(_rect.pos).draw(_color);
			before = now;
		}
	}
}
void	drawGraph(const Rect& _rect, Array<int> _log, int _timeScale, Color _color, int _max)
{
	int before = 0;
	int now = 0;

	for (int x = 0; x < _rect.size.x; x++)
	{
		if ((x + 1)*_timeScale < int(_log.size()))
		{
			int sum = 0;

			for (int i = x*_timeScale; i < (x + 1)*_timeScale; i++) sum += _log[i];
			now = sum * _rect.size.y / _max / _timeScale;
			Line(_rect.size.x - x, _rect.size.y - before, _rect.size.x - x - 1, _rect.size.y - now).movedBy(_rect.pos).draw(_color);
			before = now;
		}
	}
}