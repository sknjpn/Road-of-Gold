#include"Planet.h"
#include"Pi.h"

TinyCamera2D::TinyCamera2D()
	: restrictedRegion(-TwoPi, -HalfPi,TwoPi,HalfPi)
	, drawingRegion(-Pi,-HalfPi,Pi,HalfPi)
	, smoothDrawingRegion(drawingRegion)
{}

void TinyCamera2D::update()
{

}