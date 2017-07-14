#pragma once

struct Group;
struct Node;
struct Pos;
struct Route;
struct Citizen;

/*
商品の値札
*/
struct Ring
{
	Ring(const int& _price, const int& _num = 1, const Group* _owner = NULL);
	Ring(const int& _price, const int& _num = 1, const Citizen* _owner = NULL);

	int price;
	int num;
	int ownerGroupID;
	int ownerCitizenID;
};
bool operator<(const Ring& _left, const Ring& _right);
bool operator>(const Ring& _left, const Ring& _right);

/*
同一商品=Ringの集合体がBasket。ringsはfrontが最も安くなるようソートされる
*/
struct Basket
{
	Basket(const int& _itemType,const int& _joinedUrbanID);
	String&	getItemName() const;
	void	addRing(const int& _price, const int& _num = 1, const Group* _owner = NULL);
	void	addRing(const int& _price, const int& _num = 1, const Citizen* _owner = NULL);
	int		getCost(const int& _num) const;
	int		getNumItem() const;
	void	buyItem(const int& _num);

	int joinedUrbanID;
	int itemType;
	Array<int>	tradeLog;
	Array<int>	chart;
	Array<Ring> rings;
};

/*
住民。timerが1.0を超えると規定されたタイプの行動を行う。
*/
struct Citizen
{
	Citizen(const int& _id,const int& _citizenType, const double& _timer) 
		: citizenType(_citizenType)
		, money(1000)
		, timer(_timer)
		, id(_id)
		, price(100)
	{}

	int		id;
	int		citizenType;
	int		money;
	double	timer;
	int		price;
};

/*
都市。座標はNodeで表す。市場機能と住民をもつ。
*/
struct Urban
{
	Urban(const int& _joinedNodeID);
	void	draw() const;
	Pos&	getPos() const;
	String	getTimeAsString() const;
	Array<Route*>	getRoutes() const;

	int		id;
	String	name;
	int		joinedNodeID;
	double	timer;

	Array<Basket>	baskets;
	Array<Citizen>	citizens;
};
extern Array<String>	UrbanNames;
extern Urban*			selectedUrban;
extern Array<Urban>		urbans;
bool	setUrban(Node& _node);