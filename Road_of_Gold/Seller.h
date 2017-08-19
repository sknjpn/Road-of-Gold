#pragma once

#include"Casket.h"
#include"Wallet.h"

struct Wallet;

struct Seller
{
	int		walletID;	//U‚è‚İæ
	Casket	casket;
	int		timer;		//”„‚éŠúŠÔ

	Seller(int _walletID, Casket _casket, int _timer)
		: walletID(_walletID)
		, casket(_casket)
		, timer(_timer + 1)
	{}
	Wallet&	wallet() const { return wallets[walletID]; }
};