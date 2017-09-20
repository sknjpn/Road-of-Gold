#pragma once
#include"Casket.h"

struct Wallet;

struct Seller
{
	int		walletID;	//U‚è‚İæ
	Casket	casket;
	int		target;		//”Ì”„”
	int		progress;	//is“x
	int		period;		//‰^—pŠúŠÔ

	Seller(int _walletID, Casket _casket, int _period);
	Wallet&	wallet() const;
};

struct Buyer
{
	Casket	casket;
	int		walletID;
	int		target;		//–Ú•Ww“ü”
	int		progress;	//is“x
	int		period;		//‰^—pŠúŠÔ
	int		price;		//w“üŒÀŠE‰¿Ši

	Buyer(int _walletID, int _itemType, int _period, int _target);
	Wallet&	wallet() const;
};