#include"JSON.h"
#include"Planet.h"
#include"Node.h"
#include"Urban.h"

Array<String> GroupName;
Array<BData> bData;
Array<IData> iData;
Array<CData> cData;
Array<RData> rData;

bool loadJSONData()
{
	{
		JSONReader json(L"Assets/GroupName.json");
		if (json.isEmpty()) return false;
		for (auto j : json[L"GroupName"].arrayView()) GroupName.emplace_back(j.getOr<String>(L"hoge"));
	}
	{
		JSONReader json(L"Assets/EconomicData.json");
		if (json.isEmpty()) return false;
		for (auto r : json[L"RData"].arrayView()) rData.emplace_back(r);
		for (auto i : json[L"IData"].arrayView()) iData.emplace_back(i);
		for (auto c : json[L"CData"].arrayView()) cData.emplace_back(c);
		for (auto i : json[L"BData"].arrayView()) bData.emplace_back(i);
	}
	return true;
}

IData::IData(const JSONValue _json)
	: id(int(iData.size()))
	, name(_json[L"ItemName"].getOr<String>(L"hoge"))
	, description(_json[L"ItemDescription"].getOr<String>(L"hoge"))
	, volume(_json[L"Volume"].getOr<int>(10))
	, value(_json[L"Value"].getOr<int>(0))
{}

RData::RData(const JSONValue _json)
	: name(_json[L"ResourceName"].getOr<String>(L"hoge"))
	, description(_json[L"ResourceDescription"].getOr<String>(L"hoge"))
{}

BData::BData(const JSONValue _json)
	: name(_json[L"BiomeName"].getOr<String>(L"hoge"))
	, color(_json[L"BiomeColor"].getOr<String>(L"#000000"))
	, movingCost(_json[L"BiomeMovingCost"].getOr<double>(1.00))
{}

bool	Planet::loadBiome()
{
	auto items = FileSystem::DirectoryContents(L"Map/");

	for (auto& item : items)
	{
		if (item.indexOf(L".bin") != FilePath::npos)
		{
			BinaryReader reader(item);
			for (auto& n : nodes)
			{
				int t;
				reader.read(t);
				n.biomeType = t;
				n.color = bData[n.biomeType].color.lerp(RandomColor(), 0.05);
			}
			int numUrbans, length;
			reader.read(numUrbans);
			if (numUrbans > 256) return false;
			for (; numUrbans > 0; --numUrbans)
			{
				urbans.emplace_back();
				reader.read(urbans.back().joinedNodeID);
				nodes[urbans.back().joinedNodeID].ownUrbanID = urbans.back().id;
				reader.read(length);
				if (length > 16) return false;
				urbans.back().name.resize(length);
				reader.read(&urbans.back().name[0], length * sizeof(wchar_t));
				for (auto i : step(rData.size()))
					reader.read(urbans.back().resource[i]);
			}
			return true;
		}
	}
	return false;
}
