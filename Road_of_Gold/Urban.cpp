#include"Urban.h"
#include"Node.h"
#include"Pos.h"
#include"Group.h"
#include"CData.h"
#include"Pi.h"
#include"Route.h"

Array<Urban> urbans;
Urban* selectedUrban;
Urban::Urban(int _joinedNodeID)
	: id(int(urbans.size()))
	, name(UrbanNames.choice())
	, joinedNodeID(_joinedNodeID)
	, timer(0.0)
{}
void	Urban::draw() const
{
	const auto drawPos = getPos().mPos;
	const Circle circle(drawPos, 0.01);
	const Color color = selectedUrban == this ? Palette::Yellow :
		(circle.mouseOver() ? Palette::Orange : Palette::Red);
	circle.draw(color).drawFrame(0.005, Palette::Black);
}
Array<Route*>	Urban::getRoutes() const
{
	Array<Route*> rArray;
	for (auto& r : routes)
		if (r.originUrbanID == id) rArray.emplace_back(&r);
	return rArray;
}
Pos&	Urban::getPos() const { return nodes[joinedNodeID].pos; }
String	Urban::getTimeAsString() const { return  Format(int(timer * 24)).lpad(2, '0') + L":" + Format(int(timer * 24 * 60) % 60).lpad(2, '0'); }
bool	setUrban(Node& _node)
{
	if (_node.isSea || _node.ownUrbanID != -1) return false;
	for (const auto& p : _node.paths) if (p.getChild().ownUrbanID != -1) return false;
	_node.ownUrbanID = int(urbans.size());
	urbans.emplace_back(_node.id);
	auto& u = urbans.back();
	u.timer = 0.5 + u.getPos().mPos.x / TwoPi;

	const Array<int> numCitizen = {
		100,	//òJì≠é“
		10,	//ñÿÇ±ÇË
		10,	//ì©å|êEêl
		20,	//éÎêl
		10,	//édóßÇƒâÆ
		5,	//ãôét
		5,	//ãôét
		5,
		5,
	};

	for (int i = 0; i < int(iData.size()); i++) u.baskets.emplace_back(i, u.id);
	for (int i = 0; i<int(cData.size()); i++)
		for (int j = 0; j < numCitizen[i]; j++) u.citizens.emplace_back(int(u.citizens.size()), i, u.id);
	return true;
}