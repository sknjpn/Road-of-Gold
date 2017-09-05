#pragma once

#include"Casket.h"

struct CitizenData
{
	String	name;
	int		wage;
	int		needEnergyType;
	Casket	product;

	CitizenData(const JSONValue& _json);
	int		id() const;
};

extern Array<CitizenData>	citizenData;

CitizenData*	getCitizenData(const String& _name);
int	getCitizenType(const String& _name);