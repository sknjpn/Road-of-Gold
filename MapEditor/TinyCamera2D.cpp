#include"Planet.h"
#include"Pi.h"

TinyCamera2D::TinyCamera2D()
	: restrictedRegion(-TwoPi, -HalfPi,TwoPi,HalfPi)
	, drawingRegion(-Pi,-HalfPi,Pi,HalfPi)
	, smoothDrawingRegion(drawingRegion)
{}

void TinyCamera2D::update()
{
	//éãì_à⁄ìÆèàóù
	const auto t1 = createTransformer();
	if (!KeyControl.pressed())
		drawingRegion = drawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*double(Mouse::Wheel()));

	if (gazePoint)	//íçéãì_
	{
		auto p = gazePoint.value().mPos;
		for (int i = -1; i <= 2; i++)
		{
			if (p.distanceFrom(drawingRegion.center()) > gazePoint.value().mPos.movedBy(i*TwoPi, 0).distanceFrom(drawingRegion.center()))
				p = gazePoint.value().mPos.movedBy(i*TwoPi, 0);
		}
		drawingRegion.setCenter(p);
	}

	//drawingRegionÇîÕàÕì‡Ç…é˚ÇﬂÇÈ
	if (drawingRegion.size.y > Pi) drawingRegion = drawingRegion.scaledAt(drawingRegion.center(), (Pi / drawingRegion.size.y));
	if (drawingRegion.pos.y < -HalfPi) drawingRegion.pos.y = -HalfPi;
	if (drawingRegion.pos.y + drawingRegion.size.y > HalfPi) drawingRegion.pos.y = HalfPi - drawingRegion.size.y;
	if (drawingRegion.pos.x < -Pi) { drawingRegion.pos.x += TwoPi; smoothDrawingRegion.pos.x += TwoPi; }
	if (drawingRegion.pos.x > Pi) { drawingRegion.pos.x -= TwoPi; smoothDrawingRegion.pos.x -= TwoPi; }

	//smoothDrawingRegionÇÃí«è]
	const double followingSpeed = 0.2;
	smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + drawingRegion.pos*followingSpeed;
	smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + drawingRegion.size*followingSpeed;

}