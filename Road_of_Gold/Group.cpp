#include "Group.h"
#include "Item.h"

Item::Item()
	: itemType(-1)
	, num(0)
{}

Vehicle::Vehicle(const int& _nowNodeID)
	: nowNodeID(_nowNodeID)
	, routeID(-1)
	, routeProgress(0.0)
	, item()
{}

Array<Group> groups;
Group::Group(const int& _id)
	: id(_id)
	, name(L"hoge")
	, color(RandomHSV())
{}