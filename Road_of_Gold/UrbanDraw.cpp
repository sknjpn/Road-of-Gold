#include"Urban.h"
#include"Node.h"
#include"Pos.h"
void	Urban::draw() const
{
	const Circle circle(getPos().mPos, 0.01);
	const Color color = selectedUrban == this ? Palette::Yellow : (circle.mouseOver() ? Palette::Orange : Palette::Red);
	circle.draw(color).drawFrame(0.005, Palette::Black);
}
Pos&	Urban::getPos() const { return nodes[joinedNodeID].pos; }