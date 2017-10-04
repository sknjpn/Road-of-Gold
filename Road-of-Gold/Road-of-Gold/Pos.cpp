#include"Pos.h"

//Å’Zƒ‹[ƒg‚Ìü‚ğ•Ô‚·
Line	getLine(const Pos& p1, const Pos& p2)
{
	if (Abs(p1.mPos.x - p2.mPos.x) <= 180_deg) return Line(p1.mPos, p2.mPos);
	if (p1.mPos.x > p2.mPos.x) return Line(p1.mPos, p2.mPos.movedBy(180_deg, 0));
	return Line(p1.mPos.movedBy(180_deg, 0), p2.mPos);
}