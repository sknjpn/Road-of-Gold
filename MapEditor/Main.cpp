#include"Planet.h"
#include"Node.h"
#include"Pi.h"
/*
Road of Gold専用マップエディタ
*/

Planet planet;
int		selectedBiome = 0;
int		selectedBrush = 0;
int		brushSize = 10;

void Main()
{
	Window::SetTitle(L"MapEditor");
	Window::Resize(1280, 720);
	if (!planet.loadVoronoiMap()) return;
	const Rect uiRect(32, 32, 256, 656);
	const Font font16(16);
	bool	drawOutlineEnabled = true;
	Node*	nearestNode = &nodes[0];

	while (System::Update())
	{
		if (KeyG.down()) drawOutlineEnabled = !drawOutlineEnabled;
		if (Key1.down()) selectedBiome = 0;
		if (Key2.down()) selectedBiome = 1;
		if (Key3.down()) selectedBiome = 2;
		if (Key4.down()) selectedBiome = 3;
		if (Key5.down()) selectedBiome = 4;
		if (Key6.down()) selectedBiome = 5;
		if (Key7.down()) selectedBiome = 6;
		if (Key8.down()) selectedBiome = 7;
		if (Key9.down()) selectedBiome = 8;
		if (Key0.down()) selectedBiome = 9;

		if (KeyControl.pressed()) brushSize = Max(2, int(brushSize - Mouse::Wheel()));

		planet.updateTransform();

		//マップの描画
		for (int i = 0; i < 2; i++) {
			const auto t1 = planet.createTransformer(i);
			planet.mapTexture.resize(TwoPi, Pi).drawAt(0, 0);
			if (drawOutlineEnabled) planet.outlineTexture.resize(TwoPi, Pi).drawAt(0, 0);
		}

		if (!uiRect.mouseOver())
		{
			planet.createTransformer();

			//nearestNodeの設定
			{
				const auto& p = (planet.getCursorPos().mPos / TwoPi).movedBy(0.5, 0.25)*planet.voronoiMap.size().x;
				nearestNode = &nodes[planet.voronoiMap[int(p.y)][int(p.x)]];
			}

			//nearestNodeの描画
			for (int i = 0; i < 2; i++)
			{
				const auto t1 = planet.createTransformer(i);
				Circle(nearestNode->pos.mPos, 0.01).draw();
			}

			//selectedBiomeの反映
			if (selectedBrush == 0)
			{
				if (MouseL.pressed() && nearestNode->biomeType != selectedBiome)
				{
					Array<Node*> list = { nearestNode };
					planet.updateImage(list);

				}
			}
			else if (selectedBrush == 1)
			{
				if (MouseL.pressed())
				{
					Array<Node*> list;
					auto mp = planet.getCursorPos();
					for (auto& n : nodes)
					{
						if ((n.pos.ePos - mp.ePos).length() < 0.01*brushSize)
						{
							if (n.biomeType != selectedBiome)
							{
								list.emplace_back(&n);
							}
						}
					}
					planet.updateImage(list);
				}
			}
			else
			{
				if (MouseL.down())
				{
					Array<Node*> list;
					list.emplace_back(nearestNode);
					
					for (int i = 0; i< int(list.size());i++)
					{
						auto& n1 = list[i];
						for (auto& p : n1->paths)
						{
							auto& n2 = p.getChild();
							if (n1->biomeType == n2.biomeType && !list.any([&n2](Node* _n) {return _n == &n2; }))
							{
								list.emplace_back(&n2);
							}
						}
					}
						
					planet.updateImage(list);
				}
			}
		}

		//スライドバーの描画
		planet.updateViewPointSliding();

		//UIの描画
		uiRect.draw(Color(Palette::Gray, 128)).drawFrame(4, 0, Palette::Black);

		//バイオームセレクト
		for (int i = 0; i<int(bData.size()); i++)
		{
			auto rect = Rect(44, 44 + i * 48, 36, 36);
			rect.draw(bData[i].color).drawFrame(4, i == selectedBiome ? Palette::Red : Palette::Black);
			if (rect.mouseOver()) rect.draw(Color(Palette::Skyblue, 64)).drawFrame(4, Palette::Orange);
			if (rect.leftClicked()) selectedBiome = i;
			font16(bData[i].name).draw(88, 48 + i * 48, Palette::Black);
		}

		//ブラシセレクト
		font16(L"ブラシの選択").draw(44, 616, Palette::Black);
		for (int i = 0; i < 3; i++)
		{
			auto rect = Rect(44 + i * 64, 640, 18, 18);
			if (rect.leftClicked()) selectedBrush = i;
			rect.draw(selectedBrush == i ? Palette::Red : rect.mouseOver() ? Palette::Orange : Palette::White).drawFrame(4, Palette::Black);
		}
		font16(L"単品").draw(66, 636, Palette::Black);
		font16(L"範囲").draw(130, 636, Palette::Black);
		font16(L"塗り").draw(194, 636, Palette::Black);
	}
}
