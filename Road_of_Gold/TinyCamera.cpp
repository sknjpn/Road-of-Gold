#include"TinyCamera.h"

TinyCamera::TinyCamera()
	: restrictedRegion(-180_deg, -90_deg, 360_deg, 90_deg)
	, drawingRegion(-180_deg, -90_deg, 180_deg, 90_deg)
	, smoothDrawingRegion(drawingRegion)
	, gazePoint(none)
{}

void TinyCamera::update()
{
	{
		//視点移動処理
		const auto t1 = createTransformer();

		//ズーム
		{
			double delta = Mouse::Wheel();
			if (drawingRegion.size.y >= 180_deg && delta > 0) delta = 0;	//最大ズーム時にズームの制限
			if (drawingRegion.size.y <= 1.8_deg && delta < 0) delta = 0;	//最小ズーム時にズームの制限
			drawingRegion = drawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*delta);
		}

		//注視点
		if (gazePoint)
		{
			auto p = gazePoint.value().mPos;
			for (int i = -1; i <= 2; ++i)
			{
				if (p.distanceFrom(drawingRegion.center()) > gazePoint.value().mPos.movedBy(i * 360_deg, 0).distanceFrom(drawingRegion.center()))
					p = gazePoint.value().mPos.movedBy(i * 360_deg, 0);
			}
			drawingRegion.setCenter(p);

			//クリア
			gazePoint = none;
		}

		//drawingRegionを範囲内に収める
		if (drawingRegion.size.y > 180_deg) drawingRegion = drawingRegion.scaledAt(drawingRegion.center(), (180_deg / drawingRegion.size.y));
		if (drawingRegion.pos.y < -90_deg) drawingRegion.pos.y = -90_deg;
		if (drawingRegion.pos.y + drawingRegion.size.y > 90_deg) drawingRegion.pos.y = 90_deg - drawingRegion.size.y;
		if (smoothDrawingRegion.pos.x < -180_deg) { drawingRegion.pos.x += 360_deg; smoothDrawingRegion.pos.x += 360_deg; }
		if (smoothDrawingRegion.pos.x > 180_deg) { drawingRegion.pos.x -= 360_deg; smoothDrawingRegion.pos.x -= 360_deg; }

		//smoothDrawingRegionの追従
		const double followingSpeed = 0.2;
		smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + drawingRegion.pos*followingSpeed;
		smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + drawingRegion.size*followingSpeed;
	}
	//スライダー
	const double slidingSpeed = (drawingRegion.size.y / 180_deg)*0.05;
	const bool useKeyViewControl = true;
	if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x <= 0) drawingRegion.pos.x -= slidingSpeed;
	if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y <= 0) drawingRegion.pos.y -= slidingSpeed;
	if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x >= Window::Size().x - 1) drawingRegion.pos.x += slidingSpeed;
	if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y >= Window::Size().y - 1) drawingRegion.pos.y += slidingSpeed;
}

Pos TinyCamera::getCursorPos() const
{
	return getMat3x2().inversed().transform(Cursor::PosF());
}

Mat3x2 TinyCamera::getMat3x2(int _delta) const
{
	return Mat3x2::Translate(-smoothDrawingRegion.center().movedBy(-_delta * 360_deg, 0.0)).scaled(Window::Size().y / smoothDrawingRegion.size.y).translated(Window::ClientRect().center());
}

Transformer2D TinyCamera::createTransformer(int _delta) const
{
	return Transformer2D(getMat3x2(_delta), true);
}
