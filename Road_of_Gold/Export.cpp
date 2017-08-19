#include"Export.h"


Export::Export()
	: from(nullptr)
	, to(nullptr)
	, itemType(-1)
	, numItemPerDay(0)
{}
Export::Export(Urban* _from, Urban* _to, int _itemType, double _numItemPerDay)
	: from(_from)
	, to(_to)
	, itemType(_itemType)
	, numItemPerDay(_numItemPerDay)
{}
int		Export::id() const
{
	return int(this - &exports.front());
}