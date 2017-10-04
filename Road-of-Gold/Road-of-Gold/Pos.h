#pragma once

struct Pos
{
	Vec3 ePos;
	Vec2 mPos;

	Pos() : ePos(0.0, 0.0, 0.0), mPos(0.0, 0.0) {}
	Pos(const Vec3& _ePos) { set(_ePos); }
	Pos(const Vec2& _mPos) { set(_mPos); }

	double	distanceFrom(const Pos& _pos) const { return ePos.distanceFrom(_pos.ePos); }

	void set(const Vec3& _ePos)
	{
		ePos = _ePos.normalized();
		mPos = Vec2(atan2(ePos.z, ePos.x), -asin(ePos.y));
	}

	void set(const Vec2& _mPos)
	{
		set(Vec3(cos(_mPos.x)*cos(_mPos.y), -sin(_mPos.y), sin(_mPos.x)*cos(_mPos.y)));
	}
};

//ç≈íZÉãÅ[ÉgÇÃê¸Çï‘Ç∑
Line	getLine(const Pos& p1, const Pos& p2);