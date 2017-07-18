#pragma once
#include "Pos.h"

struct Node;
struct Region;

struct BData
{
	String	name;
	Color	color;
};
extern const Array<BData> bData;

struct Path
{
	Path::Path(int _parentNodeID, int _childNodeID);
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
	Node::Node(const Pos& _pos);
	Color	getColor() const;

	int		id;
	Pos		pos;
	int		biomeType;
	Array<Path> paths;
};
extern Array<Node> nodes;