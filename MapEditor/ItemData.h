#pragma once

struct ItemData
{
	String	name;
	int		value;
	Color	color;
	Texture	icon;

	ItemData(const JSONValue& _json)
		: name(_json[L"Name"].getOr<String>(L""))
		, value(_json[L"Value"].getOr<int>(0))
		, color(_json[L"Color"].getOr<String>(L"#FFFFFF"))
		, icon(_json[L"Icon"].getOr<String>(L""))
	{}
	int		id() const;
};

extern Array<ItemData>	itemData;

void	loadItemData();