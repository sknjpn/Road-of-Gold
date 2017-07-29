#pragma once

struct Group;
struct Node;
struct Pos;
struct Route;
struct Citizen;

struct Ring
{
	int		price;
	int		num;
	int		ownerGroupID;
	int		ownerCitizenID;

	Ring(int _price, int _num, const Group* _owner);
	Ring(int _price, int _num, const Citizen* _owner);
};
bool operator<(const Ring& _left, const Ring& _right);
bool operator>(const Ring& _left, const Ring& _right);

struct Basket
{
	int		joinedUrbanID;
	int		itemType;
	Array<int>	tradeLog;
	Array<int>	chart;
	Array<Ring> rings;

	Basket(int _itemType, int _joinedUrbanID);
	String&	getItemName() const;
	void	addRing(int _price, int _num, const Group* _owner);
	void	addRing(int _price, int _num, const Citizen* _owner);
	int		getCost(int _num) const;
	int		getNumItem() const;
	void	buyItem(int _num);

};

struct Citizen
{
	int		id;
	int		joinedUrbanID;
	int		citizenType;
	int		money;
	double	timer;
	double	progress;
	int		price;
	int		hapiness;
	Array<int>	incomeLog;	//100日分の収入記録

	Citizen(int _id, int _citizenType, int _joinedUrbanID);
	int		avgIncome() const;
	void	addMoney(int _amount);	//外部からの収入
	void	update();
	void	goToShopping();
};

struct Urban
{
	int		id;
	String	name;
	int		joinedNodeID;
	double	timer;
	int		day;
	Array<Basket>	baskets;
	Array<Citizen>	citizens;
	Array<int>		avgIncome;	//各職業の収入平均
	Array<int>		resource;
	Array<double>	jobEfficiency;
	Array<int>		routeIDs;

	Urban();
	void	update();
	void	draw() const;
	String	getTimeAsString() const;
	Array<Route*>	getRoutesToUrban(int _urbanID) const;

};
extern Urban*			selectedUrban;
extern Array<Urban>		urbans;