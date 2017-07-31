#pragma once

bool loadJSONData();

struct VData
{
	VData(const JSONValue _json);
	int		id;
	String	name;
	double	speed;
	int		volume;
	bool	isShip;
};
extern Array<VData> vData;

struct RData
{
	int		id;
	String	name;
	String	description;

	RData(const JSONValue _json);
};
extern Array<RData> rData;

struct BData
{
	int		id;
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
	Color	color;

	IData(const JSONValue _json);
};
extern Array<IData> iData;

struct Consume
{
	int	itemID;
	int numConsume;

	Consume(const JSONValue _json);
};

struct Product
{
	int	itemID;
	int numProduct;

	Product(const JSONValue _json);
};

struct Job
{
	String	name;
	String	description;
	int		cost;
	int		wage;
	Array<int>	needResourceID;
	Array<Consume> consume;
	Array<Product> product;

	Job(const JSONValue _json);
};

struct CData
{
	String name;
	Job job;

	CData(const JSONValue _json);
};
extern Array<CData> cData;
extern Array<String> GroupName;