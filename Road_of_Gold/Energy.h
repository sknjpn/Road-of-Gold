#pragma once

struct EnergyData;

struct Energy
{
	int		energyType;
	int		numEnergy;

	Energy(int _energyType, int _numEnergy)
		: energyType(_energyType)
		, numEnergy(_numEnergy)
	{}
	EnergyData& data() const;
};