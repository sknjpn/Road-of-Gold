#pragma once

struct Urban;

struct Export
{
	Urban*	from;
	Urban*	to;
	int		itemType;
	double	numItemPerDay;

	Export();
	Export(Urban* _from, Urban* _to, int _itemType, double _numItemPerDay);
	int		id() const;
};

extern Array<Export>	exports;