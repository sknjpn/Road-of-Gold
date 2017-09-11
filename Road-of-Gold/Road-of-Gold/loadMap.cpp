#include"Planet.h"
#include"Node.h"
#include"Urban.h"
#include"BiomeData.h"
#include"Incident.h"
#include"Scuttle.h"
#include"Group.h"
#include"Nation.h"
#include"UI.h"

void	selectMap()
{
	auto items = FileSystem::DirectoryContents(L"assets/map/").filter([](const FilePath& _path) {
		return FileSystem::IsDirectory(_path) && FileSystem::Exists(_path + L"BiomeData.bin");
	});
	if (items.isEmpty())
	{
		LOG_ERROR(L"使用可能なマップが存在しません");
		System::Exit();
		return;
	}


	for (;;)
	{
		(*ui.fonts[32])(L"使用するマップを選択してください").draw();
		for (int i = 0; i < int(items.size()); i++)
		{
			Rect rect(0, 48 + i * 48, Window::Size().x, 48);
			rect.draw(rect.mouseOver() ? ColorF(Palette::White, 0.5) : Color(0, 0)).drawFrame(2, 0, Palette::White);
			(*ui.fonts[32])(FileSystem::BaseName(items[i])).draw(0, 48 + i * 48);
			if (rect.leftClicked())
			{
				loadMap(items[i]);
				return;
			}
		}
		if (!System::Update()) return;
	}
}
void	loadMap(const FilePath& _path)
{
	//Planetデータのロード
	{
		JSONReader reader(_path + L"Planet.json");
		auto j = reader[L"StartTime"];

		planet.sandglass.timer = j[L"Year"].getOr<int>(0) * 360 + (j[L"Month"].getOr<int>(1) - 1) * 30 + (j[L"Day"].getOr<int>(1) - 1);
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

	//MapImageの作成or読み込み
	{
		if (FileSystem::Exists(_path + L"MapImage.png"))
		{
			planet.mapTexture = Texture(_path + L"MapImage.png");
		}
		else
		{
			//VoronoiMapの読み込み
			Image image(L"assets/nodeMap/voronoiMap.png");

			for (auto p : step(image.size()))
			{
				auto& n = nodes.at(image[p.y][p.x].r + (image[p.y][p.x].g << 8) + (image[p.y][p.x].b << 16));

				image[p.y][p.x] = n.color;
			}

			//mapTextureに適用
			planet.mapTexture = Texture(image);
		}
	}

	//Urbansデータのロード
	if (FileSystem::Exists(_path + L"Urbans.json"))
	{
		JSONReader reader(_path + L"Urbans.json");

		for (auto json : reader.arrayView())
			urbans.emplace_back(json);

		for (auto& u : urbans)
			for (auto& b : u.baskets)
				b.joinedUrban = &u;
	}

	//Incidentsデータのロード
	if (FileSystem::Exists(_path + L"Incidents.json"))
	{
		incidentsJson = JSONReader(_path + L"Incidents.json");
		for (auto json : incidentsJson.arrayView())
			incidents.emplace_back(json);
	}

	//Groupsデータのロード
	if (FileSystem::Exists(_path + L"Groups.json"))
	{
		auto json = JSONReader(_path + L"Groups.json");

		for (auto j : json.arrayView())
			groups.emplace_back(j);
	}

	//Nationsデータのロード
	if (FileSystem::Exists(_path + L"Nations.json"))
	{
		auto json = JSONReader(_path + L"Nations.json");

		for (auto j : json.arrayView())
			nations.emplace_back(j);
		for (auto n : nations)
		{
			for (auto u : n.ownUrbans) u->joinedNation = &n;
		}
	}
}