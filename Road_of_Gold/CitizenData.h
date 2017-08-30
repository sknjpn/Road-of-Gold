#pragma once

#include"Casket.h"

struct CitizenData
{
	String	name;
	int		wage;
	int		cost;
	int		needEnergyType;
	Casket	product;

	CitizenData(const JSONValue& _json);
	int		id() const;
};

extern Array<CitizenData>	citizenData;