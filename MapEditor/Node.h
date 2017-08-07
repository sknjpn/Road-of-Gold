#pragma once
#include "Pos.h"

struct Node;
struct Region;

struct Path
{
	Path(int _parentNodeID, int _childNodeID);
	Node&	getChild() const;
	Node&	getParent() const;
	Line	getLine() const;

	int		id;
	int		parentNodeID;
	int		childNodeID;

};
extern Array<Path*> paths;

struct Node
{
	Node(const Pos& _pos);
	Color	getColor() const;

	int		ownUrbanID;
	int		id;
	Pos		pos;
	int		biomeType;
	Array<Path> paths;
};
bool saveMapData(const FilePath& _path);
bool loadMapData(const FilePath& _path);
extern Array<Node> nodes;