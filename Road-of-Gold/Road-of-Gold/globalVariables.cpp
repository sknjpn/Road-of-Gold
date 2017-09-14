#include"Urban.h"
#include"Group.h"
#include"Vehicle.h"
#include"Node.h"
#include"Planet.h"
#include"Route.h"
#include"Wallet.h"
#include"UI.h"
#include"Export.h"
#include"Scuttle.h"
#include"Incident.h"
#include"Nation.h"

#include"EnergyData.h"
#include"CitizenData.h"
#include"BiomeData.h"
#include"VehicleData.h"
#include"ItemData.h"

UI			ui;
Planet		planet;
TinyCamera	tinyCamera;
JSONReader	incidentsJson;

Array<Node>		nodes;
Array<Route>	routes;
Array<Urban>	urbans;
Array<Group>	groups;
Array<Wallet>	wallets;
Array<Vehicle>	vehicles;
Array<Export>	exports;
Array<Scuttle>	scuttles;
Array<Incident>	incidents;
Array<Nation>	nations;

Array<EnergyData>	energyData;
Array<CitizenData>	citizenData;
Array<BiomeData>	biomeData;
Array<VehicleData>	vehicleData;
Array<ItemData>		itemData;