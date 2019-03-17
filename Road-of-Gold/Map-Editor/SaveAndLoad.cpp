#include"Node.h"
#include"Planet.h"
#include"BiomeData.h"
#include"EnergyData.h"
#include"Urban.h"
#include"GlobalVariables.h"
#include"River.h"


bool loadMapData(const FilePath& _path)
{
	if (!FileSystem::IsDirectory(_path)) return false;

	urbans.clear();
	rivers.clear();

	//バイオームデータのロード
	if (FileSystem::Exists(_path + U"BiomeData.bin"))
	{
		Array<Node*> list;
		BinaryReader reader(_path + U"BiomeData.bin");
		for (auto& n : nodes)
		{
			reader.read(n.biomeType);
			if (n.biomeType >= int(biomeData.size())) return false;
			list.emplace_back(&n);
		}
		planet.updateImage(list);
	}
	else return false;

	//Urbansデータのロード
	if (FileSystem::Exists(_path + U"Urbans.json"))
	{
		JSONReader reader(_path + U"Urbans.json");
		for (auto json : reader.arrayView())
			urbans.emplace_back(json);
	}

	//Riversデータのロード
	if (FileSystem::Exists(_path + U"Rivers.json"))
	{
		JSONReader reader(_path + U"Rivers.json");
		for (auto json : reader.arrayView())
			rivers.emplace_back(json);
	}
	

	textBox.setText(FileSystem::BaseName(_path));

	return true;
}
bool saveMapData(const FilePath& _path)
{
	//バイオームデータのセーブ
	{
		BinaryWriter writer(_path + U"BiomeData.bin");
		for (auto& n : nodes) writer.write(n.biomeType);
	}

	//マップをpngにして保存
	{
		planet.image.save(_path + U"MapImage.png");
	}

	//Urbansデータのセーブ
	if (!urbans.isEmpty())
	{
		String text(U"[");

		for (auto i : step(int(urbans.size())))
		{
			auto& u = urbans[i];

			if (i == 0) text += U"\r\t{";
			else text += U",\r\t{";

			//都市座標の保存
			text += Format(U"\r\t\t\"JoinedNodeID\": ", u.joinedNodeID);

			//都市名の保存
			text += Format(U",\r\t\t\"Name\": \"", u.name, U"\"");

			//人口の保存
			text += Format(U",\r\t\t\"NumCitizens\": ", u.numCitizens);

			//生産性の保存
			//text += Format(U",\r\t\t\t\"Productivity\": ", u.productivity);

			//Resourcesデータの保存
			text += U",\r\t\t\"Energies\": {";
			bool isFirst = true;
			for (auto j : step(energyData.size()))
			{
				if (u.energies[j] > 0)
				{
					if (isFirst) { text += U"\r\t\t\t\""; isFirst = false; }
					else text += U",\r\t\t\t\"";
					text += Format(energyData[j].name, U"\": ", u.energies[j]);
				}
			}
			text += U"\r\t\t}";

			text += U"\r\t}";
		}
		text += U"\r]";
		TextWriter writer(_path + U"Urbans.json");
		writer.write(text);
		writer.close();
	}

	//Riversデータのセーブ
	{
		String text(U"[");
		for (auto i : step(int(rivers.size())))
		{
			auto& r = rivers[i];
			if (i == 0) text += U"\r\t{";
			else text += U",\r\t{";

			text += Format(U"\r\t\t\"Width\": ", r.width);

			text += U",\r\t\t\"Paths\": [";
			text += Format(r.riverPaths.front()->parentNodeID);
			for (auto* p : r.riverPaths)
			{
				text += U",";
				text += Format(p->childNodeID);
			}

			text += U"]";

			text += U"\r\t}";
		}
		text += U"\r]";
		TextWriter writer(_path + U"Rivers.json");
		writer.write(text);
		writer.close();
	}
	return true;
}