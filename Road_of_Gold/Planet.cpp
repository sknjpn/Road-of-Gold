#include "Planet.h"
#include "Pi.h"

Planet::Planet()
	: drawingRegion(-Pi, -HalfPi, TwoPi, Pi)
	, smoothDrawingRegion(drawingRegion)
	, mapTexture()
	, heightNoise(Random(UINT32_MAX - 1))
	, gazePoint(none)
{}
double Planet::getHeight(const Pos& _pos, const int& _octave) const
{
	return Min(Max((heightNoise.octaveNoise(_pos.ePos, _octave) + 1.0) / 2.0, 0.0), 1.0);
}
bool	Planet::isSea(const Pos& _pos) const { return getHeight(_pos) < 0.60; }
Transformer2D Planet::createTransformer(const int& _delta) const
{
	return Transformer2D(Mat3x2::Translate(-smoothDrawingRegion.center().movedBy(-_delta*TwoPi, 0.0)).scale(Window::Size().y / smoothDrawingRegion.size.y).translate(Window::Center()), true);
}
void	Planet::updateTransform()
{
	//éãì_à⁄ìÆèàóù
	const auto t1 = createTransformer();
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
void	Planet::updateViewPointSliding()
{
	const double slidingSpeed = (drawingRegion.size.y / Pi)*0.05;
	if (Cursor::Pos().x < 32) { drawingRegion.pos.x -= slidingSpeed; RectF(0, 0, 32, Window::Size().y).draw(ColorF(Palette::White, 0.3)); }
	if (Cursor::Pos().y < 32) { drawingRegion.pos.y -= slidingSpeed; RectF(0, 0, Window::Size().x, 32).draw(ColorF(Palette::White, 0.3)); }
	if (Cursor::Pos().x > Window::Size().x - 32) { drawingRegion.pos.x += slidingSpeed; RectF(Window::Size().x - 32, 0, 32, Window::Size().y).draw(ColorF(Palette::White, 0.3)); }
	if (Cursor::Pos().y > Window::Size().y - 32) { drawingRegion.pos.y += slidingSpeed; RectF(0, Window::Size().y - 32, Window::Size().x, 32).draw(ColorF(Palette::White, 0.3)); }

}
void	Planet::makeNewWorld(const int& _sizeX)
{
	heightNoise = PerlinNoise(Random(UINT32_MAX - 1));
	Image image(_sizeX, _sizeX / 2);
	for (const auto& p : step(Size(_sizeX, _sizeX / 2)))
	{
		auto& o = image[p.y][p.x];
		const auto pos = Pos(Vec2(TwoPi*double(p.x) / double(_sizeX) - Pi, Pi*double(p.y) / double(_sizeX / 2) - HalfPi));
		const double height = getHeight(pos);
		if (height <= 0.60) o = Palette::Blue;
		else o = Palette::Green;
		o = Palette::Blue.lerp(Palette::Green, Max(0.0, Min(1.0, (height - 0.59)*50.0)));
	}
	mapTexture = Texture(image);
}
void	Planet::draw() const { for (int i = 0; i < 2; i++) mapTexture.resize(TwoPi, Pi).drawAt(i*TwoPi, 0); }