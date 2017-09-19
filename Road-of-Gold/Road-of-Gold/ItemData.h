#pragma once

struct ItemData
{
	String	name;
	int		value;
	int		volume;
	Color	color;
	Texture	icon;

	ItemData(const JSONValue& _json);
	int		id() const;
};

extern Array<ItemData>	itemData;

ItemData*	getItemData(const String& _name);
int	getItemType(const String& _name);