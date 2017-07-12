#pragma once

void loadEconomicData(const FilePath& _filePath = L"Assets/EconomicData.json");

struct IData
{
	IData(const JSONValue _json);

	int		id;
	String	name;
	String	description;
	double	volume;
	double	value;
};
extern Array<IData> iData;

struct Consume
{
	Consume(const JSONValue _json)
		: numConsume(_json[L"NumConsume"].getOr<int>(0))
		, itemID(-1)
	{
		for (auto& i : iData)
			if (i.name == _json[L"ItemName"].getOr<String>(L"")) itemID = i.id;
		Log(itemID);
	}
	int	itemID;
	int numConsume;
};

struct Product
{
	Product(const JSONValue _json)
		: numProduct(_json[L"NumProduct"].getOr<int>(0))
		, itemID(-1)
	{
		for (auto& i : iData)
			if (i.name == _json[L"ItemName"].getOr<String>(L"")) itemID = i.id;
		Log(itemID);
	}
	int	itemID;
	int numProduct;
};

struct Job
{
	Job(const JSONValue _json)
		: name(_json[L"JobName"].getOr<String>(L"hoge"))
		, description(_json[L"JobDescription"].getOr<String>(L"hoge"))
		, wage(_json[L"Wage"].getOr<int>(0))
	{
		for (auto c : _json[L"Consume"].arrayView())
			consume.emplace_back(c);
		for (auto p : _json[L"Product"].arrayView())
			product.emplace_back(p);
	}

	String	name;
	String	description;
	int		wage;
	Array<Consume> consume;
	Array<Product> product;
};

struct CData
{
	CData(const JSONValue _json)
		: name(_json[L"CitizenName"].getOr<String>(L"hoge"))
		, job(_json[L"Job"])
	{}

	String name;
	Job job;
};
extern Array<CData> cData;