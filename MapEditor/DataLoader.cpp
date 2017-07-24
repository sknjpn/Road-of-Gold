#include"JSON.h"

Array<BData> bData;
Array<IData> iData;
Array<CData> cData;
Array<RData> rData;

bool loadJSONData()
{
	JSONReader json(L"Assets/EconomicData.json");
	if (json.isEmpty()) return false;
	for (auto r : json[L"RData"].arrayView()) rData.emplace_back(r);
	for (auto i : json[L"IData"].arrayView()) iData.emplace_back(i);
	for (auto c : json[L"CData"].arrayView()) cData.emplace_back(c);
	for (auto i : json[L"BData"].arrayView()) bData.emplace_back(i);
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
{}