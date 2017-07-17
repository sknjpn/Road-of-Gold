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
	double	len;
	int		parentNodeID;
	int		childNodeID;

};
extern Array<Path*> paths;

struct Node
{
	Node::Node(int _id, const Pos& _pos);
	bool	isCoast() const;
	void	draw(const Color& _color) const;
	Region&	getJoinedRegion() const;

	int		id;
	int		joinedRegionID;
	int		ownUrbanID;
	bool	isSea;
	Pos		pos;
	bool	isScaned, isInQueue;
	double	cost;
	int		fromNodeID;
	Color	clr= RandomColor();
	Array<Path> paths;
};
extern Array<Node> nodes;
bool	loadNodeMap();
void	setPlanetToNodes();

struct Region
{
	Region(int _id);

	Color	color;
	int		id;
	int		numNodes;
	bool	hasCity;
};
extern Array<Region> regions;
