#include"JSON.h"

Array<BData> bData;
Array<RData> rData;

bool loadJSONData()
{
	//各種JSONデータの読み込み
	JSONReader rjson(L"Assets/ResourceData.json");
	JSONReader bjson(L"Assets/BiomeData.json");
	for (auto r : rjson[L"ResourceData"].arrayView()) rData.emplace_back(r);
	for (auto i : bjson[L"BiomeData"].arrayView()) bData.emplace_back(i);
	return true;
}

RData::RData(const JSONValue _json)
	: name(_json[L"Name"].getOr<String>(L"hoge"))
{}

BData::BData(const JSONValue _json)
	: name(_json[L"Name"].getOr<String>(L"hoge"))
	, color(_json[L"Color"].getOr<String>(L"#000000"))
{}
