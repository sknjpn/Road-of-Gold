#include "Company.h"
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

Array<Company> companies;
Company::Company(const int& _id)
	: id(_id)
	, name(L"hoge")
	, color(RandomHSV())
{}