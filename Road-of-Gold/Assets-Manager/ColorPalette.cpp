#include"ColorPalette.h"

ColorPalette::ColorPalette(double _x, double _y)
	: ColorPalette(Vec2(_x, _y)) {}
ColorPalette::ColorPalette(const Point& _pos)
	: ColorPalette(Vec2(_pos)) {}
ColorPalette::ColorPalette(const Vec2& _pos)
	: h(0.0)
	, s(1.0)
	, v(1.0)
	, pos(_pos)
{
	makeImage();
}
bool	ColorPalette::update()
{
	bool result = false;
	auto center = pos.movedBy(128.0, 128.0);
	if ((Cursor::Pos() - center).length() >= 96 && (Cursor::Pos() - center).length() <= 128 && MouseL.pressed() && h != atan2((Cursor::Pos() - center).y, (Cursor::Pos() - center).x) / 1_deg)
	{
		h = atan2((Cursor::Pos() - center).y, (Cursor::Pos() - center).x) / 1_deg;
		result = true;
		makeImage();
	}
	if (RectF(pos.movedBy(64, 64), Size(128, 128)).leftPressed())
	{
		auto p = Cursor::Pos().movedBy(-pos).movedBy(-64, -64);
		v = (128 - p.y) / 128.0;
		s = p.x / 128.0;
		result = true;
	}
	return result;
}
void	ColorPalette::draw() const
{
	auto center = pos.movedBy(128.0, 128.0);
	auto p = Cursor::Pos() - center;
	texture.draw(pos);
	Circle circle(center.movedBy(Vec2(112, 0).rotated(h * 1_deg)), 16);
	circle.draw(Palette::White);
	if (p.length() >= 96 && p.length() <= 128) circle.drawFrame(4, Palette::Orange);
	else circle.drawFrame(4, Palette::Gray);
	Circle(pos.movedBy(64 + s*128.0, 192 - v*128.0), 4).draw(Palette::Gray);
}
Color	ColorPalette::color() const { return HSV(h, s, v); }

void	ColorPalette::makeImage()
{
	Image image(256, 256);
	auto func = [](const Vec2& pos)
	{
		auto p = pos.movedBy(-128.0, -128.0);
		if (p.length() > 96 && p.length() < 128) return Color(HSV(atan2(p.y, p.x) / 1_deg));
		else return Palette::White;
	};

	for (auto p : step(image.size())) image[p.y][p.x] = func(Vec2(p));
	for (auto p : step(Point(64, 64), Size(128, 128))) image[p.x][p.y] = HSV(h, (p.y - 64) / 128.0, (192 - p.x) / 128.0);
	texture.fill(image);
}