#include "Planet.h"
#include "Pi.h"
#include "Node.h"

Planet::Planet()
	: drawingRegion(-Pi, -HalfPi, TwoPi, Pi)
	, smoothDrawingRegion(drawingRegion)
	, mapTexture()
	, gazePoint(none)
{}
Pos		Planet::getCursorPos() const
{
	return Mat3x2::Translate(-smoothDrawingRegion.center()).scale(Window::Size().y / smoothDrawingRegion.size.y).translate(Window::Center()).inverse().transform(Cursor::PosF());
}

Transformer2D Planet::createTransformer(int _delta) const
{
	return Transformer2D(Mat3x2::Translate(-smoothDrawingRegion.center().movedBy(-_delta*TwoPi, 0.0)).scale(Window::Size().y / smoothDrawingRegion.size.y).translate(Window::Center()), true);
}

void	Planet::updateTransform()
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

void	Planet::updateViewPointSliding()
{
	const double slidingSpeed = (drawingRegion.size.y / Pi)*0.05;
	if (Cursor::Pos().x < 32) { drawingRegion.pos.x -= slidingSpeed; RectF(0, 0, 32, Window::Size().y).draw(ColorF(Palette::White, 0.3)); }
	if (Cursor::Pos().y < 32) { drawingRegion.pos.y -= slidingSpeed; RectF(0, 0, Window::Size().x, 32).draw(ColorF(Palette::White, 0.3)); }
	if (Cursor::Pos().x > Window::Size().x - 32) { drawingRegion.pos.x += slidingSpeed; RectF(Window::Size().x - 32, 0, 32, Window::Size().y).draw(ColorF(Palette::White, 0.3)); }
	if (Cursor::Pos().y > Window::Size().y - 32) { drawingRegion.pos.y += slidingSpeed; RectF(0, Window::Size().y - 32, Window::Size().x, 32).draw(ColorF(Palette::White, 0.3)); }

}

void	Planet::updateImage(Array<Node*> _nodeList)
{
	for (auto n : _nodeList)
	{
		const auto& p = (n->pos.mPos / TwoPi).movedBy(0.5, 0.25)*voronoiMap.size().x;
		Array<Point> temp;
		temp.emplace_back(int(p.x), int(p.y));
		while (!temp.isEmpty())
		{
			for (int m = 0; m < 4; m++)
			{
				Point p1 = temp.front();
				Point p2 = temp.front();
				switch (m)
				{
				case 0: p2 = { p1.x - 1,p1.y }; break;
				case 1: p2 = { p1.x + 1,p1.y }; break;
				case 2: p2 = { p1.x ,p1.y - 1 }; break;
				case 3: p2 = { p1.x ,p1.y + 1 }; break;
				}
				//ó·äOîªíË
				if (p2.y < 0 || p2.y >= voronoiMap.size().y) continue;
				if (p2.x < 0) p2.x = voronoiMap.size().x - 1;
				if (p2.x >= voronoiMap.size().x) p2.x = 0;
				if (voronoiMap[p1.y][p1.x] == voronoiMap[p2.y][p2.x] && image[p2.y][p2.x] != bData[selectedBiome].color)
				{
					temp.emplace_back(p2);
					image[p2.y][p2.x] = bData[selectedBiome].color;
				}
			}
			temp.pop_front();
		}
		n->biomeType = selectedBiome;
	}
	if(!_nodeList.isEmpty()) mapTexture.fill(image);
}