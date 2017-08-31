#pragma once
#include "Pos.h"

struct Node;
struct BiomeData;

struct Node
{
	Pos		pos;
	int		biomeType;
	Color	color;

	Node(const Pos& _pos);
	int		ownUrbanID() const;
	int		id() const;
	BiomeData&	data() const;
};

extern Array<Node> nodes;

bool saveMapData(const FilePath& _path);
bool loadMapData(const FilePath& _path);