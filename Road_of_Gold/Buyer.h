#pragma once

#include"Casket.h"

struct Wallet;

struct Buyer
{
	Casket	casket;
	int		walletID;

	int		target;		//–Ú•Ww“ü”
	int		progress;	//is“x
	int		period;		//‰^—pŠúŠÔ
	Optional<int>	topPrice;	//w“üÅ‚‰¿Ši

	Buyer(int _walletID, int _itemType, int _period, int _target);
	Wallet&	wallet() const;

};