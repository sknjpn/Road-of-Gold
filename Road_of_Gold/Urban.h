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
	Ring(int _price, int _num = 1, const Group* _owner = NULL);
	Ring(int _price, int _num = 1, const Citizen* _owner = NULL);

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
	Basket(int _itemType,int _joinedUrbanID);
	String&	getItemName() const;
	void	addRing(int _price, int _num = 1, const Group* _owner = NULL);
	void	addRing(int _price, int _num = 1, const Citizen* _owner = NULL);
	int		getCost(int _num) const;
	int		getNumItem() const;
	void	buyItem(int _num);

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
	Citizen(int _id, int _citizenType, int _joinedUrbanID);
	void	update();

	int		id;
	int		joinedUrbanID;
	int		citizenType;
	int		money;
	double	timer;
	int		price;
	int		hapiness;
	int		bhs;	//先月の合計幸福度
	int		ths;	//今月の合計幸福度
};

/*
都市。座標はNodeで表す。市場機能と住民をもつ。
*/
struct Urban
{
	Urban(int _joinedNodeID);
	void	draw() const;
	Pos&	getPos() const;
	String	getTimeAsString() const;
	Array<Route*>	getRoutes() const;

	int		id;
	String	name;
	int		joinedNodeID;
	double	timer;
	int		day;

	Array<Basket>	baskets;
	Array<Citizen>	citizens;
};
extern Array<String>	UrbanNames;
extern Urban*			selectedUrban;
extern Array<Urban>		urbans;
bool	setUrban(Node& _node);