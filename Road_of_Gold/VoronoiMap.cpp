#include"Planet.h"
#include"Node.h"
#include"Pi.h"


void	Planet::loadVoronoiMap(int _sizeX)
{
	Grid<int> temp(_sizeX, _sizeX / 2);
	Image image(_sizeX, _sizeX / 2);
	DynamicTexture tex;
	Stopwatch tmr(true);
	int	LastUpdateA = 0;
	int	LastUpdateB = 0;

	voronoiMap.resize(_sizeX, _sizeX / 2);
	voronoiMap.fill(-1);
	temp.fill(-1);
	for (auto& n : nodes)
	{
		const auto& p = (n.pos.mPos / TwoPi).movedBy(0.5, 0.25)*_sizeX;
		voronoiMap[int(p.y)][int(p.x)] = n.id;
		temp[int(p.y)][int(p.x)] = n.id;
	}
	for (;;)
	{
		if (!KeyEnter.pressed())
		{
			LastUpdateB = LastUpdateA;
			LastUpdateA = _sizeX / 2;
			bool flag = true;
			for (auto p1 : step(Size(_sizeX, _sizeX / 2))) voronoiMap[p1.y][p1.x] = temp[p1.y][p1.x];	//前回の変更の適用
			for (int i = 0; i < 2; i++)
			{
				for (auto p1 : i==0 ? step(Size(_sizeX, _sizeX / 4 - LastUpdateB)) : step(Point(0, _sizeX / 4 + LastUpdateB), Size(_sizeX, _sizeX / 4 - LastUpdateB)))
				{
					for (auto p2 : step(p1.movedBy(-1, -1), Size(3, 3)))
					{
						//例外判定
						if (p2 == p1 || (p1.x - p2.x != 0 && p1.y - p2.y != 0) || p2.y < 0 || p2.y >= _sizeX / 2) continue;
						if (p2.x < 0) p2.x = _sizeX - 1;
						if (p2.x >= _sizeX) p2.x = 0;

						//干渉
						if (voronoiMap[p1.y][p1.x] != voronoiMap[p2.y][p2.x] && voronoiMap[p2.y][p2.x] != -1)
						{
							bool f = false;
							if (voronoiMap[p1.y][p1.x] == -1) f = true;
							else
							{
								const auto& mPos = (p1 / double(_sizeX / 2)).movedBy(-1, -0.5)*Pi;
								const auto& ePos = Vec3(cos(mPos.x)*cos(mPos.y), -sin(mPos.y), sin(mPos.x)*cos(mPos.y));
								if ((nodes[voronoiMap[p1.y][p1.x]].pos.ePos - ePos).length() > (nodes[voronoiMap[p2.y][p2.x]].pos.ePos - (ePos)).length()) f = true;
							}
							if (f)
							{
								flag = false;
								temp[p1.y][p1.x] = voronoiMap[p2.y][p2.x];
								if (LastUpdateA > abs(_sizeX / 4 - p2.y)) LastUpdateA = abs(_sizeX / 4 - p2.y);
							}
						}
					}
				}
			}
			if (flag) break;
		}
		if (tmr.ms() > 16)
		{
			for (auto& p : step(Size(_sizeX, _sizeX / 2)))
				if (voronoiMap[p.y][p.x] != -1) image[p.y][p.x] = nodes[voronoiMap[p.y][p.x]].clr;
			tex.fill(image);
			tex.resize(Window::Size()).draw();
			if (!System::Update()) return;
		}
	}
	for (auto& p : step(Size(_sizeX, _sizeX / 2)))
	{
		if (voronoiMap[p.y][p.x] != -1) image[p.y][p.x] = nodes[voronoiMap[p.y][p.x]].isSea ? Palette::Blue : Palette::Green;
	}
	mapTexture = Texture(image);

}
void	Planet::makeMapTexture()
{
	/*Image image(_sizeX, _sizeX / 2);
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
	*/
}