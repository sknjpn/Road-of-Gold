#include"Node.h"
#include"Planet.h"
#include"Pi.h"
#include"JSON.h"
#include"Urban.h"

bool loadMapData(const FilePath& _path)
{
	BinaryReader reader(_path);
	Array<Node*> list;
	for (auto& n : nodes)
	{
		int t;
		reader.read(t);
		if (t != n.biomeType)
		{
			n.biomeType = t;
			if (n.biomeType >= bData.size()) return false;	//異常検出
			list.emplace_back(&n);
		}
	}
	int numUrbans, length;
	reader.read(numUrbans);
	if (numUrbans >= 1024) return false;	//異常検出
	for (; numUrbans > 0; --numUrbans)
	{
		urbans.emplace_back();
		reader.read(urbans.back().joinedNodeID);
		nodes[urbans.back().joinedNodeID].ownUrbanID = urbans.back().id;
		reader.read(length);
		urbans.back().name.resize(length);
		reader.read(&urbans.back().name[0], length * sizeof(wchar_t));
		for (auto i : step(rData.size()))
			reader.read(urbans.back().resource[i]);
	}
	planet.updateImage(list);
	return true;
}
bool saveMapData(const FilePath& _path)
{
	//バイオームデータのセーブ
	{
		BinaryWriter writer(_path + L"BiomeData.bin");
		for (auto& n : nodes) writer.write(n.biomeType);
		writer.write(int(urbans.size()));
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
			text += Format(L"\r\t\t\t\"NodePos\": ", u.joinedNodeID);

			//都市名の保存
			text += Format(L",\r\t\t\t\"Name\": \"", u.name, L"\"");

			//Resouceデータの保存
			text += L",\r\t\t\t\"Resouces\": {";
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