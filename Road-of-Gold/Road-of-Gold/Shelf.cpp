#include"Shelf.h"
#include"Urban.h"
#include"ItemData.h"
#include"Wallet.h"

Basket::Basket(const Casket& _casket, int _price, int _ownerWalletID)
	: casket(_casket)
	, price(_price)
	, ownerWalletID(_ownerWalletID)
{}
Basket::Basket(int _itemType, int _numItem, int _price, int _ownerWalletID)
	: casket(_itemType, _numItem)
	, price(_price)
	, ownerWalletID(_ownerWalletID)
{}
Wallet& Basket::wallet() const { return wallets[ownerWalletID]; }
Owner	Basket::owner() const { return wallets[ownerWalletID].owner; }
Shelf::Shelf()
	: numItem(0)
	, joinedUrban(nullptr)
{}
int		Shelf::itemType() const { return int(this - &joinedUrban->shelves.front()); }
ItemData&	Shelf::data() const { return itemData[this - &joinedUrban->shelves.front()]; }