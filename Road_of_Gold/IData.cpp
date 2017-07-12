#include "CData.h"
Array<IData> iData;
Array<CData> cData;

void loadEconomicData(const FilePath& _filePath)
{
	JSONReader json(_filePath);
	for (auto i : json[L"IData"].arrayView())
		iData.emplace_back(i);
	for (auto c : json[L"CData"].arrayView())
		cData.emplace_back(c);
}

IData::IData(const JSONValue _json)
	: id(int(iData.size()))
	, name(_json[L"ItemName"].getOr<String>(L"hoge"))
	, description(_json[L"ItemDescription"].getOr<String>(L"hoge"))
	, volume(_json[L"Volume"].getOr<double>(10))
	, value(_json[L"Value"].getOr<double>(0))
{}