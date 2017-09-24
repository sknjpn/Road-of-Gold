#pragma once

struct ColorPalette
{
	double	h, s, v;
	Vec2	pos;
	DynamicTexture	texture;

	ColorPalette(const Vec2& _pos = { 0.0, 0.0 });
	ColorPalette(const Point& _pos = { 0, 0 });
	ColorPalette(double _x = 0.0, double _y = 0.0);
	bool	update();
	void	draw() const;
	Color	color() const;
	void	makeImage();
};