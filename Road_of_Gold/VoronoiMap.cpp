#include"Planet.h"
#include"Node.h"
#include"Pi.h"

void	Planet::makeVoronoiMap(int _sizeX)
{
	Image image(_sizeX, _sizeX / 2);
	image.fill(Palette::White);
	for (auto& n : nodes)
	{
		const auto& p = (n.pos.mPos / TwoPi).movedBy(0.5, 0.25)*_sizeX;

		image[int(p.y)][int(p.x)] = HSV(n.isSea ? Random(200.0, 220.0) : Random(110.0, 130.0), 0.5, 0.5);
	}
	Image temp;
	for (;;)
	{
		bool flag = true;
		temp.assign( image);
		for (auto& p1 : step(Size(_sizeX, _sizeX / 2)))
		{
			for (auto& p2 : step(Point(-1, -1), Size(3, 3)))
			{
				auto p3 = p1 + p2;
				if (p2.x == 0 && p2.y == 0) continue;
				if (p2.x != 0 && p2.y != 0) continue;
				if (p3.x < 0) p3.x = _sizeX - 1;
				if (p3.y < 0) continue;
				if (p3.x >= _sizeX) p3.x = 0;
				if (p3.y >= _sizeX / 2) continue;
				auto& o2 = image[p3.y][p3.x];
				auto& o1 = image[p1.y][p1.x];
				if (o1 != o2 && o2 == Palette::White)
				{
					temp[p3.y][p3.x] = temp[p1.y][p1.x];
					flag = false;
				}
			}
		}
		image = temp;
		if (flag) break;
	}
	mapTexture = Texture(image);

}