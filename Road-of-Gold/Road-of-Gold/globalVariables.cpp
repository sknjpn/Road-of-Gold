#include"Urban.h"
#include"Group.h"
#include"Vehicle.h"
#include"Node.h"
#include"Planet.h"
#include"Route.h"
#include"Wallet.h"
#include"Display.h"
#include"Export.h"
#include"Scuttle.h"
#include"River.h"
#include"Fleet.h"

#include"EnergyData.h"
#include"CitizenData.h"
#include"BiomeData.h"
#include"VehicleData.h"
#include"ItemData.h"
RouteMaker		routeMaker;
Selecter		selecter;
DisplayUrban	displayUrban;
DisplayFleets	displayFleets;
Planet			planet;
TinyCamera		tinyCamera;
JSONReader		incidentsJson;
Array<Font*>	globalFonts;
Array<Node>		nodes;
Array<Route>	routes;
Array<Urban>	urbans;
Array<Group>	groups;
Array<Wallet>	wallets;
Array<Vehicle>	vehicles;
Array<Export>	exports;
Array<Scuttle>	scuttles;
Array<Fleet>	fleets;
Array<River>	rivers;
Array<EnergyData>	energyData;
Array<CitizenData>	citizenData;
Array<BiomeData>	biomeData;
Array<VehicleData>	vehicleData;
Array<ItemData>		itemData;