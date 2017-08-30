#pragma once

#include"Casket.h"
#include"Export.h"

struct Wallet;
struct VehicleData;
struct Fleet;
struct Pos;
struct Urban;
struct Route;

enum struct Code
{
	Move,	//ˆø”‚ÌID‚Ì“ss‚ÉˆÚ“®
	Jump,	//ˆø”‚Ì”Ô’n‚ÉƒWƒƒƒ“ƒv
	Wait,	//ˆê“ú‹x~
	Buy,
	Sell,
	MVol,	//Å‘å—e—Ê
};

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
	bool	stopFlag;	//true‚È‚ç‚Î–‹Æ‚ğ’†’f

	int		maxVolume;
	int		reader;
	double	timer;	//‰^—pŠúŠÔ
	double	period;	//ˆêü‚·‚é‚Ì‚É—v‚·‚éŠÔ
	Array<std::pair<Code, int>> chain;

	Vehicle(int _vehicleType, Urban* _nowUrban);
	VehicleData&	data() const;
	Vec2	pos() const;
	Wallet&	wallet() const;
	bool	mouseOver() const;
};

extern Array<Vehicle> vehicles;

void	updateVehicles();
void	drawVehicles();