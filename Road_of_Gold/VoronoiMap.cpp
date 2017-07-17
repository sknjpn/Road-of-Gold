#include"Planet.h"
#include"Node.h"
#include"Pi.h"


void	Planet::loadVoronoiMap(int _sizeX)
{
	Image image(_sizeX, _sizeX / 2);
	voronoiMap.resize(_sizeX, _sizeX / 2);
	voronoiMap.fill(-1);

	BinaryReader reader(Format(L"Assets/VoronoiMap", _sizeX, L".bin"));
	if (reader.isOpened())
	{
		for (auto p : step(Size(_sizeX, _sizeX / 2)))
		{
			reader.read(voronoiMap[p.y][p.x]);
		}
	}
	else
	{
		Grid<int> temp(_sizeX, _sizeX / 2);
		DynamicTexture tex;
		Stopwatch tmr(true);
		int	LastUpdateA = 0;
		int	LastUpdateB = 0;

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
					for (auto p1 : i == 0 ? step(Size(_sizeX, _sizeX / 4 - LastUpdateB)) : step(Point(0, _sizeX / 4 + LastUpdateB), Size(_sizeX, _sizeX / 4 - LastUpdateB)))
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

		//ファイル書き出し
		BinaryWriter writer(Format(L"Assets/VoronoiMap", _sizeX, L".bin"));
		for (auto p : step(Size(_sizeX, _sizeX / 2)))
		{
			writer.write(voronoiMap[p.y][p.x]);
		}
		writer.close();
	}
	const Array<std::pair<Color, double>> dph = {
		{ Palette::White,		 2.00 },
		{ Palette::White,		 0.90 },
		{ Palette::Sandybrown,	 0.85 },
		{ Palette::Sandybrown,	 0.80 },
		{ Palette::Green,		 0.65 },
		{ Palette::Green,		 0.60 },
		{ Palette::Blue,		 0.59 },
		{ Palette::Blue,		 0.40 },
		{ Palette::Darkblue,	 0.30 },
		{ Palette::Darkblue,	-1.00 },
	};
	for (auto& n : nodes)
	{
		const auto h = getHeight(n.pos);
		for (int i = 0; i<int(dph.size()); i++)
		{
			if (dph[i].second < h)
			{
				n.clr = dph[i - 1].first.lerp(dph[i].first, (dph[i - 1].second - h) / (dph[i - 1].second - dph[i].second));
				break;
			}
		}
		n.clr = n.clr.lerp(RandomColor(), 0.1);
	}
	for (auto& p : step(Size(_sizeX, _sizeX / 2)))
		if (voronoiMap[p.y][p.x] != -1)
			image[p.y][p.x] = nodes[voronoiMap[p.y][p.x]].clr;
	mapTexture = Texture(image);

}
void	Planet::makeMapTexture()
{
}