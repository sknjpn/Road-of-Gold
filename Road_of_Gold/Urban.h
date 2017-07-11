#pragma once

struct Company;
struct Node;
struct Pos;

/*
商品の値札
*/
struct Ring
{
	Ring(const int& _price, const int& _num = 1, const Company* _owner = NULL);

	int price;
	int num;
	int ownerCompanyID;
};
bool operator<(const Ring& _left, const Ring& _right);
bool operator>(const Ring& _left, const Ring& _right);

/*
同一商品=Ringの集合体がBasket。ringsはfrontが最も安くなるようソートされる
*/
struct Basket
{
	Basket(const int& _itemType);
	String&	getItemName() const;
	void	addRing(const int& _price, const int& _num = 1, const Company* _owner = NULL);
	int		getCost(const int& _num) const;
	int		getNumItem() const;

	int itemType;
	int minimumPrice;
	Array<int>	chart;
	Array<Ring> rings;
};

/*
住民。timerが1.0を超えると規定されたタイプの行動を行う。
*/
struct Citizen
{
	Citizen(const int& _citizenType, const double& _timer) : citizenType(_citizenType), timer(_timer) {}
	int		citizenType;
	double	timer;
};

/*
都市。座標はNodeで表す。市場機能と住民をもつ。
*/
struct Urban
{
	Urban(const int& _joinedNodeID);
	void	draw() const;
	Pos&	getPos() const;
	String	Urban::getTimeAsString() const;

	int		id;
	String	name;
	int		joinedNodeID;
	double	timer;

	Array<int>		ItemStock;
	Array<int>		numProduct;
	Array<Basket>	baskets;
	Array<Citizen>	citizens;
};
extern Array<String>	UrbanNames;
extern Urban*			selectedUrban;
extern Array<Urban>		urbans;
bool	setUrban(Node& _node);