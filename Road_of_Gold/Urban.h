#pragma once

struct Group;
struct Node;
struct Pos;
struct Route;
struct Citizen;

struct Ring
{
	Ring(int _price, int _num, const Group* _owner);
	Ring(int _price, int _num, const Citizen* _owner);

	int price;
	int num;
	int ownerGroupID;
	int ownerCitizenID;
};
bool operator<(const Ring& _left, const Ring& _right);
bool operator>(const Ring& _left, const Ring& _right);

struct Basket
{
	Basket(int _itemType, int _joinedUrbanID);
	String&	getItemName() const;
	void	addRing(int _price, int _num, const Group* _owner);
	void	addRing(int _price, int _num, const Citizen* _owner);
	int		getCost(int _num) const;
	int		getNumItem() const;
	void	buyItem(int _num);

	int joinedUrbanID;
	int itemType;
	Array<int>	tradeLog;
	Array<int>	chart;
	Array<Ring> rings;
};

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
	int		tmr;	//転職判定までのカウント
};

struct Urban
{
	Urban(int _joinedNodeID);
	void	update();
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
	Array<int>		avgBhs;	//各職業のBHS
};
extern Array<String>	UrbanNames;
extern Urban*			selectedUrban;
extern Array<Urban>		urbans;
bool	setUrban(Node& _node);