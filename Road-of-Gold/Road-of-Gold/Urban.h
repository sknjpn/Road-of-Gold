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
struct Nation;

struct Urban
{
	String	name;
	int		walletID;
	int		joinedNodeID;
	int		averageIncome;
	double	productivity;
	Nation*	joinedNation;
	Sandglass	sandglass;
	Array<Energy>	energies;
	Array<Citizen>	citizens;
	Array<double>	jobEfficiency;
	Array<Route*>	ownRoutes;
	Array<Seller>	sellers;
	Array<Buyer>	buyers;
	Array<Shelf>	shelves;
	Array<Customer>	customers;

	Urban(const JSONValue& _json);
	void	addEnergy(const String _name, int _num);
	void	pullEnergy(const String _name, int _num);
	void	sellItem(const Casket& _casket, int _price, int _ownerWalletID);
	void	sellItem(int _itemType, int _numItem, int _price, int _ownerWalletID);
	int		numItem(int _itemType) const;	//îÑÇÁÇÍÇƒÇ¢ÇÈêî
	bool	isSoldOut(int _itemType) const;	//îÑÇËêÿÇÍÇ©Ç«Ç§Ç©
	int		cost(int _itemType, int _numItem = 1) const;	//_numItemå¬îÉÇ§ÇÃÇ…Ç©Ç©ÇÈç≈í·ÉRÉXÉg	Ç»Ç¢Ç»ÇÁ-1Çï‘Ç∑
	void	buyItem(int _itemType, int _walletID, int _numItem = 1);
	int		id() const;
	Pos		pos() const;
	Circle	shape() const;
	bool	mouseOver() const;
	bool	leftClicked() const;
	bool	hasRoute() const;
	bool	hasSeaRoute() const;
	bool	hasLandRoute() const;
};

extern Array<Urban> urbans;

void	updateUrbans();
void	drawUrbanIcon();
void	drawUrbanName();
Urban*	getUrban(const String& _name);