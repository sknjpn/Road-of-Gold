#pragma once

struct Wallet;
struct CitizenData;

struct Citizen
{
	int		citizenType;
	int		walletID;
	int		targetRevenue;	//ñ⁄ïWîÑÇËè„Ç∞
	double	jobProgress;
	double	timer;
	Array<int>	incomeLog;
	double	personality;
	double	jobEfficiency;

	Citizen(int _citizenType);
	CitizenData&	data() const;
	Wallet&	wallet() const;
	int	avgIncome() const { return int(incomeLog.sum() / incomeLog.size()); }
};