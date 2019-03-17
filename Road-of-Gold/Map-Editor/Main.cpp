#include"Planet.h"
#include"Node.h"
#include"Urban.h"
#include"TinyCamera.h"
#include"Data.h"
#include"EnergyData.h"
#include"BiomeData.h"
#include"VehicleData.h"
#include"ItemData.h"
#include"River.h"
/*
Road of Gold専用マップエディタ
*/

Array<Urban> urbans;
Planet	planet;
TinyCamera tinyCamera;
int		selectedBiome = 0;
int		selectedBrush = 0;
int		brushSize = 10;
Urban*	selectedUrban = nullptr;

//ファイル名入力欄
TextBox textBox;
bool	keyControlEnabled = true;

Array<std::pair<Texture, String>> textureAssets;

Texture*	getTexture(const String& _name)
{
	for (auto& tex : textureAssets)
	{
		if (tex.second == _name) return &tex.first;
	}
	return nullptr;
}
void	addTexture(const FilePath& _path, const String& _name)
{
	textureAssets.emplace_back(Texture(_path), _name);
}


void Main()
{
	addTexture(U"assets/image/button/minus.png", U"minus");
	addTexture(U"assets/image/button/minusMouseOver.png", U"minusMouseOver");
	addTexture(U"assets/image/button/plus.png", U"plus");
	addTexture(U"assets/image/button/plusMouseOver.png", U"plusMouseOver");

	{
		Window::SetTitle(U"MapEditor");

		INIData iniReader(U"assets/config.ini");
		if (iniReader.getOr<bool>(U"Window", U"FullScreen", false)) Graphics::SetFullScreen(true, Size(1920, 1080));
		else Window::Resize(iniReader.getOr<Size>(U"Window", U"WindowSize", Size(1280, 720)));
	}

	const Rect uiRect(32, 32, 320, 720 - 64);
	const Font font12(12);
	const Font font16(16);
	const Font font24(24);
	const Font textBoxFont(12, Typeface::Bold);

	enum struct ActionMode {
		none,
		set,
		remove,
	} actionMode = ActionMode::set;

	enum struct UIMode {
		setBiome,
		setUrban,
		saveAndLoad,
		setRiver,
	} uiMode = UIMode::setBiome;

	bool	drawOutlineEnabled = true;
	Node*	nearestNode = &nodes[0];

	loadData();

	//データの読み込み
	if (!planet.loadVoronoiMap()) return;

	textBox = TextBox(textBoxFont, Vec2(160, 72), 120);

	//都市名入力欄
	TextBox urbanNameTextBox(textBoxFont, Vec2(78, 66), 112);

	//人口入力欄
	TextBox numCitizensTextBox(textBoxFont, Vec2(78, 86), 112);


	while (System::Update())
	{

		if (numCitizensTextBox.isActive() ||
			urbanNameTextBox.isActive() ||
			textBox.isActive()) keyControlEnabled = false;
		else keyControlEnabled = true;

		//キー入力
		if (keyControlEnabled)
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
			if (KeyR.down()) selectedBrush = 0;
			if (KeyF.down()) selectedBrush = 1;
			if (KeyV.down()) selectedBrush = 2;
		}

		//ブラシサイズの変更
		if (KeyControl.pressed()) brushSize = Max(2, int(brushSize - Mouse::Wheel()));

		//カメラの更新
		tinyCamera.update();

		planet.setCoverImage();

		//マップの描画
		for (int i = 0; i < 2; ++i) {
			const auto t1 = tinyCamera.createTransformer(i);

			planet.mapTexture.resized(360_deg, 180_deg).drawAt(0, 0);
			if (drawOutlineEnabled) planet.outlineTexture.resized(360_deg, 180_deg).drawAt(0, 0);
			if (planet.coverTexture) planet.coverTexture.resized(360_deg, 180_deg).drawAt(0, 0, ColorF(Palette::White, planet.coverRate));
		}

		//川の描画
		for (int i = 0; i < 2; ++i) {
			const auto t1 = tinyCamera.createTransformer(i);

			for (auto& r : rivers)
			{
				r.draw();
			}
		}

		//都市の描画
		for (int i = 0; i < 2; ++i) {
			const auto t1 = tinyCamera.createTransformer(i);

			for (auto& u : urbans)
			{
				Circle(u.getPos().mPos, 0.012).draw(Palette::Red).drawFrame(0.002, 0.0, Palette::Black);
			}
		}

		//selectedUrbanの描画
		if (selectedUrban != nullptr && uiMode == UIMode::setUrban)
		{
			for (int i = 0; i < 2; ++i) {
				const auto t1 = tinyCamera.createTransformer(i);

				Circle(selectedUrban->getPos().mPos, 0.012).draw(Palette::Yellow).drawFrame(0.002, 0.0, Palette::Black);
			}
		}
		else selectedUrban = nullptr;

		if (!uiRect.mouseOver())
		{
			//nearestNodeの設定
			{
				const auto& p = (tinyCamera.getCursorPos().mPos / 360_deg).movedBy(0.5, 0.25)*planet.voronoiMap.size().x;
				nearestNode = &nodes[planet.voronoiMap[int(p.y)][int(p.x)]];
			}

			//nearestNodeの描画
			for (int i = 0; i < 2; ++i)
			{
				const auto t1 = tinyCamera.createTransformer(i);
				Circle(nearestNode->pos.mPos, 0.01).drawFrame(0.003, Palette::Black);
			}

			switch (uiMode)
			{
			case UIMode::setBiome:

				//色の取得
				if (MouseR.down()) selectedBiome = nearestNode->biomeType;

				//ブラシの使用
				switch (selectedBrush)
				{
				case 0:
					if (MouseL.pressed() && nearestNode->biomeType != selectedBiome)
					{
						Array<Node*> list = { nearestNode };
						nearestNode->biomeType = selectedBiome;
						planet.updateImage(list);
					}
					break;

				case 1:
					if (MouseL.pressed())
					{
						Array<Node*> list;
						auto mp = tinyCamera.getCursorPos();
						for (auto& n : nodes)
						{
							if ((n.pos.ePos - mp.ePos).length() < 0.01*brushSize)
							{
								if (n.biomeType != selectedBiome)
								{
									n.biomeType = selectedBiome;
									list.emplace_back(&n);
								}
							}
						}
						planet.updateImage(list);
					}
					break;

				case 2:
					if (MouseL.down() && nearestNode->biomeType != selectedBiome)
					{
						Array<Node*> list;
						list.emplace_back(nearestNode);
						for (int i = 0; i < int(list.size()); ++i)
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
						for (auto& n : list) n->biomeType = selectedBiome;
						planet.updateImage(list);
					}
					break;
				}

				break;

			case UIMode::setUrban:

				if (MouseR.down())
				{
					selectedUrban = nullptr;
					actionMode = ActionMode::none;
				}

				switch (actionMode)
				{
				case ActionMode::none:
					if (MouseL.down() && nearestNode->ownUrbanID() != -1)
					{
						selectedUrban = &urbans[nearestNode->ownUrbanID()];
						urbanNameTextBox.setText(selectedUrban->name);
						numCitizensTextBox.setText(Format(selectedUrban->numCitizens));
					}
					break;
				case ActionMode::set:
					if (MouseL.down() && nearestNode->ownUrbanID() == -1)
					{
						urbans.emplace_back(nearestNode->id());
						selectedUrban = &urbans.back();
						urbanNameTextBox.setText(selectedUrban->name);
						numCitizensTextBox.setText(Format(selectedUrban->numCitizens));
					}
					break;
				case ActionMode::remove:
					if (MouseL.down() && nearestNode->ownUrbanID() != -1)
					{
						const int targetID = nearestNode->ownUrbanID();
						urbans.remove_if([&nearestNode](Urban& u) {return nearestNode->ownUrbanID() == u.id(); });
						selectedUrban = nullptr;
					}
					break;
				}

				break;
			}
		}


		//UIの描画
		uiRect.draw(Color(Palette::Darkcyan, 192)).drawFrame(2, Palette::Skyblue);

		//UIModeの選択
		{
			const Array<String> ns = { U"🌍", U"🏭", U"📂", U"🚤" };
			for (auto i : step(int(ns.size())))
			{
				const int width = 320 / int(ns.size());
				const Rect rect(32 + width*i, 32, width, 32);
				if (rect.leftClicked()) uiMode = UIMode(i);
				rect.draw(int(uiMode) == i ? Palette::Red : rect.mouseOver() ? Palette::Orange : Color(0, 0)).drawFrame(2, Palette::Skyblue);
				font24(ns[i]).drawAt(rect.center());
			}
		}
		switch (uiMode)
		{
		case UIMode::setBiome:
		{
			//一覧
			for (auto i : step(20))
			{
				const Rect rect(32, 64 + i * 19, 160, 19);
				const Color color(selectedBiome == i ? Palette::Red : rect.mouseOver() ? Palette::Orange : Color(0, 0));
				if (i < int(biomeData.size()))
				{
					if (rect.leftClicked()) selectedBiome = i;
					rect.draw(biomeData[i].color);
					rect.drawFrame(3, 0, color);

					font12(biomeData[i].nameJP).draw(rect.pos.movedBy(11, 1), Palette::Black);
					font12(biomeData[i].nameJP).draw(rect.pos.movedBy(13, 1), Palette::Black);
					font12(biomeData[i].nameJP).draw(rect.pos.movedBy(12, 0), Palette::Black);
					font12(biomeData[i].nameJP).draw(rect.pos.movedBy(12, 2), Palette::Black);
					font12(biomeData[i].nameJP).draw(rect.pos.movedBy(12, 1), Palette::White);
				}
				else
				{
					if (color != Color(0, 0)) rect.draw(Color(color, 128));
					font12(U"---").draw(rect.pos.movedBy(12, 1));
				}
				rect.drawFrame(2, Palette::Skyblue);
			}
			//詳細
			{
				const Rect rect(192, 64, 160, 40);
				rect.drawFrame(2, Palette::Skyblue);
				font16(U"選択中のバイオーム").draw(rect.pos.movedBy(4, 0));
				font12(biomeData[selectedBiome].nameJP).draw(rect.pos.movedBy(4, 22));
			}
			//ブラシの選択
			{
				const Rect rect(192, 104, 160, 128);
				rect.drawFrame(2, Palette::Skyblue);
				font16(U"ブラシの選択").draw(rect.pos.movedBy(4, 0));
				const Array<String> brushName = { U"鉛筆:KeyR", U"筆:KeyF", U"バケツ:KeyV" };
				for (auto i : step(int(brushName.size())))
				{
					const Rect s(rect.pos.movedBy(6, 26 + i * 20), 16, 16);
					if (s.leftClicked()) selectedBrush = i;
					s.draw(selectedBrush == i ? Palette::Red : s.mouseOver() ? Palette::Orange : Palette::White).drawFrame(2, 0, Palette::Black);
					font12(brushName[i]).draw(s.pos.movedBy(24, 0));
				}
				font12(U"太さ:", brushSize).draw(rect.pos.movedBy(4, 94));
				font12(U"太さはCtrl+Wheelで変更可").draw(rect.pos.movedBy(4, 110));
			}
			//生成
			{
				const Rect rect(192, 232, 160, 24);
				rect.drawFrame(2, Palette::Skyblue);
				const Rect s(rect.pos.movedBy(136, 4), 16, 16);
				if (s.leftClicked()) planet.generateBiome();
				s.draw(s.mouseOver() ? Palette::Orange : Palette::White).drawFrame(2, 0, Palette::Black);
				font16(U"マップの自動生成").draw(rect.pos.movedBy(4, 0));
			}
			//セーブ
			{
				const Rect rect(192, 256, 160, 48);
				textBox.setPos(rect.pos.movedBy(4, 28));
				rect.drawFrame(2, Palette::Skyblue);
				const Rect s(rect.pos.movedBy(136, 4), 16, 16);
				if (s.leftClicked())
				{
					FilePath filePath = U"assets/map/" + textBox.getText() + U"/";
					saveMapData(filePath);
				}
				s.draw(s.mouseOver() ? Palette::Orange : Palette::White).drawFrame(2, 0, Palette::Black);
				font16(U"セーブ").draw(rect.pos.movedBy(4, 0));
				textBox.update();
				textBox.draw();
			}
			//CoverRate
			{
				const Rect rect(192, 304, 160, 24);
				rect.drawFrame(2, Palette::Skyblue);
				Line(212, 316, 332, 316).draw(4, Palette::White);
				Rect(210 + int(planet.coverRate * 120), 308, 4, 16).draw(Palette::Black);
				if (rect.leftPressed())
				{
					planet.coverRate = Min(1.0, Max(0.0, double(Cursor::Pos().x - 210) / 120.0));
				}
			}

			break;
		}
		case UIMode::setUrban:
		{
			{
				const Rect rect(192, 64, 160, 20);
				rect.drawFrame(2, Palette::Skyblue);
				const Rect s(rect.pos.movedBy(2, 2), 16, 16);
				if (s.leftClicked()) actionMode = actionMode == ActionMode::set ? ActionMode::none : ActionMode::set;
				s.draw(actionMode == ActionMode::set ? Palette::Red : s.mouseOver() ? Palette::Orange : Palette::White).drawFrame(2, 0, Palette::Black);
				font12(U"都市配置モード").draw(rect.pos.movedBy(28, 1));
			}
			{
				const Rect rect(192, 84, 160, 20);
				rect.drawFrame(2, Palette::Skyblue);
				const Rect s(rect.pos.movedBy(2, 2), 16, 16);
				if (s.leftClicked()) actionMode = actionMode == ActionMode::remove ? ActionMode::none : ActionMode::remove;
				s.draw(actionMode == ActionMode::remove ? Palette::Red : s.mouseOver() ? Palette::Orange : Palette::White).drawFrame(2, 0, Palette::Black);
				font12(U"都市削除モード").draw(rect.pos.movedBy(28, 1));
			}
			{
				urbanNameTextBox.update();
				if (selectedUrban != nullptr) selectedUrban->name = urbanNameTextBox.getText();
				else urbanNameTextBox.setText(U"");
				urbanNameTextBox.draw();

				const Rect rect(32, 64, 44, 20);
				rect.drawFrame(2, Palette::Skyblue);
				font12(U"都市名").draw(rect.pos.movedBy(4, 1));
			}
			for (auto& i : step(int(energyData.size())))
			{
				const Rect rect(32, 104 + i * 20, 72, 20);
				rect.drawFrame(2, Palette::Skyblue);
				font12(energyData[i].name).draw(rect.pos.movedBy(4, 1));
			}
			for (auto& i : step(int(energyData.size())))
			{
				const Rect rect(104, 104 + i * 20, 100, 20);
				auto& data = energyData[i];

				data.icon.resized(16, 16).draw(106, 106 + i * 20);

				Point p1(124, 106 + i * 20);
				Point p2(144, 106 + i * 20);

				if (selectedUrban != nullptr)
				{
					auto& energy = selectedUrban->energies[i];
					const int amount = KeyShift.pressed() ? 100 : KeyControl.pressed() ? 10 : 1;

					if (Rect(p1, 16, 16).leftClicked()) energy -= amount;
					if (Rect(p2, 16, 16).leftClicked()) energy += amount;
					if (energy < 0) energy = 0;

					const int width = int(font12(energy).region().size.x);
					font12(energy).draw(200 - width, 106 + i * 20);
				}
				if (Rect(p1, 16, 16).mouseOver()) getTexture(U"minusMouseOver")->resized(16, 16).draw(p1);
				else getTexture(U"minus")->resized(16, 16).draw(p1);
				if (Rect(p2, 16, 16).mouseOver()) getTexture(U"plusMouseOver")->resized(16, 16).draw(p2);
				else getTexture(U"plus")->resized(16, 16).draw(p2);

				rect.drawFrame(2, Palette::Skyblue);
			}
			font12(U"Ctrl :  x10").draw(208, 108);
			font12(U"Shift: x100").draw(208, 124);
			if (selectedUrban != nullptr && selectedUrban->energies[0] == 0)
			{
				font12(U"都市に農地がない場合").draw(208, 140, Palette::Red);
				font12(U"資金生産が出来ません").draw(208, 156, Palette::Red);
			}


			//都市人口の設定
			{
				numCitizensTextBox.update();
				if (selectedUrban != nullptr)
				{
					selectedUrban->numCitizens = Min(ParseInt<int>(numCitizensTextBox.getText(), Arg::radix = 10), 10000);
					numCitizensTextBox.setText(Format(selectedUrban->numCitizens));
				}
				else numCitizensTextBox.setText(U"");
				numCitizensTextBox.draw();
				const Rect rect(32, 84, 44, 20);
				rect.drawFrame(2, Palette::Skyblue);
				font12(U"人口").draw(rect.pos.movedBy(4, 1));

			}
			break;
		}

		case UIMode::saveAndLoad:
		{
			font16(U"クリックしただけで読み込みが開始されます").draw(32, 64);

			//ロードファイルリストの表示
			{
				auto items = FileSystem::DirectoryContents(U"assets/map/");
				items.remove_if([](FilePath& _item) {
					return !FileSystem::IsDirectory(_item) || !FileSystem::Exists(_item + U"BiomeData.bin");
				});
				for (auto i : step(int(items.size())))
				{
					auto item = items[i].remove(FileSystem::CurrentPath());
					Rect rect(32, 88 + i * 16, 160, 16);
					if (rect.mouseOver()) rect.draw(Palette::Orange);
					if (rect.leftClicked())
					{
						/*	オートセーブ機能
						for (int j = 0;; j++)
						{
							if (!FileSystem::Exists(Format(U"assets/map/_autosave/", j, U"/")))
							{
								saveMapData(Format(U"assets/map/_autosave/", j, U"/"));
								break;
							}
						}
						*/
						loadMapData(item);
					}
					rect.drawFrame(2, Palette::Skyblue);
					font12(item).draw(rect.pos.movedBy(4, 0));
				}
			}

			break;
		}

		case UIMode::setRiver:
		{
			for (int i = 0; i < int(rivers.size()); i++)
			{
				auto& r = rivers[i];
				{
					Rect rect(32, 64 + i * 24, 320, 24);
					rect.drawFrame(2, Palette::Skyblue);
					if (rect.mouseOver()) r.isMouseOver = true;
					else r.isMouseOver = false;
					font16(U"幅", r.width, U"長さ", r.riverPaths.size()).draw(rect.pos.movedBy(4, 0));
				}
				{
					Rect rect(184, 64 + i * 24, 120, 24);
					if (rect.leftPressed()) r.width = (Cursor::PosF().x - 184.0) / 6000.0;
					Line(rect.pos.movedBy(int(r.width * 6000), 0), rect.pos.movedBy(int(r.width * 6000), 24)).draw(4, Palette::Red);
					Line(184, 76 + i * 24, 304, 76 + i * 24).draw(4, Palette::Black);
					rect.drawFrame(2, Palette::Skyblue);
				}
				{
					Rect rect(32 + 320 - 48, 64 + i * 24, 48, 24);
					if (rect.mouseOver()) rect.draw(Palette::Orange);
					if (rect.leftClicked()) rivers.erase(rivers.begin() + i);
					rect.drawFrame(2, Palette::Skyblue);
					font16(U"削除").drawAt(rect.center());

				}
			}
			if (!uiRect.mouseOver())
			{
				if (MouseL.down())
				{
					rivers.emplace_back(0.01, nearestNode);
				}
				if (MouseL.pressed())
				{
					auto& r = rivers.back();
					if (r.lastNode != nearestNode)
					{
						bool flag = false;
						for (auto& p : r.lastNode->paths)
						{
							if (&p.getChild() == nearestNode)
							{
								flag = true;
								r.riverPaths.emplace_back(&p);
								r.lastNode = nearestNode;
								break;
							}
						}
						//つなげられない場合、新River生成
						if (!flag) rivers.emplace_back(0.01, nearestNode);
					}
				}
			}
			break;
		}

		}

		//ロード
		auto items = DragDrop::GetDroppedFilePaths();
		for (auto item : items)
		{
			Logger << item.path;
			if (loadMapData(item.path)) break;
		}
	}
}
