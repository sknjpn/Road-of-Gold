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
			list.emplace_back(&n);
		}
	}
	int numUrbans, length;
	reader.read(numUrbans);
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
	BinaryWriter writer(_path + L"BiomeData.bin");
	for (auto& n : nodes) writer.write(n.biomeType);
	writer.write(int(urbans.size()));

	for (auto& u : urbans)
	{
		writer.write(u.joinedNodeID);
		writer.write(int(u.name.length()));
		writer.write(u.name.data(), int(u.name.length()) * sizeof(wchar));
		for (auto i : step(rData.size())) writer.write(u.resource[i]);
	}
	writer.close();
	return true;
}