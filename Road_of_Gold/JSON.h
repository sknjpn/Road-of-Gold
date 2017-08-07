#pragma once

bool loadJSONData();

struct VData
{
	VData(const JSONValue _json);
	int		id;
	String	name;
	double	speed;
	double	range;
	int		volume;
	bool	isShip;
};
extern Array<VData> vData;

struct RData
{
	int		id;
	String	name;

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
	int		volume;
	int		value;
	Color	color;

	IData(const JSONValue _json);
};
extern Array<IData> iData;


struct Product
{
	int	itemID;
	int numProduct;

	Product(const JSONValue _json);
};

struct CData
{
	String	name;
	int		cost;
	int		wage;
	int		needResourceID;
	Product product;

	CData(const JSONValue _json);
};
extern Array<CData> cData;
extern Array<String> GroupName;