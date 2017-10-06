#pragma once

struct BiomeData
{
	String	name;
	String	nameJP;
	Color	color;
	double	movingCost;
	bool	isSea;

	BiomeData(const JSONValue& _json);
	int		id() const;
};

extern Array<BiomeData>	biomeData;

BiomeData*	getBiomeData(const String& _name);
int	getBiomeType(const String& _name);