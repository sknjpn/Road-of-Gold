#include"JSON.h"

Array<BData> bData;

bool loadJSONData()
{
	JSONReader json(L"Assets/EconomicData.json");
	if (json.isEmpty()) return false;
	for (auto i : json[L"BData"].arrayView()) bData.emplace_back(i);
	return true;
}

BData::BData(const JSONValue _json)
	: name(_json[L"Name"].getOr<String>(L"hoge"))
	, color(_json[L"Color"].getOr<String>(L"#000000"))
{}