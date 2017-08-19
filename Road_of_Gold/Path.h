#pragma once

struct Node;

struct Path
{
	int		toID;
	double	length;

	Path(int _toID)
		: toID(_toID)
		, length(0)
	{}
};