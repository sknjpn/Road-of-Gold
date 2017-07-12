#include"Urban.h"
#include"Node.h"
#include"Pos.h"
#include"Group.h"
#include"CData.h"
#include"Pi.h"
#include"Route.h"

Ring::Ring(const int& _price, const int& _num, const Group* _owner)
	: price(_price)
	, num(_num)
	, ownerGroupID(_owner == NULL ? -1 : _owner->id)
	, ownerCitizenID(-1)
{}

Ring::Ring(const int& _price, const int& _num, const Citizen* _owner)
	: price(_price)
	, num(_num)
	, ownerCitizenID(_owner == NULL ? -1 : _owner->id)
	, ownerGroupID(-1)
{}
bool	operator<(const Ring& _left, const Ring& _right)
{
	return _left.price < _right.price;
}

bool	operator>(const Ring& _left, const Ring& _right)
{
	return _left.price > _right.price;
}

Basket::Basket(const int& _itemType) : itemType(_itemType), mpy(100), mpt(mpy)
{
	chart.resize(1024);
	chart.fill(mpy);
}
String&	Basket::getItemName() const { return iData[itemType].name; }
void	Basket::addRing(const int& _price, const int& _num, const Group* _owner)
{
	rings.push_back({ _price, _num, _owner });
	rings.sort();
}
void	Basket::addRing(const int& _price, const int& _num, const Citizen* _owner)
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

	const Array<int> numCitizen = {
		30,	//òJì≠é“
		2,	//ñÿÇ±ÇË
		2,	//ì©å|êEêl
		2,	//éÎêl
		2,	//édóßÇƒâÆ
		5,	//ãôét
	};

	for (int i = 0; i < int(iData.size()); i++) u.baskets.push_back(i);
	for (int i = 0; i<int(numCitizen.size()); i++)
		for (int j = 0; j < numCitizen[i]; j++) u.citizens.emplace_back(int(u.citizens.size()), i, Random(0.25, 0.75) + u.timer);
	return true;
}