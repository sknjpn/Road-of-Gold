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
	: itemType(-1)
	, numItem(_json[L"NumItem"].getOr<int>(0))
{
	if (numItem > 0)
	{
		for (auto i : step(int(itemData.size())))
		{
			if (itemData[i].name == _json[L"ItemName"].getOr<String>(L""))
			{
				itemType = i;
				return;
			}
		}

		LOG_ERROR(L"ë∂ç›ÇµÇ»Ç¢ItemÇéQè∆Ç≥ÇÍÇ‹ÇµÇΩ");
		System::Exit();
	}
}
ItemData&	Casket::data() const
{
	return itemData[itemType];
}
bool	Casket::isEmpty() const
{
	return numItem == 0;
}