#include"Seller.h"
#include"Buyer.h"
#include"Vehicle.h"
#include"Wallet.h"

Seller::Seller(int _walletID, Casket _casket, int _period)
	: walletID(_walletID)
	, casket(_casket)
	, target(_casket.numItem)
	, period(_period)
	, progress(0)
{}

Buyer::Buyer(int _walletID, int _itemType, int _period, int _target)
	: walletID(_walletID)
	, casket(_itemType, 0)
	, target(_target)
	, progress(0)
	, period(_period)
{}

Wallet& Seller::wallet() const { return wallets[walletID]; }
Wallet& Buyer::wallet() const { return wallets[walletID]; }