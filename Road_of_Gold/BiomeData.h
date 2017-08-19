#pragma once

struct BiomeData
{
	String	name;
	Color	color;
	double	movingCost;
	bool	isSea;

	BiomeData(const JSONValue& _json)
		: name(_json[L"Name"].getOr<String>(L""))
		, color(_json[L"Color"].getOr<String>(L"#FFFFFF"))
		, movingCost(_json[L"MovingCost"].getOr<double>(1.0))
		, isSea(_json[L"IsSea"].getOr<bool>(false))
	{}
	int		id() const;
};

extern Array<BiomeData>	biomeData;

void	loadBiomeData();