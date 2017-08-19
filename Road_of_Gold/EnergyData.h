#pragma once

struct EnergyData
{
	String	name;

	EnergyData(const JSONValue& _json)
		: name(_json[L"Name"].getOr<String>(L""))
	{}
	int		id() const;
};

extern Array<EnergyData>	energyData;

void	loadEnergyData();