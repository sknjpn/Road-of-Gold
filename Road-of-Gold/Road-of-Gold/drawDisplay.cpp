#include"Display.h"
#include"Planet.h"
#include"Urban.h"
#include"ItemData.h"
#include"CitizenData.h"
#include"Route.h"
#include"Vehicle.h"
#include"Export.h"
#include"VehicleData.h"
#include"Fleet.h"
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
	selecter.update();
	selecter.draw();
	displayUrban.update();
	displayFleets.update();
	routeMaker.update();
	/*
	display.updateSelectItem();

	display.keyControlBlocked = false;
	if (display.fleetNameTextBox.isActive()) display.keyControlBlocked = true;

	//selectedRegion
	if (MouseL.down()) display.selectedRegion.pos = Cursor::Pos();
	if (MouseL.pressed())
	{
		display.selectedRegion.size = Cursor::Pos() - display.selectedRegion.pos;
		display.selectedRegion.draw(Color(Palette::Red, 128)).drawFrame(2, Palette::Red);

	}

	if (MouseL.up())
	{
		display.selectedFleets.clear();
		if (display.selectedFleetID == -1 && display.selectedUrban == nullptr)
		{
			if (display.selectedRegion.size.x < 0) { display.selectedRegion.pos.x += display.selectedRegion.size.x; display.selectedRegion.size.x = -display.selectedRegion.size.x; }
			if (display.selectedRegion.size.y < 0) { display.selectedRegion.pos.y += display.selectedRegion.size.y; display.selectedRegion.size.y = -display.selectedRegion.size.y; }
			for (int i = 0; i < 2; ++i)
			{
				for (auto& f : fleets)
				{
					if (display.selectedRegion.intersects(tinyCamera.getMat3x2(i).transform(f.pos()))) display.selectedFleets.emplace_back(&f);
				}
			}
		}
	}
	if (display.selectedUrban != nullptr || display.selectedFleetID != -1) display.selectedFleets.clear();
	if (!display.selectedFleets.isEmpty())
	{
		//右クリック移動
		for (int i = 0; i < 2; ++i)
		{
			auto t = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				if (u.rightClicked())
				{
					for (auto* f : display.selectedFleets)
					{
						//test
						f->chain.clear();
						f->setMoveTo(u);
						f->planFixed = true;
					}
				}
			}
		}

	}
	//Export
	if (KeyE.down() && !display.keyControlBlocked) display.drawExportLineEnabled = !display.drawExportLineEnabled;
	if (KeyR.down() && !display.keyControlBlocked) display.drawExportImportPowerEnabled = !display.drawExportImportPowerEnabled;
	if (display.drawExportImportPowerEnabled)
	{
		for (int i = 0; i < 2; ++i)
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

	if (display.drawExportLineEnabled)
	{
		const auto fColor = Palette::Skyblue;
		const auto bColor = Color(Palette::Darkcyan, 192);

		if (MouseL.down()) display.selectedItemType = -1;

		for (int i = 0; i<int(itemData.size()); i++)
		{
			auto& data = itemData[i];
			{
				Rect rect(32 + i * 64, 8, 64, 24);
				rect.draw(bColor).drawFrame(2, fColor);
				(*globalFonts[16])(data.name).drawAt(rect.center());
			}
			{
				Rect rect(32 + i * 64, 32, 64, 64);
				if (rect.leftClicked()) display.selectedItemType = i;
				rect.draw(rect.mouseOver() ? Palette::Orange : display.selectedItemType == i ? Palette::Red : bColor).drawFrame(2, fColor);
				data.icon.resize(64).drawAt(rect.center());
			}
		}

		for (int i = 0; i < 2; ++i)
		{
			auto t = tinyCamera.createTransformer(i);

			if (display.selectedItemType != -1)
			{
				for (auto& e : exports)
				{
					if (display.selectedItemType == e.itemType) drawArrow(*e.from, *e.to, 0.02*e.numItemPerDay, ColorF(itemData[e.itemType].color, 0.8));
				}
			}
			else
			{
				if (display.selectedUrban != nullptr)
				{
					auto* u1 = display.selectedUrban;

					for (auto& u2 : urbans)
					{
						double sum = 0.0;
						for (auto& e : exports)
						{
							if (e.from == u1 && e.to == &u2) sum += e.numItemPerDay;
						}

						drawArrow(*u1, u2, 0.02*sum, ColorF(Palette::Red, 0.8));
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

	if (KeyR.down() && !display.keyControlBlocked) display.useRouteMenu = !display.useRouteMenu;
	if (KeyU.down() && !display.keyControlBlocked) display.useUrbanMenu = !display.useUrbanMenu;
	if (display.useUrbanMenu)
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
			if (display.urbanDrawState == i) rect.draw(Palette::Red);
			if (rect.leftClicked()) display.urbanDrawState = i;
			rect.drawFrame(2, fColor);
			(*globalFonts[16])(list[i]).drawAt(rect.center());
		}

		for (int j = 0; j < int(itemData.size()); j++)
		{
			Rect rect(160 + j * 64, 0, 64, 20);
			rect.drawFrame(2, fColor);

			(*globalFonts[12])(itemData[j].name).draw(rect.pos.movedBy(4, 1));
		}
		for (int i = 0; i < int(urbans.size()); i++)
		{
			auto& u = urbans[i];
			{
				Rect rect(0, i * 20 + 20, 160, 20);
				Rect(rect.pos, int(rect.w*u.productivity), rect.h).draw(Palette::Orange);
				rect.drawFrame(2, fColor);
				(*globalFonts[12])(u.name).draw(rect.pos.movedBy(4, 1));
				if (rect.leftPressed()) u.productivity = (Cursor::Pos() - rect.pos).x / double(rect.w);
			}
			for (int j = 0; j < int(itemData.size()); j++)
			{
				auto& b = u.shelves[j];
				Rect rect(160 + j * 64, i * 20 + 20, 64, 20);

				rect.drawFrame(2, fColor);
				int sum = 0;
				switch (display.urbanDrawState)
				{
				case 0: for (int k = 0; k < 10; k++) sum += b.tradeLog.numTrade[k] * b.tradeLog.price[k]; break;
				case 1: for (int k = 0; k < 10; k++) sum += b.tradeLog.numExport[k]; break;
				case 2: for (int k = 0; k < 10; k++) sum += b.tradeLog.numImport[k]; break;
				case 3: for (int k = 0; k < 10; k++) sum += b.tradeLog.numProduction[k]; break;
				case 4: for (int k = 0; k < 10; k++) sum += b.tradeLog.numConsumption[k]; break;
				default: break;
				}
				auto s = (*globalFonts[12])(sum / 10);
				s.draw(rect.tr().movedBy(-4 - int(s.region().w), 1));
			}
		}
	}
	else if (display.useRouteMenu)
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
				(*globalFonts[12])(r->fromUrban->name, L" ", r->toUrban->name, L" ", r->numVehicles, L"隻").draw(rect.pos.movedBy(4, 1));
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
				(*globalFonts[12])(r->fromUrban->name, L" ", r->toUrban->name, L" ", r->numVehicles, L"台").draw(rect.pos.movedBy(4, 1));
			}
		}
	}
	else if (display.selectedFleetID != -1)
	{
		auto& sf = fleets[display.selectedFleetID];
		tinyCamera.gazePoint = Pos(sf.pos());

		const auto fColor = Palette::Skyblue;
		const auto bColor = Color(Palette::Darkcyan, 192);

		//右クリック移動
		for (int i = 0; i < 2; ++i)
		{
			auto t = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				if (u.rightClicked())
				{
					//test
					sf.chain.clear();
					sf.setMoveTo(u);
					sf.planFixed = true;
				}
			}
		}

		//全体枠
		{
			Rect rect(480, Window::Size().y);
			rect.draw(bColor).drawFrame(2, fColor);
		}

		//ユニット名
		{
			Rect rect(240, 32);
			rect.drawFrame(2, fColor);
			if (rect.leftClicked())
			{
				display.fleetNameTextBox.setText(sf.name);
				display.fleetNameTextBox.setActive(true);
			}
			if (display.fleetNameTextBox.isActive())
			{
				display.fleetNameTextBox.update();
				display.fleetNameTextBox.draw();
				sf.name = display.fleetNameTextBox.getText();
			}
			else (*globalFonts[24])(sf.name).drawAt(rect.center());

		}

		//ユニットセレクト
		{
			Rect rect1(240, 0, 32, 32);
			if (rect1.mouseOver()) rect1.draw(Palette::Orange);
			rect1.drawFrame(2, fColor);
			if (rect1.leftClicked())
			{
				display.fleetNameTextBox.setActive(false);
				if (KeyShift.pressed()) display.selectedFleetID -= 100;
				else if (KeyControl.pressed()) display.selectedFleetID -= 10;
				else display.selectedFleetID--;
				if (display.selectedFleetID < 0) display.selectedFleetID += int(vehicles.size());
			}
			(*globalFonts[24])(L"←").drawAt(rect1.center());

			Rect rect2(272, 0, 32, 32);
			if (rect2.mouseOver()) rect2.draw(Palette::Orange);
			rect2.drawFrame(2, fColor);
			if (rect2.leftClicked())
			{
				display.fleetNameTextBox.setActive(false);
				if (KeyShift.pressed()) display.selectedFleetID += 100;
				else if (KeyControl.pressed()) display.selectedFleetID += 10;
				else display.selectedFleetID++;
				if (display.selectedFleetID >= int(vehicles.size())) display.selectedFleetID -= int(vehicles.size());
			}
			(*globalFonts[24])(L"→").drawAt(rect2.center());

		}

		//状態
		{
			Rect rect(0, 32, 240, 24);
			rect.drawFrame(2, fColor);

			if (sf.chain.isError)
			{
				rect.draw(Color(Palette::Red, 192));
				(*globalFonts[16])(L"状態:エラー").draw(rect.pos.movedBy(4, 1));
			}
			else if (sf.route != nullptr)
			{
				auto* r = sf.route;

				RectF(rect.pos, rect.w*sf.routeProgress * sf.data.speed / r->movingCost, rect.h).draw(Color(Palette::Orange, 192));
				(*globalFonts[16])(L"状態:航行中:", r->fromUrban->name, L"-", r->toUrban->name, L"間 ", int(100.0*sf.routeProgress * sf.data.speed / r->movingCost), L"%").draw(rect.pos.movedBy(4, 1));
			}
			else if (sf.sleepTimer > 0)
			{
				(*globalFonts[16])(L"状態:停止").draw(rect.pos.movedBy(4, 2));
			}
		}

		//状態2
		{
			Rect rect(240, 32, 240, 24);
			rect.drawFrame(2, fColor);
			if (sf.cargo.numItem == 0) (*globalFonts[16])(L"積載物:").draw(rect.pos.movedBy(4, 1));
			else
			{
				int volume = int(sf.data.volume / itemData[sf.cargo.itemType].volume);
				RectF(rect.pos, rect.w*sf.cargo.numItem / volume, rect.h).draw(Color(Palette::Orange, 192));
				(*globalFonts[16])(L"積載物:", sf.cargo.data().name, L" ", sf.cargo.numItem, L"個").draw(rect.pos.movedBy(4, 1));
			}
		}

		//コマンドビュー
		{
			for (int i = 0; i < int(sf.chain.size()); i++)
			{
				auto& ring = sf.chain.rings[i];
				Rect rect1(24, 56 + 24 * i, 48, 24);
				Rect rect2(72, 56 + 24 * i, 168, 24);
				Rect rect3(0, 56 + 24 * i, 24, 24);
				rect1.drawFrame(2, fColor);
				rect2.drawFrame(2, fColor);
				rect3.drawFrame(2, fColor);
				if (int(sf.chain.readerPos) == i) rect1.draw(Color(Palette::Orange, 192));
				(*globalFonts[16])(i).drawAt(rect3.center());
				if (int(sf.chain.readerPos) == i)
				{
					switch (ring.code)
					{
					case Code::Move:
						if (sf.route != nullptr) RectF(rect2.pos, rect2.w*sf.routeProgress * sf.data.speed / sf.route->movingCost, rect2.h).draw(Color(Palette::Orange, 192));
						break;
					case Code::Buy:
						RectF(rect2.pos, rect2.w*(0.5 - sf.sleepTimer) / 0.5, rect2.h).draw(Color(Palette::Orange, 192));
						break;
					case Code::Sell:
						RectF(rect2.pos, rect2.w*(0.5 - sf.sleepTimer) / 0.5, rect2.h).draw(Color(Palette::Orange, 192));
						break;
					case Code::Wait:
						RectF(rect2.pos, rect2.w*(ring.value / 24.0 - sf.sleepTimer) / (ring.value / 24.0), rect2.h).draw(Color(Palette::Orange, 192));
						break;
					}
				}
				(*globalFonts[16])(ring.codeText()).drawAt(rect1.center());
				(*globalFonts[16])(ring.valueText()).draw(rect2.pos.movedBy(4, 1));
				if (sf.chain.isError && int(sf.chain.readerPos) == i)
				{
					rect2.draw(Color(Palette::Red, 192));
					(*globalFonts[16])(L"深刻なエラー").drawAt(rect2.center());
				}
			}
		}

		//コマンドセットビュー
		{
			for (int i = 0; i < int(display.newChain.size()); i++)
			{
				auto& ring = display.newChain.rings[i];
				Rect rect1(240, 56 + 24 * i, 48, 24);
				Rect rect2(288, 56 + 24 * i, 192, 24);
				Rect rect3(0, 56 + 24 * i, 24, 24);
				if (rect1.leftClicked())
				{
					ring.set(Code(int(ring.code) + 1), 0);
					if (ring.code == Code::ERR) ring.code = Code(0);
				}
				else if (rect1.rightClicked())
				{
					if (ring.code == Code(0)) ring.set(Code(int(Code::ERR) - 1), 0);
					else ring.set(Code(int(ring.code) - 1), 0);
				}
				if (rect1.mouseOver()) rect1.draw(Palette::Orange);
				if (rect2.mouseOver()) rect2.draw(Palette::Orange);
				rect1.drawFrame(2, fColor);
				rect2.drawFrame(2, fColor);
				rect3.drawFrame(2, fColor);
				(*globalFonts[16])(i).drawAt(rect3.center());
				switch (ring.code)
				{
				case Code::Move:
					if (rect2.leftClicked())
					{
						ring.value++;
						if (ring.value >= int(urbans.size())) ring.value = 0;
					}
					else if (rect2.rightClicked())
					{
						ring.value--;
						if (ring.value < 0) ring.value = int(urbans.size()) - 1;
					}
					break;
				case Code::Jump:
					if (rect2.leftClicked())
					{
						ring.value++;
						if (ring.value >= int(display.newChain.size())) ring.value = 0;
					}
					else if (rect2.rightClicked())
					{
						ring.value--;
						if (ring.value < 0) ring.value = int(display.newChain.size()) - 1;
					}
					break;
				case Code::Buy:
					if (rect2.leftClicked())
					{
						ring.value++;
						if (ring.value >= int(itemData.size())) ring.value = 0;
					}
					else if (rect2.rightClicked())
					{
						ring.value--;
						if (ring.value < 0) ring.value = int(itemData.size()) - 1;
					}
					break;
				case Code::Wait:
					if (rect2.leftClicked())
					{
						ring.value += KeyShift.pressed() ? 100 : KeyControl.pressed() ? 10 : 1;
					}
					else if (rect2.rightClicked())
					{
						ring.value -= KeyShift.pressed() ? 100 : KeyControl.pressed() ? 10 : 1;
						if (ring.value < 0) ring.value = 0;
					}
					break;
				}
				(*globalFonts[16])(ring.codeText()).drawAt(rect1.center());
				(*globalFonts[16])(ring.valueText()).draw(rect2.pos.movedBy(4, 1));
			}
		}

		//autoCode
		{
			{
				Rect rect(240, 296, 240, 24);
				rect.drawFrame(2, fColor);
				(*globalFonts[16])(L"自動コード生成").drawAt(rect.center());
			}
			{
				Rect rect(240, 320, 240, 24);
				if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
				rect.drawFrame(2, fColor);
				(*globalFonts[16])(sf.nowUrban->name, L"から", urbans[display.destinationUrbanID].name, L"に移動").drawAt(rect.center());
				if (rect.leftClicked())
				{
					display.destinationUrbanID++;
					if (display.destinationUrbanID >= int(urbans.size())) display.destinationUrbanID = 0;
				}
				if (rect.rightClicked())
				{
					display.destinationUrbanID--;
					if (display.destinationUrbanID < 0) display.destinationUrbanID = int(urbans.size() - 1);
				}
			}
			{
				Rect rect(240, 344, 240, 24);
				if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
				rect.drawFrame(2, fColor);
				(*globalFonts[16])(itemData[display.transportItemType].name, L"を輸送").drawAt(rect.center());
				if (rect.leftClicked())
				{
					display.transportItemType++;
					if (display.transportItemType >= int(itemData.size())) display.transportItemType = 0;
				}
				if (rect.rightClicked())
				{
					display.transportItemType--;
					if (display.transportItemType < 0) display.transportItemType = int(itemData.size() - 1);
				}
			}
			{
				Rect rect(240, 368, 240, 24);
				if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
				rect.drawFrame(2, fColor);
				(*globalFonts[16])(L"片道移動、輸送なしで生成").drawAt(rect.center());
				if (rect.leftClicked())
				{
					display.newChain.clear();
					display.newChain.rings.emplace_back(0, Code::Move, sf.nowUrban->id());
					display.newChain.rings.emplace_back(1, Code::Move, display.destinationUrbanID);
					for (int i = int(display.newChain.rings.size()); i < 10; i++) display.newChain.rings.emplace_back(i, Code::None, 0);
				}
			}
			{
				Rect rect(240, 392, 240, 24);
				if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
				rect.drawFrame(2, fColor);
				(*globalFonts[16])(L"往復移動、輸送ありで生成").drawAt(rect.center());
				if (rect.leftClicked())
				{
					display.newChain.clear();
					display.newChain.rings.emplace_back(0, Code::Move, sf.nowUrban->id());
					display.newChain.rings.emplace_back(1, Code::Sell, 0);
					display.newChain.rings.emplace_back(2, Code::Move, display.destinationUrbanID);
					display.newChain.rings.emplace_back(3, Code::Buy, display.transportItemType);
					display.newChain.rings.emplace_back(4, Code::Jump, 0);
					for (int i = int(display.newChain.rings.size()); i < 10; i++) display.newChain.rings.emplace_back(i, Code::None, 0);
				}
			}
		}
		//コピー１
		{
			Rect rect(0, 296, 240, 24);
			if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
			rect.drawFrame(2, fColor);
			(*globalFonts[16])(L"Copy→").drawAt(rect.center());
			if (rect.leftClicked())
			{
				display.newChain = sf.chain;
				display.newChain.isError = false;
				display.newChain.readerPos = 0;
			}
		}
		//コピー２
		{
			Rect rect(0, 320, 240, 24);
			if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
			rect.drawFrame(2, fColor);
			(*globalFonts[16])(L"Copy←").drawAt(rect.center());
			if (rect.leftClicked())
			{
				sf.chain = display.newChain;
				sf.chain.readerPos = 0;
				sf.chain.isError = false;
				sf.sleepTimer = 0;
			}
		}
		//設定
		{
			Rect rect(0, 344, 240, 24);
			if (sf.planFixed) rect.draw(Palette::Red);
			if (rect.mouseOver()) rect.draw(Color(Palette::Orange, 192));
			rect.drawFrame(2, fColor);
			(*globalFonts[16])(L"事業固定").drawAt(rect.center());
			if (rect.leftClicked()) sf.planFixed = !sf.planFixed;
		}
		//艦隊整理
		{
			{
				Rect rect(0, 368, 240, 24);
				rect.drawFrame(2, fColor);
				(*globalFonts[16])(L"保有機材").drawAt(rect.center());
			}

			for (int i = 0; i < int(vehicleData.size()); i++)
			{
				{
					Rect rect(0, 392 + 24 * i, 120, 24);
					rect.drawFrame(2, fColor);
					(*globalFonts[16])(vehicleData[i].name).draw(rect.pos.movedBy(4, 1));
				}
				{
					Rect rect(120, 392 + 24 * i, 60, 24);
					rect.drawFrame(2, fColor);
					(*globalFonts[16])(L"x", sf.ownVehicles.count_if([&i](Vehicle* v) { return v->vehicleType == i; })).drawAt(rect.center());
				}
			}

		}

	}
	else if (display.selectedUrban != nullptr)
	{
		auto& su = urbans[display.selectedUrbanID];

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
			(*globalFonts[24])(su.name).drawAt(rect.center());
		}

		//Route
		{
			int p = 200;

			display.selectedRoute = nullptr;

			//海路
			{
				{
					Rect rect(0, p, 240, 24);
					rect.drawFrame(2, fColor);
					(*globalFonts[16])(L"海路の一覧").drawAt(rect.center());
					p += 24;
				}

				Array<Route*> list = su.ownRoutes.filter([](const Route* r) { return r->isSeaRoute; });
				list.sort_by([](const Route* r1, const Route* r2) { return  r1->movingCost < r2->movingCost; });

				for (auto i : step(int(list.size())))
				{
					Rect rect(0, p + i * 17, 240, 17);
					auto* r = list[i];

					rect.draw(rect.mouseOver() ? Palette::Orange : Color(0, 0)).drawFrame(2, fColor);
					if (rect.mouseOver()) display.selectedRoute = r;

					const int width = int((*globalFonts[12])(int(r->movingCost * 1000), L"mt").region().w);
					(*globalFonts[12])(int(r->movingCost * 1000), L"mt").draw(rect.pos.movedBy(64 - width, 0));
					(*globalFonts[12])(r->toUrban->name).draw(rect.pos.movedBy(96, 0));
					(*globalFonts[12])(r->numVehicles, L"隻").draw(rect.pos.movedBy(160, 0));
				}
				p += int(list.size()) * 17;
			}

			p += 24;

			//陸路
			{
				{
					Rect rect(0, p, 240, 24);
					rect.drawFrame(2, fColor);
					(*globalFonts[16])(L"陸路の一覧").drawAt(rect.center());
					p += 24;
				}

				Array<Route*> list = su.ownRoutes.filter([](const Route* r) { return !r->isSeaRoute; });
				list.sort_by([](const Route* r1, const Route* r2) { return  r1->movingCost < r2->movingCost; });

				for (auto i : step(int(list.size())))
				{
					Rect rect(0, p + i * 17, 240, 17);
					auto* r = list[i];

					rect.draw(rect.mouseOver() ? Palette::Orange : Color(0, 0)).drawFrame(2, fColor);
					if (rect.mouseOver()) display.selectedRoute = r;

					const int width = int((*globalFonts[12])(int(r->movingCost * 1000), L"mt").region().w);
					(*globalFonts[12])(int(r->movingCost * 1000), L"mt").draw(rect.pos.movedBy(64 - width, 0));
					(*globalFonts[12])(r->toUrban->name).draw(rect.pos.movedBy(96, 0));
					(*globalFonts[12])(r->numVehicles, L"台").draw(rect.pos.movedBy(160, 0));

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
					(*globalFonts[12])(data.name).draw(rect.pos.movedBy(4, 0));
				}
				{
					Rect rect(80, 32 + 17 * i, 48, 17);
					rect.drawFrame(2, fColor);
					const int width = int((*globalFonts[12])(su.citizens.count_if([&i](const Citizen& c) { return c.citizenType == i; }), L"人").region().size.x);
					(*globalFonts[12])(su.citizens.count_if([&i](const Citizen& c) { return c.citizenType == i; }), L"人").draw(rect.pos.movedBy(44 - width, 0));
				}
				{
					Rect rect(128, 32 + 17 * i, 40, 17);
					rect.drawFrame(2, fColor);
					const int width = int((*globalFonts[12])(int(je * 100), L"%").region().size.x);
					(*globalFonts[12])(int(je * 100), L"%").draw(rect.pos.movedBy(36 - width, 0));
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
						const int width = int((*globalFonts[12])(sum / num, L"G").region().size.x);
						(*globalFonts[12])(sum / num, L"G").draw(rect.pos.movedBy(48 - width, 0));
					}
				}
			}
		}
		//Shelf
		{
			for (auto i : step(int(su.shelves.size())))
			{
				const auto& b = su.shelves[i];
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
					(*globalFonts[16])(data.name).draw(rect.pos.movedBy(4, 0));
					if (!b.baskets.isEmpty())
						(*globalFonts[16])(b.baskets.front().price, L"G").draw(rect.pos.movedBy(64 + 4, 0));
					(*globalFonts[16])(b.numItem, L"個").draw(rect.pos.movedBy(128 + 4, 0));
				}
			}
		}
	}
	displayUrban.update();

	//タイムスケールの変更
	{
		const auto fColor = Palette::Skyblue;
		const auto bColor = Palette::Greenyellow;
		const auto& font = *globalFonts[24];
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
	*/
	//グローバル時間の表示
	{
		const Point pos(Window::Size().x - 300, 0);
		const auto& s = planet.sandglass;
		const auto& font = *globalFonts[24];
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