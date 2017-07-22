#include"Planet.h"
#include"Pi.h"
#include"Pos.h"

TinyCamera2D::TinyCamera2D()
	: restrictedRegion(-TwoPi, -HalfPi, TwoPi, HalfPi)
	, drawingRegion(-Pi, -HalfPi, Pi, HalfPi)
	, smoothDrawingRegion(drawingRegion)
	, gazePoint(none)
{}

void TinyCamera2D::update()
{
	{
		//視点移動処理
		const auto t1 = createTransformer();
		if (!KeyControl.pressed())
			drawingRegion = drawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*double(Mouse::Wheel()));

		if (gazePoint)	//注視点
		{
			auto p = gazePoint.value().mPos;
			for (int i = -1; i <= 2; i++)
			{
				if (p.distanceFrom(drawingRegion.center()) > gazePoint.value().mPos.movedBy(i*TwoPi, 0).distanceFrom(drawingRegion.center()))
					p = gazePoint.value().mPos.movedBy(i*TwoPi, 0);
			}
			drawingRegion.setCenter(p);
		}

		//drawingRegionを範囲内に収める
		if (drawingRegion.size.y > Pi) drawingRegion = drawingRegion.scaledAt(drawingRegion.center(), (Pi / drawingRegion.size.y));
		if (drawingRegion.pos.y < -HalfPi) drawingRegion.pos.y = -HalfPi;
		if (drawingRegion.pos.y + drawingRegion.size.y > HalfPi) drawingRegion.pos.y = HalfPi - drawingRegion.size.y;
		if (drawingRegion.pos.x < -Pi) { drawingRegion.pos.x += TwoPi; smoothDrawingRegion.pos.x += TwoPi; }
		if (drawingRegion.pos.x > Pi) { drawingRegion.pos.x -= TwoPi; smoothDrawingRegion.pos.x -= TwoPi; }

		//smoothDrawingRegionの追従
		const double followingSpeed = 0.2;
		smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + drawingRegion.pos*followingSpeed;
		smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + drawingRegion.size*followingSpeed;
	}
	//スライダー
	const double slidingSpeed = (drawingRegion.size.y / Pi)*0.05;
	const bool useKeyViewControl = true;
	if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x < 32) drawingRegion.pos.x -= slidingSpeed;
	if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y < 32) drawingRegion.pos.y -= slidingSpeed;
	if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x > Window::Size().x - 32) drawingRegion.pos.x += slidingSpeed;
	if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y > Window::Size().y - 32) drawingRegion.pos.y += slidingSpeed;
}
void TinyCamera2D::draw() const
{
	//スライダー
	const ColorF color(Palette::White, 0.3);
	const bool useKeyViewControl = true;
	if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x < 32) RectF(32, Window::Size().y).draw(color);
	if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y < 32) RectF(Window::Size().x, 32).draw(color);
	if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x > Window::Size().x - 32) RectF(Window::Size().x - 32, 0, 32, Window::Size().y).draw(color);
	if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y > Window::Size().y - 32) RectF(0, Window::Size().y - 32, Window::Size().x, 32).draw(color);
}
Pos TinyCamera2D::getCursorPos() const
{
	return getMat3x2().inverse().transform(Cursor::PosF());
}
Mat3x2 TinyCamera2D::getMat3x2(int _delta) const
{
	return Mat3x2::Translate(-smoothDrawingRegion.center().movedBy(-_delta*TwoPi, 0.0)).scale(Window::Size().y / smoothDrawingRegion.size.y).translate(Window::Center());
}
Transformer2D TinyCamera2D::createTransformer(int _delta) const
{
	return Transformer2D(getMat3x2(_delta), true);
}
