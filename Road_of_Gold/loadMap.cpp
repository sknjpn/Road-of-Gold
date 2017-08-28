#include"Planet.h"
#include"Node.h"
#include"Urban.h"
#include"BiomeData.h"
#include"Incident.h"

void	loadMap()
{
	//Mapデータの選択
	auto items = FileSystem::DirectoryContents(L"Map/");
	for (auto& item : items)
	{
		if (FileSystem::IsDirectory(item) && FileSystem::Exists(item + L"BiomeData.bin"))
		{
			//バイオームデータのロード
			{
				BinaryReader reader(item + L"BiomeData.bin");
				for (auto& n : nodes)
				{
					reader.read(n.biomeType);

					//Nodeに色の適用
					n.color = n.data().color.lerp(RandomColor(), 0.05);
				}
			}

			//Urbansデータのロード
			if (FileSystem::Exists(item + L"Urbans.json"))
			{
				JSONReader reader(item + L"Urbans.json");
				for (auto json : reader[L"Urbans"].arrayView())
					urbans.emplace_back(json);

				for (auto& u : urbans)
					for (auto& b : u.baskets)
						b.joinedUrban = &u;
			}

			//Incidentsデータのロード
			if (FileSystem::Exists(item + L"Incidents.json"))
			{
				JSONReader reader(item + L"Incidents.json");
				for (auto json : reader[L"Incidents"].arrayView())
					incidents.emplace_back(json);
			}

			return;
		}
	}
	LOG_ERROR(L"有効なMapデータが見つかりませんでした");
	System::Exit();
}