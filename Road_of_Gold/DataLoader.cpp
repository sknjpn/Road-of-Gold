#include"JSON.h"
#include"Planet.h"
#include"Node.h"
#include"Urban.h"
#include"GlobalVariables.h"

Array<String> GroupName;
Array<BData> bData;
Array<IData> iData;
Array<CData> cData;
Array<RData> rData;
Array<VData> vData;

bool loadJSONData()
{
	//グループ名の読み込み
	JSONReader json(L"Assets/GroupName.json");
	if (json.isEmpty()) return false;
	for (auto j : json[L"GroupName"].arrayView()) GroupName.emplace_back(j.getOr<String>(L"hoge"));

	//各種JSONデータの読み込み
	JSONReader rjson(L"Assets/ResourceData.json");
	JSONReader ijson(L"Assets/ItemData.json");
	JSONReader cjson(L"Assets/CitizenData.json");
	JSONReader bjson(L"Assets/BiomeData.json");
	JSONReader vjson(L"Assets/VehicleData.json");
	for (auto v : vjson[L"VehicleData"].arrayView()) vData.emplace_back(v);
	for (auto r : rjson[L"ResourceData"].arrayView()) rData.emplace_back(r);
	for (auto i : ijson[L"ItemData"].arrayView()) iData.emplace_back(i);
	for (auto c : cjson[L"CitizenData"].arrayView()) cData.emplace_back(c);
	for (auto i : bjson[L"BiomeData"].arrayView()) bData.emplace_back(i);
	return true;
}
VData::VData(const JSONValue _json)
	: id(int(vData.size()))
	, name(_json[L"Name"].getOr<String>(L"hoge"))
	, speed(_json[L"Speed"].getOr<double>(1.00))
	, range(_json[L"Range"].getOr<double>(1.00))
	, volume(_json[L"Volume"].getOr<int>(0))
	, isShip(_json[L"IsShip"].getOr<bool>(false))
{}

RData::RData(const JSONValue _json)
	: id(int(rData.size()))
	, name(_json[L"Name"].getOr<String>(L"hoge"))
{}

BData::BData(const JSONValue _json)
	: id(int(bData.size()))
	, name(_json[L"Name"].getOr<String>(L"hoge"))
	, color(_json[L"Color"].getOr<String>(L"#000000"))
	, movingCost(_json[L"MovingCost"].getOr<double>(1.00))
{}

IData::IData(const JSONValue _json)
	: id(int(iData.size()))
	, name(_json[L"Name"].getOr<String>(L"hoge"))
	, volume(_json[L"Volume"].getOr<int>(10))
	, value(_json[L"Value"].getOr<int>(0))
	, color(_json[L"Color"].getOr<String>(L"#000000"))
{}

Product::Product(const JSONValue _json)
	: numProduct(_json[L"NumProduct"].getOr<int>(0))
	, itemID(-1)
{
	for (auto& i : iData)
		if (i.name == _json[L"ItemName"].getOr<String>(L"")) itemID = i.id;
}

CData::CData(const JSONValue _json)
	: name(_json[L"Name"].getOr<String>(L"hoge"))
	, cost(_json[L"Cost"].getOr<int>(0))
	, wage(_json[L"Wage"].getOr<int>(0))
	, product(_json[L"Product"])
	, needResourceID(-1)
{
	//NeedResourceデータの読み込み
	for (auto& i : rData)
		if (i.name == _json[L"NeedResource"].getOr<String>(L"")) needResourceID = i.id;
}

bool	Planet::loadBiome()
{
	auto items = FileSystem::DirectoryContents(L"Map/");
	for (auto& item : items)
	{
		if (FileSystem::IsDirectory(item))
		{
			//バイオームデータのロード
			if (FileSystem::Exists(item + L"BiomeData.bin"))
			{
				BinaryReader reader(item + L"BiomeData.bin");
				for (auto& n : nodes)
				{
					reader.read(n.biomeType);
					if (n.biomeType >= bData.size()) return false;
					n.color = bData[n.biomeType].color.lerp(RandomColor(), 0.05);
				}
			}
			else continue;

			//Urbansデータのロード
			if (FileSystem::Exists(item + L"Urbans.json"))
			{
				JSONReader reader(item + L"Urbans.json");
				for (auto json : reader[L"Urbans"].arrayView())
					urbans.emplace_back(json);
			}
			return true;
		}
	}

	return false;
}

Urban::Urban(const JSONValue _json)
	: id(int(urbans.size()))
	, joinedNodeID(_json[L"JoinedNodeID"].getOr<int>(-1))
	, name(_json[L"Name"].getOr<String>(L"hoge"))
	, timer(0.5 + nodes[joinedNodeID].pos.mPos.x / TwoPi)
	, day(0)
	, numCitizens(_json[L"NumCitizens"].getOr<int>(1))
{
	nodes[joinedNodeID].ownUrbanID = id;
	resource.resize(rData.size());
	jobEfficiency.resize(cData.size());
	avgIncome.resize(cData.size());

	for (auto i : step(int(rData.size())))
	{
		if (!_json[L"Resources." + rData[i].name].isEmpty())
		{
			resource[i] = _json[L"Resources." + rData[i].name].getOr<int>(10);
		}
	}

	for (int i = 0; i < int(iData.size()); ++i) baskets.emplace_back(i, id);
	for (int i = 0; i < int(cData.size()); ++i)
		for (int j = 0; j < 3; ++j) citizens.emplace_back(int(citizens.size()), i, id);
	for (int i = 0; i < Max(0, int(numCitizens - cData.size() * 3)); ++i)
		citizens.emplace_back(int(citizens.size()), 0, id);
}