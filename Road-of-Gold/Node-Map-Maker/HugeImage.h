#pragma once

struct HugeImage
{
	Grid<Color>	pixels;

	HugeImage(const Size& _size) { pixels.resize(_size); }
	Color&	at(int _y, int _x) { return pixels[_y][_x]; }
	Color&	at(const Point& _p) { return at(_p.y, _p.x); }
	Size	size() const { return pixels.size(); }
	Image	getAsImage(const Rect& _rect) const
	{
		Image image(_rect.size);
		for (auto p : step(_rect.size)) image[p.y][p.x] = pixels[_rect.pos.y + p.y][_rect.pos.x + p.x];
		return image;
	}
	void	saveImg(const Rect& _rect, const FilePath& _path)
	{
		Image image(_rect.size);
		for (auto p : step(_rect.size)) image[p.y][p.x] = pixels[_rect.pos.y + p.y][_rect.pos.x + p.x];
		image.save(_path);
	}
};