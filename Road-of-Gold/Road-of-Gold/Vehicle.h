#pragma once

#include"Casket.h"
#include"Export.h"
#include"Chain.h"

struct Wallet;
struct VehicleData;
struct Fleet;
struct Pos;
struct Urban;
struct Route;

struct Vehicle
{
	int		vehicleType;
	int		walletID;
	Casket	cargo;
	double	routeProgress;
	double	sleepTimer;
	Urban*	nowUrban;
	Route*	route;
	Export	exportLog;
	bool	stopFlag;	//true‚È‚ç‚ÎŽ–‹Æ‚ð’†’f
	bool	planFixed;
	double	timer;	//‰^—pŠúŠÔ
	double	period;	//ˆêŽü‚·‚é‚Ì‚É—v‚·‚éŽžŠÔ
	Chain	chain;

	Vehicle(int _vehicleType, Urban* _nowUrban);
	VehicleData&	data() const;
	Vec2	pos() const;
	double	angle() const;
	Wallet&	wallet() const;
	bool	mouseOver() const;
	int		id() const;
};

extern Array<Vehicle> vehicles;

void	updateVehicles();
void	drawVehicles();