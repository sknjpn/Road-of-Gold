#include"Display.h"
#include"Fleet.h"
#include"VehicleData.h"

void	DisplayFleets::update()
{
	const auto frameColor = Color(20);
	const auto baseColor = Color(40);
	const auto fontColor = Color(255);
	const auto mouseOverColor = Color(70);
	const auto font16 = (*globalFonts[16]);
	const auto font24 = (*globalFonts[24]);
	const auto font36 = (*globalFonts[36]);
	const auto thickness = 4;
	auto conv = [](int value) {
		if (value < 1000) return Format(value);
		if (value > 1000) return Format(value / 1000, L"K");
		if (value > 1000000) return Format(value / 1000000, L"M");
		else return String(L"HugeValue");
	};
	Transformer2D t1;
	if (selecter.selectedFleets.isEmpty())  t1 = Transformer2D(Mat3x2::Translate(EaseOut(Easing::Expo, 0.0, -480.0, Min(1.0, closeElapsedTime.ms() / 500.0)), 0));
	else t1 = Transformer2D(Mat3x2::Translate(EaseOut(Easing::Expo, -480.0, 0.0, Min(1.0, openElapsedTime.ms() / 500.0)), 0));


	//全体枠の表示
	{
		const Rect(0, 0, 480, 720).draw(baseColor).drawFrame(thickness, frameColor);
	}

	//デザイン
	{
		Rect rect(8, 8, 464, 88);
		rect.draw(Color(80)).drawFrame(thickness, Color(60));
		font36(L"船団運用指示").drawAt(rect.center());
	}

	if (selecter.selectedFleets.isEmpty()) return;


	//船団の表示
	if (selecter.selectedFleets.size() == 1)	//１つの船団を表示する場合
	{
		Point pos(0, 96);
		auto* sf = selecter.selectedFleets.front();
		//全体枠	
		{
			Rect rect(8, 8, 464, 48);
			rect.moveBy(pos);
			rect.draw(Color(80)).drawFrame(thickness, Color(60));
		}
		//船団名
		{
			Rect rect(8, 8, 12 * 20, 24);
			rect.moveBy(pos);
			rect.draw(Color(80)).drawFrame(thickness, Color(60));
			font16(sf->name).drawAt(rect.center(), fontColor);
		}
		//船団パターン
		{
			Rect rect(8, 32, 12 * 20, 24);
			rect.moveBy(pos);
			rect.draw(Color(80)).drawFrame(thickness, Color(60));
			if (sf->chain.size() > sf->chain.readerPos && sf->chain.readerPos >= 0)
			{
				font16(sf->chain.nowRing().valueText()).drawAt(rect.center(), fontColor);
			}
		}
		//性能
		{
			{
				Rect rect(8, 160, 464, 552);
				rect.draw(Color(80)).drawFrame(thickness, Color(60));
			}
			{
				Rect rect(16, 168, 216, 36);
				rect.drawFrame(thickness, Color(60));
				font24(L"船団データ").drawAt(rect.center());
			}
			{
				Rect rect(16, 204, 104, 24);
				rect.drawFrame(thickness, Color(60));
				font16(L"航続距離").drawAt(rect.center());
			}
			{
				auto f = font16(sf->data.range*1000.0, L"mt");
				Rect rect(120, 204, 112, 24);
				rect.drawFrame(thickness, Color(60));
				f.draw(rect.tr().movedBy(-4 - int(f.region().w), 0));
			}
			{
				Rect rect(16, 228, 104, 24);
				rect.drawFrame(thickness, Color(60));
				font16(L"移動速度").drawAt(rect.center());
			}
			{
				auto f = font16(sf->data.speed);
				Rect rect(120, 228, 112, 24);
				rect.drawFrame(thickness, Color(60));
				f.draw(rect.tr().movedBy(-4 - int(f.region().w), 0));
			}
			{
				Rect rect(16, 252, 104, 24);
				rect.drawFrame(thickness, Color(60));
				font16(L"貨物容量").drawAt(rect.center());
			}
			{
				auto f = font16(sf->data.volume);
				Rect rect(120, 252, 112, 24);
				rect.drawFrame(thickness, Color(60));
				f.draw(rect.tr().movedBy(-4 - int(f.region().w), 0));
			}
		}
		//所属ユニット
		{
			{
				Rect rect(16, 284, 216, 36);
				rect.drawFrame(thickness, Color(60));
				font24(L"所属船").drawAt(rect.center());
			}
			int j = 0;
			for (int i = 0; i < int(vehicleData.size()); i++)
			{
				int	num = int(sf->ownVehicles.count_if([&i](const Vehicle* v) { return v->vehicleType == i; }));
				if (num > 0)
				{
					{
						Rect rect(16, 320 + j * 24, 104, 24);
						rect.drawFrame(thickness, Color(60));
						font16(vehicleData[i].name).drawAt(rect.center());
					}
					{
						Rect rect(120, 320 + j * 24, 112, 24);
						auto f = font16(num, L"隻");
						rect.drawFrame(thickness, Color(60));
						f.draw(rect.tr().movedBy(-4 - int(f.region().w), 0));
					}
					j++;
				}
			}
		}
		//ルート作成
		{
			{
				Rect rect(240, 168, 224, 36);
				if (rect.mouseOver()) rect.draw(Color(120));
				rect.drawFrame(thickness, Color(60));
				if (rect.leftClicked())
				{
					routeMaker.set(sf);
					selecter.selectedFleets.clear();
					closeElapsedTime.restart();
					return;
				}
				font24(L"ルート作成").drawAt(rect.center());
			}

		}
		//コマンド表示
		{
			for (int i = 0; i < int(sf->chain.size()); i++)
			{
				auto& r = sf->chain.rings[i];
				Rect rect(240, 212 + i * 24, 224, 24);
				if (int(sf->chain.readerPos) == i) rect.draw(Color(120));
				rect.drawFrame(thickness, Color(60));
				font16(r.valueText()).draw(rect.pos.movedBy(4, 0));
			}
		}
	}
	else	//複数船団を表示する場合
	{


		for (int i = 0; i < Min(11, int(selecter.selectedFleets.size())); i++)
		{
			Point pos(0, i * 56 + 56 + 40);
			auto* sf = selecter.selectedFleets[i];
			//全体枠
			{
				Rect rect(8, 8, 464, 48);
				rect.moveBy(pos);
				rect.draw(Color(80)).drawFrame(thickness, Color(60));
			}
			//船団名
			{
				Rect rect(8, 8, 12 * 20, 24);
				rect.moveBy(pos);
				rect.draw(Color(80)).drawFrame(thickness, Color(60));
				font16(sf->name).drawAt(rect.center(), fontColor);
			}
			//船団パターン
			{
				Rect rect(8, 32, 12 * 20, 24);
				rect.moveBy(pos);
				rect.draw(Color(80)).drawFrame(thickness, Color(60));
				if (sf->chain.size() > sf->chain.readerPos && sf->chain.readerPos >= 0)
				{
					font16(sf->chain.nowRing().valueText()).drawAt(rect.center(), fontColor);
				}
			}
			//選択
			{
				Rect rect(424, 8, 48, 24);
				rect.moveBy(pos);
				rect.draw(Color(rect.mouseOver() ? 100 : 80)).drawFrame(thickness, Color(60));
				if (rect.leftClicked())
				{
					selecter.selectedFleets.clear();
					selecter.selectedFleets.emplace_back(sf);
					return;
				}
				font16(L"選択").drawAt(rect.center(), fontColor);
			}
			//船団の個別
			{
				int j = 0;
				for (int k = 0; k < int(vehicleData.size()); k++)
				{
					size_t size = sf->ownVehicles.count_if([&k](Vehicle* _v) { return _v->vehicleType == k; });
					if (size > 0)
					{
						auto p = Vec2(256 + j * 64, 128 + i * 56);
						vehicleData[k].icon.resize(24, 24).draw(p);
						font16(L"x", size).draw(p.movedBy(32.0, 0.0));
						j++;
					}
				}
			}
		}
	}
}