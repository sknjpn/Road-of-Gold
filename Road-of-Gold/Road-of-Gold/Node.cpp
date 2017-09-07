#include"Node.h"
#include"BiomeData.h"
#include"Urban.h"

BiomeData& Node::data() const { return biomeData.at(biomeType); }
bool	Node::isSea() const { return data().isSea; }
int		Node::id() const { return int(this - &nodes.front()); }

bool	Node::hasUrban() const
{
	return urbans.any([this](const Urban& u) { return this->id() == u.joinedNodeID; });
}
