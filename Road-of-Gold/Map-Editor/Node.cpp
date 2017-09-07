#include"Node.h"
#include"Planet.h"
#include"BiomeData.h"
#include"Urban.h"

Array<Node> nodes;
Array<Path*> paths;

Node::Node(const Pos& _pos)
	: pos(_pos)
	, biomeType(0)
{}
Color	Node::getColor() const { return biomeData[biomeType].color; }
Path::Path(int _parentNodeID, int _childNodeID)
	: parentNodeID(_parentNodeID)
	, childNodeID(_childNodeID)
{}
Node&	Path::getChild() const { return nodes[childNodeID]; }
Node&	Path::getParent() const { return nodes[parentNodeID]; }
Line	Path::getLine() const
{
	auto p1 = getParent().pos.mPos;
	auto p2 = getChild().pos.mPos;
	if (abs(p1.x - p2.x) > 180_deg)
	{
		if (p1.x > 0) p1.x -= 360_deg;
		else p2.x -= 360_deg;
	}
	return Line(p1, p2);
}
int		Node::ownUrbanID() const
{
	for (auto& u : urbans)
	{
		if (u.joinedNodeID == id()) return u.id();
	}

	return -1;
}
int	Node::id() const
{
	return int(this - &nodes.front());
}