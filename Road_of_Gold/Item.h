#pragma once

struct IData
{
	String	name;
};
extern Array<IData> iData;

struct Item
{
	Item();

	int		itemType;
	int		num;
};