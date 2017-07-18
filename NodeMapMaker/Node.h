#pragma once
#include "Pos.h"

struct Node;
struct Region;

struct Path
{
	Path::Path(int _parentNodeID, int _childNodeID);
	Node&	getChild() const;
	Node&	getParent() const;
	Line	getLine() const;

	int		id;
	double	length;
	int		parentNodeID;
	int		childNodeID;

};
extern Array<Path*> paths;

struct Node
{
	Node::Node(const Pos& _pos);

	Color	color = RandomColor();
	int		id;
	Pos		pos;
	Array<Path> paths;
};
extern Array<Node> nodes;