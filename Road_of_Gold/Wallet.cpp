#include"Wallet.h"

int		getNewWalletID()
{
	wallets.emplace_back();
	return int(wallets.size() - 1);
}
Wallet::Wallet()
	: money(0)
	, price(0)
	, expenses(0)
	, income(0)
{}
int		Wallet::id() const { return int(this - &wallets.front()); }
void	Wallet::sendTo(Wallet* _wallet, int	_money)	//‘—‹à
{
	//to
	_wallet->money += _money;
	_wallet->income += _money;

	//this
	money -= _money;
	expenses += _money;
}
void	Wallet::add(int _money)
{
	money += _money;
	income += _money;
}
void	Wallet::pull(int _money)
{
	money -= _money;
	expenses += _money;
}