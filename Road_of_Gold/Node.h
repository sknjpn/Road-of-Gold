#pragma once
#include "Pos.h"


struct Node;

struct Path
{
	int		id;
	double	length;
	double	cost;
	int		parentNodeID;
	int		childNodeID;

	Path(int _parentNodeID, int _childNodeID);
	Node&	getChildNode() const;
	Node&	getParentNode() const;
	Line	getLine() const;

};
extern Array<Path*> paths;

struct Node
{
	int		biomeType;
	int		id;
	int		joinedRegionID;
	int		ownUrbanID;
	Pos		pos;
	bool	isScaned, isInQueue;
	double	cost;
	int		fromNodeID;
	Color	color;
	Array<Path> paths;

	Node(const Pos& _pos);
	bool	isCoast() const;
	void	draw(const Color& _color) const;
	bool	isSea() const { return biomeType == 0 || biomeType == 1; }
};
extern Array<Node> nodes;
