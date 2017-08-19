#pragma once

#include"Pos.h"
#include"Path.h"

struct BiomeData;

struct Node
{
	int		biomeType;
	Pos		pos;
	Color	color;
	Array<Path>	paths;

	Node(const Pos& _pos)
		: pos(_pos)
		, color(RandomColor())
	{}
	BiomeData& data() const;
	bool	isSea() const;
	bool	hasUrban() const;
	int		id() const;
};

extern Array<Node> nodes;

void	initNodes();