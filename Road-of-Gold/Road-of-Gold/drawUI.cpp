#include"UI.h"
#include"Planet.h"
#include"Urban.h"
#include"ItemData.h"
#include"CitizenData.h"
#include"Route.h"
#include"Vehicle.h"
#include"Export.h"
#include"VehicleData.h"
#include<boost\range\numeric.hpp>


void	drawArrow(const Urban& _from, const Urban& _to, double _value, Color _color)
{
	auto p = (_to.pos().mPos - _from.pos().mPos).normalized();
	auto p1 = _from.pos().mPos + p*0.02;
	auto p2 = _to.pos().mPos - p*0.02;

	Triangle(p1, p1.lerp(p2, 0.8) + _value*p.rotated(90_deg)*0.01, p1.lerp(p2, 0.8) - _value*p.rotated(90_deg)*0.01).draw(_color);
	Triangle(p2, p1.lerp(p2, 0.8) + _value*p.rotated(90_deg)*0.03, p1.lerp(p2, 0.8) - _value*p.rotated(90_deg)*0.03).draw(_color);
}
void	drawUI()
{
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
				{
					if (e.from == &u || e.to == &u) sum += e.numItemPerDay;
				}
				Circle(u.pos().mPos, 0.0020*pow(sum, 0.5)).draw(Palette::Red);
			}
		}
	}

	//矢印
	if (ui.drawExportLineEnabled)
	{
		for (int i = 0; i < 2; i++)
		{
			auto t = tinyCamera.createTransformer(i);

			if (ui.selectedItemType != -1)
			{
				for (auto& e : exports)
				{
					if (ui.selectedItemType == e.itemType)
					{
						drawArrow(*e.from, *e.to, 0.02*e.numItemPerDay, ColorF(itemData[e.itemType].color, 0.8));
					}
				}
			}
			else
			{
				if (ui.selectedUrbanID != -1)
				{
					auto& u1 = urbans[ui.selectedUrbanID];

					for (auto& u2 : urbans)
					{
						double sum = 0.0;
						for (auto& e : exports)
						{
							if (e.from == &u1 && e.to == &u2) sum += e.numItemPerDay;
						}

						drawArrow(u1, u2, 0.02*sum, ColorF(Palette::Red, 0.8));
					}
				}
				else
				{
					for (auto& u1 : urbans)
					{
						for (auto& u2 : urbans)
						{
							double sum = 0.0;
							for (auto& e : exports)
							{
								if (e.from == &u1 && e.to == &u2) sum += e.numItemPerDay;
							}

							drawArrow(u1, u2, 0.02*sum, ColorF(Palette::Red, 0.8));
						}
					}
				}
			}
		}
	}


	if (KeyR.down()) ui.useRouteMenu = !ui.useRouteMenu;
	if (KeyU.down()) ui.useUrbanMenu = !ui.useUrbanMenu;
	if (ui.useUrbanMenu)
	{
		const auto fColor = Palette::Skyblue;
		const auto bColor = Color(Palette::Darkcyan, 192);

		Array<String> list = { L"総取引量", L"輸出数", L"輸入数", L"生産数", L"消費数" };

		//全体枠
		{
			Rect rect(960, Window::Size().y);
			rect.draw(bColor).drawFrame(2, fColor);
		}

		for (int i = 0; i < int(list.size()); i++)
		{
			Rect rect(32, 480 + i * 24, 256, 24);
			if (rect.mouseOver()) rect.draw(Palette::Orange);
			if(ui.urbanDrawState == i) rect.draw(Palette::Red);
			if (rect.leftClicked()) ui.urbanDrawState = i;
			rect.drawFrame(2, fColor);
			(*ui.fonts[16])(list[i]).drawAt(rect.center());
		}

		for (int j = 0; j < int(itemData.size()); j++)
		{
			Rect rect(160 + j * 64, 0, 64, 20);
			rect.drawFrame(2, fColor);

			(*ui.fonts[12])(itemData[j].name).draw(rect.pos.movedBy(4, 1));
		}
		for (int i = 0; i < int(urbans.size()); i++)
		{
			auto& u = urbans[i];
			{
				Rect rect(0, i * 20 + 20, 160, 20);
				Rect(rect.pos, int(rect.w*u.productivity), rect.h).draw(Palette::Orange);
				rect.drawFrame(2, fColor);
				(*ui.fonts[12])(u.name).draw(rect.pos.movedBy(4, 1));
				if (rect.leftPressed()) u.productivity = (Cursor::Pos() - rect.pos).x / double(rect.w);
			}
			for (int j = 0; j < int(itemData.size()); j++)
			{
				auto& b = u.baskets[j];
				Rect rect(160 + j * 64, i * 20 + 20, 64, 20);

				rect.drawFrame(2, fColor);
				int sum = 0;
				switch (ui.urbanDrawState)
				{
				case 0:
					for (int k = 0; k < 10; k++) sum += b.tradeLog.numTrade[k] * b.tradeLog.price[k];
					break;
				case 1:
					for (int k = 0; k < 10; k++) sum += b.tradeLog.numExport[k];
					break;
				case 2:
					for (int k = 0; k < 10; k++) sum += b.tradeLog.numImport[k];
					break;
				case 3:
					for (int k = 0; k < 10; k++) sum += b.tradeLog.numProduction[k];
					break;
				case 4:
					for (int k = 0; k < 10; k++) sum += b.tradeLog.numConsumption[k];
					break;
				default:
					break;
				}
				auto s = (*ui.fonts[12])(sum / 10);
				s.draw(rect.tr().movedBy(-4 - s.region().w, 1));
			}
		}
	}
	else if (ui.useRouteMenu)
	{
		const auto fColor = Palette::Skyblue;
		const auto bColor = Color(Palette::Darkcyan, 192);

		//全体枠
		{
			Rect rect(480, Window::Size().y);
			rect.draw(bColor).drawFrame(2, fColor);
		}
		{
			Array<Route*> rs;
			for (auto& r : routes)
			{
				if (r.isSeaRoute && !rs.any([&r](const Route* t) { return t->isSeaRoute == r.isSeaRoute && t->toUrban == r.fromUrban && t->fromUrban == r.toUrban; }))
				{
					rs.emplace_back(&r);
				}
			}
			rs.sort_by([](Route* r1, Route* r2) { return r1->numVehicles > r2->numVehicles; });

			for (int i = 0; i < int(rs.size()); i++)
			{
				auto& r = rs[i];
				Rect rect(0, 20 * i, 240, 20);
				rect.drawFrame(2, fColor);
				(*ui.fonts[12])(r->fromUrban->name, L" ", r->toUrban->name, L" ", r->numVehicles, L"隻").draw(rect.pos.movedBy(4, 1));
			}
		}
		{
			Array<Route*> rs;
			for (auto& r : routes)
			{
				if (!r.isSeaRoute && !rs.any([&r](const Route* t) { return t->isSeaRoute == r.isSeaRoute && t->toUrban == r.fromUrban && t->fromUrban == r.toUrban; }))
				{
					rs.emplace_back(&r);
				}
			}
			rs.sort_by([](Route* r1, Route* r2) { return r1->numVehicles > r2->numVehicles; });

			for (int i = 0; i < int(rs.size()); i++)
			{
				auto& r = rs[i];
				Rect rect(240, 20 * i, 240, 20);
				rect.drawFrame(2, fColor);
				(*ui.fonts[12])(r->fromUrban->name, L" ", r->toUrban->name, L" ", r->numVehicles, L"台").draw(rect.pos.movedBy(4, 1));
			}
		}
	}
	else if (ui.selectedVehicleID != -1)
	{
		auto& sv = vehicles[ui.selectedVehicleID];
		tinyCamera.gazePoint.emplace(sv.pos());

		const auto fColor = Palette::Skyblue;
		const auto bColor = Color(Palette::Darkcyan, 192);

		//全体枠
		{
			Rect rect(480, Window::Size().y);
			rect.draw(bColor).drawFrame(2, fColor);
		}

		//ユニット名
		{
			Rect rect(240, 32);
			rect.drawFrame(2, fColor);
			(*ui.fonts[24])(sv.id(), L" ", sv.data().name).drawAt(rect.center());
		}

		//ユニットセレクト
		{
			Rect rect1(240, 0, 32, 32);
			if (rect1.mouseOver()) rect1.draw(Palette::Orange);
			rect1.drawFrame(2, fColor);
			if (rect1.leftClicked())
			{
				if (KeyShift.pressed()) ui.selectedVehicleID -= 100;
				else if (KeyControl.pressed()) ui.selectedVehicleID -= 10;
				else ui.selectedVehicleID--;
				if (ui.selectedVehicleID < 0) ui.selectedVehicleID += int(vehicles.size());
			}
			(*ui.fonts[24])(L"←").drawAt(rect1.center());

			Rect rect2(272, 0, 32, 32);
			if (rect2.mouseOver()) rect2.draw(Palette::Orange);
			rect2.drawFrame(2, fColor);
			if (rect2.leftClicked())
			{
				if (KeyShift.pressed()) ui.selectedVehicleID += 100;
				else if (KeyControl.pressed()) ui.selectedVehicleID += 10;
				else ui.selectedVehicleID++;
				if (ui.selectedVehicleID >= int(vehicles.size())) ui.selectedVehicleID -= int(vehicles.size());
			}
			(*ui.fonts[24])(L"→").drawAt(rect2.center());

		}

		//状態
		{
			Rect rect(0, 32, 240, 24);
			rect.drawFrame(2, fColor);

			if (sv.chain.isEmpty() || sv.reader >= int(sv.chain.size()))
			{
				rect.draw(Color(Palette::Red, 192));
				(*ui.fonts[16])(L"状態:エラー").draw(rect.pos.movedBy(4, 1));
			}
			else if (sv.route != nullptr)
			{
				auto* r = sv.route;

				RectF(rect.pos, rect.w*sv.routeProgress * sv.data().speed / r->movingCost, rect.h).draw(Color(Palette::Orange, 192));
				(*ui.fonts[16])(L"状態:航行中:", r->fromUrban->name, L"-", r->toUrban->name, L"間 ", int(100.0*sv.routeProgress * sv.data().speed / r->movingCost), L"%").draw(rect.pos.movedBy(4, 1));
			}
			else if (sv.sleepTimer > 0)
			{
				(*ui.fonts[16])(L"状態:停止").draw(rect.pos.movedBy(4, 2));
			}
		}

		//状態2
		{
			Rect rect(240, 32, 240, 24);
			rect.drawFrame(2, fColor);
			if (sv.cargo.numItem == 0) (*ui.fonts[16])(L"積載物:").draw(rect.pos.movedBy(4, 1));
			else
			{
				int volume = int(sv.data().volume / itemData[sv.cargo.itemType].volume);
				RectF(rect.pos, rect.w*sv.cargo.numItem / volume, rect.h).draw(Color(Palette::Orange, 192));
				(*ui.fonts[16])(L"積載物:", sv.cargo.data().name, L" ", sv.cargo.numItem, L"個").draw(rect.pos.movedBy(4, 1));
			}
		}

		//コマンドビュー
		{
			for (int i = 0; i < int(sv.chain.size()); i++)
			{
				auto& c = sv.chain[i];
				Rect rect1(24, 56 + 24 * i, 48, 24);
				Rect rect2(72, 56 + 24 * i, 168, 24);
				Rect rect3(0, 56 + 24 * i, 24, 24);
				if (sv.reader == i) rect1.draw(Color(Palette::Orange, 192));
				rect1.drawFrame(2, fColor);
				rect2.drawFrame(2, fColor);
				rect3.drawFrame(2, fColor);
				(*ui.fonts[16])(i).drawAt(rect3.center());
				switch (c.first)
				{
				case Code::Move:
					if (sv.reader == i && !sv.isError) RectF(rect2.pos, rect2.w*sv.routeProgress * sv.data().speed / sv.route->movingCost, rect2.h).draw(Color(Palette::Orange, 192));
					(*ui.fonts[16])(L"Move").drawAt(rect1.center());
					(*ui.fonts[16])(urbans[c.second].name, L"へ航行").draw(rect2.pos.movedBy(4, 1));
					break;
				case Code::Wait:
					(*ui.fonts[16])(L"Wait").drawAt(rect1.center());
					break;
				case Code::Jump:
					(*ui.fonts[16])(L"Jump").drawAt(rect1.center());
					(*ui.fonts[16])(c.second, L"番地").draw(rect2.pos.movedBy(4, 1));
					break;
				case Code::Buy:
					if (sv.reader == i) RectF(rect2.pos, rect2.w*(0.5 - sv.sleepTimer) / 0.5, rect2.h).draw(Color(Palette::Orange, 192));
					(*ui.fonts[16])(L"Buy").drawAt(rect1.center());
					(*ui.fonts[16])(itemData[c.second].name, L"を購入").draw(rect2.pos.movedBy(4, 1));
					break;
				case Code::Sell:
					if (sv.reader == i) RectF(rect2.pos, rect2.w*(0.5 - sv.sleepTimer) / 0.5, rect2.h).draw(Color(Palette::Orange, 192));
					(*ui.fonts[16])(L"Sell").drawAt(rect1.center());
					(*ui.fonts[16])(L"全商品の売却").draw(rect2.pos.movedBy(4, 1));
					break;
				case Code::None:
					(*ui.fonts[16])(L"None").drawAt(rect1.center());
					(*ui.fonts[16])(i + 1, L"番地に移動").draw(rect2.pos.movedBy(4, 1));
					break;
				}
				if (sv.isError && sv.reader == i)
				{
					rect2.draw(Color(Palette::Red, 192));
					(*ui.fonts[16])(L"深刻なエラー").drawAt(rect2.center());
				}
			}
		}

		//コマンドセットビュー
		{
			for (int i = 0; i < int(ui.newChain.size()); i++)
			{
				auto& c = ui.newChain[i];
				Rect rect1(240, 56 + 24 * i, 48, 24);
				Rect rect2(288, 56 + 24 * i, 192, 24);
				Rect rect3(0, 56 + 24 * i, 24, 24);
				if (rect1.leftClicked())
				{
					c = { Code(int(c.first) + 1), 0 };
					if (c.first == Code::ERR) c.first = Code(0);
				}
				if (rect1.rightClicked())
				{
					if (int(c.first) == int(Code::None)) c = { Code(int(Code::ERR) - 1), 0 };
					else c = { Code(int(c.first) - 1), 0 };
				}
				if (rect1.mouseOver()) rect1.draw(Palette::Orange);
				if (rect2.mouseOver()) rect2.draw(Palette::Orange);
				rect1.drawFrame(2, fColor);
				rect2.drawFrame(2, fColor);
				rect3.drawFrame(2, fColor);
				(*ui.fonts[16])(i).drawAt(rect3.center());
				switch (c.first)
				{
				case Code::Move:
					(*ui.fonts[16])(L"Move").drawAt(rect1.center());
					(*ui.fonts[16])(urbans[c.second].name, L"へ航行").draw(rect2.pos.movedBy(4, 1));
					if (rect2.leftClicked())
					{
						c.second++;
						if (c.second >= int(urbans.size())) c.second = 0;
					}
					if (rect2.rightClicked())
					{
						c.second--;
						if (c.second < 0) c.second = int(urbans.size()) - 1;
					}
					break;
				case Code::Wait:
					(*ui.fonts[16])(L"Wait").drawAt(rect1.center());
					break;
				case Code::Jump:
					(*ui.fonts[16])(L"Jump").drawAt(rect1.center());
					(*ui.fonts[16])(c.second, L"番地").draw(rect2.pos.movedBy(4, 1));
					if (rect2.leftClicked())
					{
						c.second++;
						if (c.second >= int(ui.newChain.size())) c.second = 0;
					}
					if (rect2.rightClicked())
					{
						c.second--;
						if (c.second < 0) c.second = int(ui.newChain.size()) - 1;
					}
					break;
				case Code::Buy:
					(*ui.fonts[16])(L"Buy").drawAt(rect1.center());
					(*ui.fonts[16])(itemData[c.second].name, L"を購入").draw(rect2.pos.movedBy(4, 1));
					if (rect2.leftClicked())
					{
						c.second++;
						if (c.second >= int(itemData.size())) c.second = 0;
					}
					if (rect2.rightClicked())
					{
						c.second--;
						if (c.second < 0) c.second = int(itemData.size()) - 1;
					}
					break;
				case Code::Sell:
					(*ui.fonts[16])(L"Sell").drawAt(rect1.center());
					(*ui.fonts[16])(L"全商品の売却").draw(rect2.pos.movedBy(4, 1));
					break;
				case Code::None:
					(*ui.fonts[16])(L"None").drawAt(rect1.center());
					(*ui.fonts[16])(i + 1, L"番地に移動").draw(rect2.pos.movedBy(4, 1));
					break;
				}
			}

			//コピー１
			{
				Rect rect(0, 296, 240, 24);
				if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
				rect.drawFrame(2, fColor);
				(*ui.fonts[16])(L"Copy→").drawAt(rect.center());
				if (rect.leftClicked()) ui.newChain = sv.chain;
			}
			//コピー２
			{
				Rect rect(0, 320, 240, 24);
				if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
				rect.drawFrame(2, fColor);
				(*ui.fonts[16])(L"Copy←").drawAt(rect.center());
				if (rect.leftClicked())
				{
					sv.chain = ui.newChain;
					sv.isError = false;
				}
			}
			//設定
			{
				Rect rect(0, 344, 240, 24);
				if (sv.planFixed) rect.draw(Palette::Red);
				if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
				rect.drawFrame(2, fColor);
				(*ui.fonts[16])(L"事業固定").drawAt(rect.center());
				if (rect.leftClicked()) sv.planFixed = !sv.planFixed;
			}
		}
	}
	else if (ui.selectedUrbanID != -1)
	{
		auto& su = urbans[ui.selectedUrbanID];

		const auto fColor = Palette::Skyblue;
		const auto bColor = Color(Palette::Darkcyan, 192);

		//全体枠
		{
			Rect rect(480, Window::Size().y);
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

			ui.selectedRoute = nullptr;

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
					auto* r = list[i];

					rect.draw(rect.mouseOver() ? Palette::Orange : Color(0, 0)).drawFrame(2, fColor);
					if (rect.mouseOver()) ui.selectedRoute = r;

					const int width = int((*ui.fonts[12])(int(r->movingCost * 1000), L"mt").region().w);
					(*ui.fonts[12])(int(r->movingCost * 1000), L"mt").draw(rect.pos.movedBy(64 - width, 0));
					(*ui.fonts[12])(r->toUrban->name).draw(rect.pos.movedBy(96, 0));
					(*ui.fonts[12])(r->numVehicles, L"隻").draw(rect.pos.movedBy(160, 0));
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
					auto* r = list[i];

					rect.draw(rect.mouseOver() ? Palette::Orange : Color(0, 0)).drawFrame(2, fColor);
					if (rect.mouseOver()) ui.selectedRoute = r;

					const int width = int((*ui.fonts[12])(int(r->movingCost * 1000), L"mt").region().w);
					(*ui.fonts[12])(int(r->movingCost * 1000), L"mt").draw(rect.pos.movedBy(64 - width, 0));
					(*ui.fonts[12])(r->toUrban->name).draw(rect.pos.movedBy(96, 0));
					(*ui.fonts[12])(r->numVehicles, L"台").draw(rect.pos.movedBy(160, 0));

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
					rect.drawFrame(2, fColor);
					(*ui.fonts[12])(data.name).draw(rect.pos.movedBy(4, 0));
				}
				{
					Rect rect(80, 32 + 17 * i, 48, 17);
					rect.drawFrame(2, fColor);
					const int width = int((*ui.fonts[12])(su.citizens.count_if([&i](const Citizen& c) { return c.citizenType == i; }), L"人").region().size.x);
					(*ui.fonts[12])(su.citizens.count_if([&i](const Citizen& c) { return c.citizenType == i; }), L"人").draw(rect.pos.movedBy(44 - width, 0));
				}
				{
					Rect rect(128, 32 + 17 * i, 40, 17);
					rect.drawFrame(2, fColor);
					const int width = int((*ui.fonts[12])(int(je * 100), L"%").region().size.x);
					(*ui.fonts[12])(int(je * 100), L"%").draw(rect.pos.movedBy(36 - width, 0));
				}
				{
					int sum = 0, num = 0;
					for (auto& c : su.citizens)
					{
						if (c.citizenType == i)
						{
							sum += c.averageIncome;
							num++;
						}
					}
					if (num > 0)
					{
						Rect rect(168, 32 + 17 * i, 52, 17);
						rect.drawFrame(2, fColor);
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
					Rect rect(240, 24 + i * 88, 240, 64);
					const int timeScale = 1;

					int max = 1;
					for (int j = 0; j < rect.size.x; j++)
					{
						max = Max(max, b.tradeLog.numConsumption[j]);
						max = Max(max, b.tradeLog.numExport[j]);
						max = Max(max, b.tradeLog.numImport[j]);
						max = Max(max, b.tradeLog.numProduction[j]);
					}

					drawGraph(rect, b.tradeLog.numExport, timeScale, Palette::Blue, max);
					drawGraph(rect, b.tradeLog.numImport, timeScale, Palette::Purple, max);
					drawGraph(rect, b.tradeLog.numConsumption, timeScale, Palette::Yellowgreen, max);
					drawGraph(rect, b.tradeLog.numProduction, timeScale, Palette::Red, max);

					drawGraph(rect, b.tradeLog.price, timeScale, Palette::Yellow);
					data.icon.draw(rect.pos);

					{
						int nex = std::accumulate(b.tradeLog.numExport.begin() + 1, b.tradeLog.numExport.begin() + 11, 0);
						int nco = std::accumulate(b.tradeLog.numConsumption.begin() + 1, b.tradeLog.numConsumption.begin() + 11, 0);
						if (nex + nco > 0) Circle(336, 56 + i * 88, 24).draw(Palette::Yellowgreen).drawPie(0, nex * 360_deg / (nex + nco), Palette::Blue);
					}

					rect.drawFrame(2, fColor);
				}

				{
					Rect rect(240, i * 88, 240, 24);
					rect.drawFrame(2, fColor);
					(*ui.fonts[16])(data.name).draw(rect.pos.movedBy(4, 0));
					if (!b.rings.isEmpty())
						(*ui.fonts[16])(b.rings.front().price, L"G").draw(rect.pos.movedBy(64 + 4, 0));
					(*ui.fonts[16])(b.numItem, L"個").draw(rect.pos.movedBy(128 + 4, 0));
				}
			}
		}
	}
	else //ItemSelect
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