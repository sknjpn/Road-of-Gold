#include "Node.h"
#include "Pi.h"

Array<Node> nodes;
Node::Node(const Pos& _pos)
	: id(int(nodes.size()))
	, pos(_pos)
{
	paths.clear();
}

Array<Path*> paths;
Path::Path(int _parentNodeID, int _childNodeID)
	: id(0), len(0.0), parentNodeID(_parentNodeID), childNodeID(_childNodeID) {}
Node&	Path::getChild() const
{
	return nodes[childNodeID];
}

Node&	Path::getParent() const
{
	return nodes[parentNodeID];
}

Line	Path::getLine() const
{
	auto p1 = getParent().pos.mPos;
	auto p2 = getChild().pos.mPos;
	if (abs(p1.x - p2.x) > Pi)
	{
		if (p1.x > 0) p1.x -= TwoPi;
		else p2.x -= TwoPi;
	}
	return Line(p1, p2);
}