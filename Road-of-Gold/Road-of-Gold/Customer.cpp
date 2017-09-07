#include"Wallet.h"
#include"Customer.h"

Customer::Customer(double _rate)
	: walletID(getNewWalletID())
	, sandgrass(Random(1.0))
	, rate(_rate)
{}
Wallet&	Customer::wallet() const { return wallets[walletID]; }