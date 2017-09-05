#pragma once

struct EnergyData
{
	String	name;

	EnergyData(const JSONValue& _json);
	int		id() const;
};

extern Array<EnergyData>	energyData;

EnergyData*	getEnergyData(const String& _name);
int	getEnergyType(const String& _name);