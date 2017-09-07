#pragma once

#include"Casket.h"

struct Wallet;

struct Buyer
{
	Casket	casket;
	int		walletID;
	int		target;		//目標購入数
	int		progress;	//進行度
	int		period;		//運用期間

	Buyer(int _walletID, int _itemType, int _period, int _target);
	Wallet&	wallet() const;
};