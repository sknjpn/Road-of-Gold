#pragma once

#include"Citizen.h"
#include"Shelf.h"
#include"Wallet.h"
#include"Energy.h"
#include"Sandglass.h"
#include"Trader.h"
#include"Customer.h"

struct Node;
struct Wallet;
struct Pos;
struct Route;
struct Group;
struct VehicleData;

struct Ticket
{
	Group*	client;
	int		vehicleType;
	bool	isInProcess;

	Ticket(Group* _client, int _vehicleType)
		: client(_client)
		, vehicleType(_vehicleType)
		, isInProcess(false)
	{}
	VehicleData&	data() const;
};

//造船所
struct Dock
{
	Ticket*	inProcessTicket;
	double	progress;
	int		tier;

	Dock()
		: inProcessTicket(nullptr)
		, progress(0.0)
		, tier(0)
	{}
	Dock(int _tier)
		: inProcessTicket(nullptr)
		, progress(0.0)
		, tier(_tier)
	{}
};

struct Urban
{
	String	name;
	int		walletID;
	int		joinedNodeID;
	int		averageIncome;
	bool	isSelected;
	double	productivity;
	Sandglass	sandglass;
	Array<Energy>	energies;
	Array<Citizen>	citizens;
	Array<double>	jobEfficiency;
	Array<Route*>	ownRoutes;
	Array<Seller>	sellers;
	Array<Buyer>	buyers;
	Array<Shelf>	shelves;
	Array<Customer>	customers;
	Array<Dock>		docks;
	Array<Ticket>	tickets;

	Urban(const JSONValue& _json);
	void	addEnergy(const String _name, int _num);
	void	pullEnergy(const String _name, int _num);
	void	sellItem(const Casket& _casket, int _price, int _ownerWalletID);
	void	sellItem(int _itemType, int _numItem, int _price, int _ownerWalletID);
	int		numItem(int _itemType) const;	//売られている数
	bool	isSoldOut(int _itemType) const;	//売り切れかどうか
	int		cost(int _itemType, int _numItem = 1) const;	//_numItem個買うのにかかる最低コスト	ないなら-1を返す
	void	buyItem(int _itemType, int _walletID, int _numItem = 1);
	int		id() const;
	Pos		pos() const;
	Circle	shape() const;
	bool	mouseOver() const;
	bool	leftClicked() const;
	bool	rightClicked() const;
	bool	hasRoute() const;
	bool	hasSeaRoute() const;
	bool	hasLandRoute() const;
	int		numCitizens(int _citizenType) const;
};

extern Array<Urban> urbans;

void	updateUrbans();
void	drawUrbanIcon();
void	drawUrbanName();
Urban*	getUrban(const String& _name);