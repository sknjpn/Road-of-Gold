#pragma once
#include"Pos.h"

struct TinyCamera
{
	RectF	restrictedRegion;	//‹“_ˆÚ“®§ŒÀ—Ìˆæ
	RectF	drawingRegion;		//‰¼‘z•`‰æ—Ìˆæ
	RectF	smoothDrawingRegion;//À•`‰æ—Ìˆæ
	Optional<Pos>	gazePoint;	//’‹“_

	TinyCamera();
	void	update();
	double	getMagnification() const;
	Pos		getCursorPos() const;
	Mat3x2	getMat3x2(int _delta = 0, double _r = 1) const;
	Transformer2D createTransformer(int _delta = 0, double _r = 1) const;
};

extern TinyCamera tinyCamera;