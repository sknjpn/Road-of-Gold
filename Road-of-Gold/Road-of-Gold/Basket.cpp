#include"Basket.h"
#include"Urban.h"
#include"ItemData.h"

Basket::Basket()
	: numItem(0)
	, joinedUrban(nullptr)
{}
int		Basket::itemType() const { return int(this - &joinedUrban->baskets.front()); }
ItemData&	Basket::data() const { return itemData[this - &joinedUrban->baskets.front()]; }