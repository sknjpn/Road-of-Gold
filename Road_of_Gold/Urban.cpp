#include"Urban.h"
#include"Node.h"
#include"Pos.h"
#include"Item.h"
#include"Group.h"
#include"CData.h"
#include"Pi.h"
#include"Route.h"

Ring::Ring(const int& _price, const int& _num, const Group* _owner)
	: price(_price), num(_num), ownerCompanyID(_owner == NULL ? -1 : _owner->id)
{}
bool	operator<(const Ring& _left, const Ring& _right)
{
	return _left.price < _right.price;
}

bool	operator>(const Ring& _left, const Ring& _right)
{
	return _left.price > _right.price;
}

Basket::Basket(const int& _itemType) : itemType(_itemType), minimumPrice(10000)
{
	chart.resize(1024);
}
String&	Basket::getItemName() const { return iData[itemType].name; }
void	Basket::addRing(const int& _price, const int& _num, const Group* _owner)
{
	rings.push_back({ _price, _num, _owner });
	rings.sort();
}
int		Basket::getCost(const int& _num) const
{
	int cost = 0;
	int num = _num;
	for (auto& r : rings)
	{
		if (r.num < num)
		{
			cost += r.num*r.price;
			num -= r.num;
		}
		else return cost + num*r.price;
	}
	return -1;
}
int		Basket::getNumItem() const
{
	int num = 0;
	for (auto& r : rings) num += r.num;
	return num;
}

Array<Urban> urbans;
Urban* selectedUrban;
Urban::Urban(const int& _joinedNodeID)
	: id(int(urbans.size()))
	, name(UrbanNames.choice())
	, joinedNodeID(_joinedNodeID)
	, timer(0.0)
{}
void	Urban::draw() const
{
	const auto drawPos = getPos().mPos;
	const Circle circle(drawPos, 0.01);
	const Color color = selectedUrban == this ? Palette::Yellow :
		(circle.mouseOver() ? Palette::Orange : Palette::Red);
	circle.draw(color).drawFrame(0.005, Palette::Black);
}
Array<Route*>	Urban::getRoutes() const
{
	Array<Route*> rArray;
	for (auto& r : routes)
		if (r.originUrbanID == id) rArray.emplace_back(&r);
	return rArray;
}
Pos&	Urban::getPos() const { return nodes[joinedNodeID].pos; }
String	Urban::getTimeAsString() const { return  Format(int(timer * 24)).lpad(2, '0') + L":" + Format(int(timer * 24 * 60) % 60).lpad(2, '0'); }
bool	setUrban(Node& _node)
{
	if (_node.isSea || _node.ownUrbanID != -1) return false;
	for (const auto& p : _node.paths) if (p.getChild().ownUrbanID != -1) return false;
	_node.ownUrbanID = int(urbans.size());
	urbans.emplace_back(_node.id);
	auto& u = urbans.back();
	u.timer = 0.5 + u.getPos().mPos.x / TwoPi;
	u.ItemStock.resize(iData.size());
	u.ItemStock.fill(0);

	const Array<int> numCitizen = {
		Random(10,50),	//–Ø‚±‚è
		Random(10,50),	//“SzR˜J“­Ò
		Random(1,5),	//•ŠíEl
		Random(1,5),	//‹Rm
		Random(1,5),	//•óÎEl
		Random(5,10),	//‹M‘°
		Random(50,100),	//ƒpƒ“El
		Random(5,10),	//“¹‹ïEl
	};

	for (int i = 0; i < int(iData.size()); i++) u.baskets.push_back(i);
	for (int i = 0; i<int(numCitizen.size()); i++)
		for (int j = 0; j < numCitizen[i]; j++) u.citizens.push_back(Citizen(i, Random(0.25, 0.75) + u.timer));
	return true;
}