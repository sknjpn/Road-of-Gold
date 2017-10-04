#pragma once

struct Path
{
	int		toID;
	double	length;
	double	riverWidth;

	Path(int _toID)
		: toID(_toID)
		, length(0)
		, riverWidth(0)
	{}
};