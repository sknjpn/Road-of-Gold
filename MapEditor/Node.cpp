#include"Node.h"
#include"Planet.h"
#include"Pi.h"
#include"JSON.h"

Array<Node> nodes;
Array<Path*> paths;

Node::Node(const Pos& _pos)
	: id(int(nodes.size()))
	, pos(_pos)
	, biomeType(0)
	, ownUrbanID(-1)
{}
Color	Node::getColor() const { return bData[biomeType].color; }
Path::Path(int _parentNodeID, int _childNodeID)
	: id(0)
	, parentNodeID(_parentNodeID)
	, childNodeID(_childNodeID)
{}
Node&	Path::getChild() const { return nodes[childNodeID]; }
Node&	Path::getParent() const { return nodes[parentNodeID]; }
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
void	saveBiomeData(const FilePath& _filePath)
{
	BinaryWriter writer(_filePath);
	for (auto& n : nodes) writer.write(n.biomeType);
	writer.close();
}