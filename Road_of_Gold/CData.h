#pragma once

/*

CitizenData

*/

struct Material
{
	int itemtype;
	int num;
};

struct Product
{
	int	itemType;
	int num;
	int costMax;
	int costMin;
	Array<Material> material;
};


struct Need
{
	int itemType;
	int priceMin;
	int priceMax;
};

struct CData
{
	String name;
	Array<Need> need;
	Product product;
};
extern Array<CData> cData;