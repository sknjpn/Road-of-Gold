#include"Urban.h"
#include"Group.h"
#include"CData.h"

Ring::Ring(int _price, int _num, const Group* _owner)
	: price(_price)
	, num(_num)
	, ownerGroupID(_owner == NULL ? -1 : _owner->id)
	, ownerCitizenID(-1)
{}

Ring::Ring(int _price, int _num, const Citizen* _owner)
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

Basket::Basket(int _itemType, int _joinedUrbanID)
	: itemType(_itemType)
	, joinedUrbanID(_joinedUrbanID)
{
	chart.resize(1024);
}
String&	Basket::getItemName() const { return iData[itemType].name; }
void	Basket::addRing(int _price, int _num, const Group* _owner)
{
	rings.push_back({ _price, _num, _owner });
	rings.sort();
}
void	Basket::addRing(int _price, int _num, const Citizen* _owner)
{
	rings.push_back({ _price, _num, _owner });
	rings.sort();
}
int		Basket::getCost(int _num) const
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
void	Basket::buyItem(int _num)
{
	int num = _num;
	auto& u = urbans[joinedUrbanID];
	for (;;)
	{
		auto& r = rings.front();
		if (r.num < num) {
			if (r.ownerCitizenID != -1)
			{
				u.citizens[r.ownerCitizenID].money += r.num*r.price;
				u.citizens[r.ownerCitizenID].price = r.price;
			}
			else groups[r.ownerGroupID].money += r.num*r.price;
			for (int i = 0; i < r.num; i++) tradeLog.emplace_back(r.price);
			num -= r.num; rings.pop_front();
		}
		else if (r.num == num) {
			if (r.ownerCitizenID != -1)
			{
				u.citizens[r.ownerCitizenID].money += r.num*r.price;
				u.citizens[r.ownerCitizenID].price = r.price;
			}
			else groups[r.ownerGroupID].money += r.num*r.price;
			for (int i = 0; i < r.num; i++) tradeLog.emplace_back(r.price);
			rings.pop_front();
			return;
		}
		else {
			if (r.ownerCitizenID != -1)
			{
				u.citizens[r.ownerCitizenID].money += num*r.price;
				u.citizens[r.ownerCitizenID].price = r.price;
			}
			else groups[r.ownerGroupID].money += num*r.price;
			for (int i = 0; i < num; i++) tradeLog.emplace_back(r.price);
			r.num -= num; return;
		}
	}
}
