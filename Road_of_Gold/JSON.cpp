#include"JSON.h"

Array<BData> bData;
Array<IData> iData;
Array<CData> cData;

bool loadJSONData()
{
	{
		JSONReader json(L"Assets/EconomicData.json");
		if (json.isEmpty()) return false;
		for (auto i : json[L"IData"].arrayView()) iData.emplace_back(i);
		for (auto c : json[L"CData"].arrayView()) cData.emplace_back(c);
	}
	{
		JSONReader json(L"Assets/BiomeData.json");
		if (json.isEmpty()) return false;
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

BData::BData(const JSONValue _json)
	: name(_json[L"Name"].getOr<String>(L"hoge"))
	, color(_json[L"Color"].getOr<String>(L"#000000"))
{}