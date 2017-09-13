#include"Ring.h"
#include"Wallet.h"

Wallet& Ring::wallet() const { return wallets[ownerWalletID]; }
Owner	Ring::owner() const { return wallets[ownerWalletID].owner; }