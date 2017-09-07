#include"Energy.h"
#include"EnergyData.h"

EnergyData& Energy::data() const { return energyData[energyType]; }