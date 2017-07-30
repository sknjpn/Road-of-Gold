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
	//グループ名の読み込み
	JSONReader json(L"Assets/GroupName.json");
	if (json.isEmpty()) return false;
	for (auto j : json[L"GroupName"].arrayView()) GroupName.emplace_back(j.getOr<String>(L"hoge"));

	//各種JSONデータの読み込み
	JSONReader rjson(L"Assets/RData.json");
	JSONReader ijson(L"Assets/IData.json");
	JSONReader cjson(L"Assets/CData.json");
	JSONReader bjson(L"Assets/BData.json");
	for (auto r : rjson[L"RData"].arrayView()) rData.emplace_back(r);
	for (auto i : ijson[L"IData"].arrayView()) iData.emplace_back(i);
	for (auto c : cjson[L"CData"].arrayView()) cData.emplace_back(c);
	for (auto i : bjson[L"BData"].arrayView()) bData.emplace_back(i);
	return true;
}

RData::RData(const JSONValue _json)
	: id(int(rData.size()))
	, name(_json[L"ResourceName"].getOr<String>(L"hoge"))
	, description(_json[L"ResourceDescription"].getOr<String>(L"hoge"))
{}

BData::BData(const JSONValue _json)
	: id(int(bData.size()))
	, name(_json[L"BiomeName"].getOr<String>(L"hoge"))
	, color(_json[L"BiomeColor"].getOr<String>(L"#000000"))
	, movingCost(_json[L"BiomeMovingCost"].getOr<double>(1.00))
{}

IData::IData(const JSONValue _json)
	: id(int(iData.size()))
	, name(_json[L"ItemName"].getOr<String>(L"hoge"))
	, description(_json[L"ItemDescription"].getOr<String>(L"hoge"))
	, volume(_json[L"Volume"].getOr<int>(10))
	, value(_json[L"Value"].getOr<int>(0))
	, color(_json[L"Color"].getOr<String>(L"#000000"))
{}

Consume::Consume(const JSONValue _json)
	: numConsume(_json[L"NumConsume"].getOr<int>(0))
	, itemID(-1)
{
	for (auto& i : iData)
		if (i.name == _json[L"ItemName"].getOr<String>(L"")) itemID = i.id;
}

Product::Product(const JSONValue _json)
	: numProduct(_json[L"NumProduct"].getOr<int>(0))
	, itemID(-1)
{
	for (auto& i : iData)
		if (i.name == _json[L"ItemName"].getOr<String>(L"")) itemID = i.id;
}

Job::Job(const JSONValue _json)
	: name(_json[L"JobName"].getOr<String>(L"hoge"))
	, description(_json[L"JobDescription"].getOr<String>(L"hoge"))
	, cost(_json[L"Cost"].getOr<int>(0))
	, wage(_json[L"Wage"].getOr<int>(0))
{
	//Consumeデータの読み込み
	for (auto c : _json[L"Consume"].arrayView()) consume.emplace_back(c);

	//Productデータの読み込み
	for (auto p : _json[L"Product"].arrayView()) product.emplace_back(p);

	//NeedResourceデータの読み込み
	for (auto r : _json[L"NeedResource"].arrayView())
		for (auto& i : rData)
			if (i.name == r.getOr<String>(L"")) needResourceID.emplace_back(i.id);
}

CData::CData(const JSONValue _json)
	: name(_json[L"CitizenName"].getOr<String>(L"hoge"))
	, job(_json[L"Job"])
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
