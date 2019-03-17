#include"Casket.h"
#include"ItemData.h"

Casket::Casket()
	: itemType(-1)
	, numItem(0)
{}
Casket::Casket(int _itemType, int _numItem)
	: itemType(_itemType)
	, numItem(_numItem)
{}
Casket::Casket(const JSONValue& _json)
	: numItem(_json[U"NumItem"].getOr<int>(0))
	, itemType(getItemType(_json[U"ItemName"].getString()))
{}
ItemData&	Casket::data() const { return itemData[itemType]; }
bool	Casket::isEmpty() const { return numItem == 0; }