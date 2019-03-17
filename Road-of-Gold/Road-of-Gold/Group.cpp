#include"Group.h"
#include"Vehicle.h"
#include"Wallet.h"

Group::Group()
	: walletID(getNewWalletID())
{}
Group::Group(const JSONValue& _json)
	: name(_json[U"Name"].getString())
{}