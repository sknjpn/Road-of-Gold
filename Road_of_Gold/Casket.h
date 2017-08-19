#pragma once

struct ItemData;

struct Casket
{
	int	itemType;
	int	numItem;

	Casket();
	Casket(int _itemType, int _numItem);
	Casket(const JSONValue& _json);
	ItemData&	data() const;
	bool	isEmpty() const;
};