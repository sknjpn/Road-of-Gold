#include"Node.h"
#include"Planet.h"
#include"Pi.h"
#include"JSON.h"
#include"Urban.h"
#include"GlobalVariables.h"

Urban::Urban(const JSONValue _json)
	: id(int(urbans.size()))
	, joinedNodeID(_json[L"JoinedNodeID"].getOr<int>(-1))
	, name(_json[L"Name"].getOr<String>(L"hoge"))
	, numCitizens(_json[L"NumCitizens"].getOr<int>(1))
{
	nodes[joinedNodeID].ownUrbanID = id;
	resource.resize(rData.size());

	for (auto i : step(int(rData.size())))
	{
		if (!_json[L"Resources." + rData[i].name].isEmpty())
		{
			resource[i] = _json[L"Resources." + rData[i].name].getOr<int>(10);
		}
	}
}

bool loadMapData(const FilePath& _path)
{
	if (!FileSystem::IsDirectory(_path)) return false;

	urbans.clear();

	//バイオームデータのロード
	if (FileSystem::Exists(_path + L"BiomeData.bin"))
	{
		Array<Node*> list;
		BinaryReader reader(_path + L"BiomeData.bin");
		for (auto& n : nodes)
		{
			reader.read(n.biomeType);
			if (n.biomeType >= bData.size()) return false;
			list.emplace_back(&n);
		}
		planet.updateImage(list);
	}
	else return false;

	//Urbansデータのロード
	if (FileSystem::Exists(_path + L"Urbans.json"))
	{
		JSONReader reader(_path + L"Urbans.json");
		for (auto json : reader[L"Urbans"].arrayView())
			urbans.emplace_back(json);
	}

	textBox.setText(FileSystem::BaseName(_path));
	
	return true;
}
bool saveMapData(const FilePath& _path)
{
	//バイオームデータのセーブ
	{
		BinaryWriter writer(_path + L"BiomeData.bin");
		for (auto& n : nodes) writer.write(n.biomeType);
	}

	//Urbansデータのセーブ
	if (!urbans.isEmpty())
	{
		String text(L"{\r\t\"Urbans\": [");
		for (auto i : step(int(urbans.size())))
		{
			auto& u = urbans[i];
			if (i == 0) text += L"\r\t\t{";
			else text += L",\r\t\t{";

			//都市座標の保存
			text += Format(L"\r\t\t\t\"JoinedNodeID\": ", u.joinedNodeID);

			//都市名の保存
			text += Format(L",\r\t\t\t\"Name\": \"", u.name, L"\"");

			//人口の保存
			text += Format(L",\r\t\t\t\"NumCitizens\": ", u.numCitizens);

			//Resourcesデータの保存
			text += L",\r\t\t\t\"Resources\": {";
			bool isFirst = true;
			for (auto j : step(rData.size()))
			{
				if (u.resource[j] > 0)
				{
					if (isFirst) { text += L"\r"; isFirst = false; }
					else text += L",\r";
					text += Format(L"\t\t\t\t\"", rData[j].name, L"\": ", u.resource[j]);
				}
			}
			text += L"\r\t\t\t}";

			text += L"\r\t\t}";
		}
		text += L"\r\t]\r}";
		TextWriter writer(_path + L"Urbans.json");
		writer.write(text);
		writer.close();
	}
	return true;
}