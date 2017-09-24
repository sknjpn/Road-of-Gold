#include"Planet.h"
#include"Node.h"
#include"Urban.h"
#include"BiomeData.h"

#include"Scuttle.h"
#include"Group.h"
#include"Nation.h"
#include"Display.h"
#include"Route.h"
#include<thread>
#include"Data.h"
#include<lua.hpp>

bool	selectMap()
{
	std::thread thread(initNodes);
	loadData();

	auto items = FileSystem::DirectoryContents(L"assets/map/").filter([](const FilePath& _path) {
		return FileSystem::IsDirectory(_path) && FileSystem::Exists(_path + L"BiomeData.bin");
	});

	for (;;)
	{
		(*display.fonts[32])(L"使用するマップを選択してください").draw();
		for (int i = 0; i < int(items.size()); i++)
		{
			Rect rect(0, 48 + i * 48, Window::Size().x, 48);
			rect.draw(rect.mouseOver() ? ColorF(Palette::White, 0.5) : Color(0, 0)).drawFrame(2, 0, Palette::White);
			(*display.fonts[32])(FileSystem::BaseName(items[i])).draw(0, 48 + i * 48);
			if (rect.leftClicked())
			{
				thread.join();
				loadMap(items[i]);
				return true;
			}
		}
		if (!System::Update()) break;
	}
	thread.join();
	return false;
}
void	loadMap(const FilePath& _path)
{
	//mapPathの登録
	planet.mapPath = _path.removed(FileSystem::CurrentPath());

	//MapImageの作成or読み込み
	auto mapImageFunc = [&_path]() {
		{
			if (FileSystem::Exists(_path + L"MapImage.png")) planet.mapTexture = Texture(_path + L"MapImage.png");
			else
			{
				//VoronoiMapの読み込み
				Image image(L"assets/nodeMap/voronoiMap.png");
				auto size = image.size();

				for (auto p : step(size))
				{
					auto& n = nodes.at(image[p.y][p.x].r + (image[p.y][p.x].g << 8) + (image[p.y][p.x].b << 16));

					image[p.y][p.x] = n.color;
				}

				//mapTextureに適用
				planet.mapTexture = Texture(image);
			}
		}
	};
	std::thread thread(mapImageFunc);

	//Planetデータのロード
	{
		JSONReader reader(_path + L"Planet.json");
		auto j = reader[L"StartTime"];

		//時間設定
		planet.sandglass.set(j[L"Year"].getOr<int>(0), j[L"Month"].getOr<int>(1), j[L"Day"].getOr<int>(1));
	}

	//バイオームデータのロード
	{
		BinaryReader reader(_path + L"BiomeData.bin");

		for (auto& n : nodes)
		{
			reader.read(n.biomeType);

			//Nodeに色の適用
			n.color = n.data().color.lerp(RandomColor(), 0.05);
		}
	}

	//Urbansデータのロード
	if (FileSystem::Exists(_path + L"Urbans.json"))
	{
		JSONReader reader(_path + L"Urbans.json");

		for (auto json : reader.arrayView()) urbans.emplace_back(json);
		for (auto& u : urbans)
		{
			for (auto& b : u.shelves) b.joinedUrban = &u;
		}
	}
	initRoutes();

	//Incidentsデータのロード
	if (FileSystem::Exists(_path + L"Incidents.lua"))
	{
		auto* lua = planet.incidentsLua;
		const auto& filePath = String(_path + L"Incidents.lua").narrow().c_str();

		if (luaL_loadfile(lua, filePath) || lua_pcall(planet.incidentsLua, 0, 0, 0))
		{
			Output << L"Incidents.luaの読み込みに失敗";
			Output << CharacterSet::FromUTF8(lua_tostring(lua, -1));
		}
	}

	//Groupsデータのロード
	if (FileSystem::Exists(_path + L"Groups.json"))
	{
		auto json = JSONReader(_path + L"Groups.json");

		for (auto j : json.arrayView()) groups.emplace_back(j);
	}

	//Nationsデータのロード
	if (FileSystem::Exists(_path + L"Nations.json"))
	{
		auto json = JSONReader(_path + L"Nations.json");

		for (auto j : json.arrayView()) nations.emplace_back(j);
		for (auto& n : nations)
		{
			for (auto& u : n.ownUrbans) u->joinedNation = &n;
		}
	}

	thread.join();	//mapImage用
}