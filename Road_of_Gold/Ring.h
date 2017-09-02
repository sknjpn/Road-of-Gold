#pragma once

#include"Casket.h"

struct Wallet;
enum struct Owner;

struct Ring
{
	Casket	casket;
	int		price;
	int		ownerWalletID;

	Ring(const Casket& _casket, int _price, int _ownerWalletID)
		: casket(_casket)
		, price(_price)
		, ownerWalletID(_ownerWalletID)
	{}
	Ring(int _itemType, int _numItem, int _price, int _ownerWalletID)
		: casket(_itemType, _numItem)
		, price(_price)
		, ownerWalletID(_ownerWalletID)
	{}
	Wallet& wallet() const;
	Owner	owner() const;
};