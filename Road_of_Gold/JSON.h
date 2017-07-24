#pragma once

bool loadJSONData();

struct RData
{
	String name;
	String description;

	RData(const JSONValue _json);
};
extern Array<RData> rData;

struct BData
{
	String	name;
	Color	color;
	double	movingCost;

	BData(const JSONValue _json);
};
extern Array<BData> bData;

struct IData
{
	int		id;
	String	name;
	String	description;
	int		volume;
	int		value;

	IData(const JSONValue _json);
};
extern Array<IData> iData;

struct Consume
{
	int	itemID;
	int numConsume;

	Consume(const JSONValue _json)
		: numConsume(_json[L"NumConsume"].getOr<int>(0))
		, itemID(-1)
	{
		for (auto& i : iData)
			if (i.name == _json[L"ItemName"].getOr<String>(L"")) itemID = i.id;
	}
};

struct Product
{
	int	itemID;
	int numProduct;

	Product(const JSONValue _json)
		: numProduct(_json[L"NumProduct"].getOr<int>(0))
		, itemID(-1)
	{
		for (auto& i : iData)
			if (i.name == _json[L"ItemName"].getOr<String>(L"")) itemID = i.id;
	}
};

struct Job
{
	String	name;
	String	description;
	int		cost;
	int		wage;
	Array<Consume> consume;
	Array<Product> product;

	Job(const JSONValue _json)
		: name(_json[L"JobName"].getOr<String>(L"hoge"))
		, description(_json[L"JobDescription"].getOr<String>(L"hoge"))
		, wage(_json[L"Wage"].getOr<int>(0))
		, cost(_json[L"Cost"].getOr<int>(0))
	{
		for (auto c : _json[L"Consume"].arrayView())
			consume.emplace_back(c);
		for (auto p : _json[L"Product"].arrayView())
			product.emplace_back(p);
	}
};

struct CData
{
	String name;
	Job job;

	CData(const JSONValue _json)
		: name(_json[L"CitizenName"].getOr<String>(L"hoge"))
		, job(_json[L"Job"])
	{}
};
extern Array<CData> cData;

extern Array<String> UrbanName;