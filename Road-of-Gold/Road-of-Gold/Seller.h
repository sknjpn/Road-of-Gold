#pragma once

#include"Casket.h"

struct Wallet;

struct Seller
{
	int		walletID;	//振り込み先
	Casket	casket;
	int		target;		//販売数
	int		progress;	//進行度
	int		period;		//運用期間

	Seller(int _walletID, Casket _casket, int _period);
	Wallet&	wallet() const;
};