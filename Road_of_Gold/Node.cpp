#include"Node.h"
#include"Planet.h"
#include"GlobalVariables.h"


Array<Node> nodes;
Node::Node(const Pos& _pos)
	: id(int(nodes.size()))
	, joinedRegionID(-1)
	, ownUrbanID(-1)
	, biomeType(0)
	, pos(_pos)
	, isScaned(false)
	, isInQueue(false)
	, cost(0.0)
	, fromNodeID(-1)
{
	paths.clear();
}
bool	Node::isCoast() const { return !isSea() && paths.any([](const Path& n) {return n.getChildNode().isSea(); }); }

void	Node::draw(const Color& _color) const
{
	if (!Rect(Point(0, 0), Window::Size()).intersects(Graphics2D::GetTransform().transform(pos.mPos))) return;
	Circle(pos.mPos, 0.005).draw(_color);
	for (const auto& p : paths)
		if (p.getChildNode().joinedRegionID == joinedRegionID) p.getLine().draw(0.002, _color);
}

Region&	Node::getJoinedRegion() const { return regions[joinedRegionID]; }

bool	Planet::loadNodeMap()
{
	//NodeÇÃì«Ç›çûÇ›
	BinaryReader reader(L"Assets/NodeMap.bin");
	if (!reader) return false;	//ì«Ç›çûÇ›é∏îs

	int	nodesSize, pathsSize;
	reader.read(nodesSize);
	for (int i = 0; i < nodesSize; ++i)
	{
		Vec3 ePos;
		reader.read(ePos);
		nodes.emplace_back(ePos);
	}
	reader.read(pathsSize);
	for (int i = 0; i < pathsSize; ++i)
	{
		int parentNodeID;
		int childNodeID;
		reader.read(parentNodeID);
		reader.read(childNodeID);
		nodes[parentNodeID].paths.push_back(Path(parentNodeID, childNodeID));
	}
	for (auto& n : nodes)
	{
		for (auto& p : n.paths)
		{
			p.id = int(paths.size());
			p.length = nodes[p.childNodeID].pos.ePos.distanceFrom(n.pos.ePos);
			paths.push_back(&p);
		}
	}
	return true;
}

void	Planet::setRegions()
{
	//ReigonÇÃê›íË
	for (auto& n : nodes)
	{
		if (!n.isSea() && n.joinedRegionID == -1)
		{
			regions.emplace_back();
			Array<Node*> nodeTemp;
			nodeTemp.push_back(&n);
			for (int w = 0; w < nodeTemp.size(); w++)
			{
				for (auto& p : nodeTemp[w]->paths)
				{
					auto& m = p.getChildNode();
					if (!m.isScaned && !m.isSea())
					{
						regions.back().numNodes++;
						m.joinedRegionID = regions.back().id;
						m.isScaned = true;
						nodeTemp.push_back(&m);
					}
				}
			}
			for (auto& t : nodeTemp) t->isScaned = false;
		}
	}
}

Array<Path*> paths;
Path::Path(int _parentNodeID, int _childNodeID)
	: id(0)
	, cost(0)
	, length(0)
	, parentNodeID(_parentNodeID)
	, childNodeID(_childNodeID)
{}
Node&	Path::getChildNode() const { return nodes[childNodeID]; }
Node&	Path::getParentNode() const { return nodes[parentNodeID]; }

Line	Path::getLine() const
{
	auto p1 = getParentNode().pos.mPos;
	auto p2 = getChildNode().pos.mPos;
	if (abs(p1.x - p2.x) > Pi)
	{
		if (p1.x > 0) p1.x -= TwoPi;
		else p2.x -= TwoPi;
	}
	return Line(p1, p2);
}

Array<Region> regions;
Region::Region()
	: id(int(regions.size()))
	, numNodes(0)
	, hasCity(false)
	, color(RandomHSV())
{}