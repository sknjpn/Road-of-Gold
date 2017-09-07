#pragma once
#include"Sandglass.h"

struct Customer
{
	Sandglass	sandgrass;
	int		walletID;
	double	rate;	//全カスタマー中の資金保有率

	Customer(double _rate);
	Wallet&	wallet() const;
};