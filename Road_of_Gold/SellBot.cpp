#include"Group.h"
#include"Urban.h"

SellBot::SellBot(int _joinedUrbanID, int _itemType, int _numItem, Group* _owner, int _time, int _price)
	: id(int(urbans[_joinedUrbanID].sellBots.size()))
	, joinedUrbanID(_joinedUrbanID)
	, itemType(_itemType)
	, numItem(_numItem)
	, ownerGroupID(_owner->id)
	, time(1)
	, price(_price)
{
	if (time != 1) Log(time);
}
void	SellBot::update()
{
	if (time == 0) return;
	auto& u = urbans[joinedUrbanID];
	const int numSell = numItem / time;
	Log(price);
	if (numSell > 0) u.baskets[itemType].addRing(price, numSell, this);
	numItem -= numSell;
	time--;
}