#pragma once

struct Wallet
{
	int		money;

	//MarketŠÖ˜A
	int		price;	//Market‚©‚çU‚è‚İ‚É‰¿Ši‚ğ‘‚«‚İ
	int		expenses;	//xo‚ğ‹L˜^
	int		income;		//û“ü‚ğ‹L˜^

	Wallet();
	void	sendTo(Wallet* _wallet, int	_money);	//‘—‹à
	void	add(int _money);
	void	pull(int _money);
	int		id() const;
};

extern Array<Wallet>	wallets;

int	getNewWalletID();