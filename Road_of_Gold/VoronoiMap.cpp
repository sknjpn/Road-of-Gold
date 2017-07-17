#include"Planet.h"
#include"Node.h"
#include"Pi.h"


bool	Planet::loadVoronoiMap()
{
	Image reader(L"Assets/VoronoiMap.png");
	if (!reader.isEmpty())
	{
		voronoiMap.resize(reader.size());
		for (auto p : step(reader.size()))
		{
			voronoiMap[p.y][p.x] = reader[p.y][p.x].r + (reader[p.y][p.x].g << 8) + (reader[p.y][p.x].b << 16);
		}
	}
	else return false;

	Image image(reader.size());

	const Array<std::pair<Color, double>> dph = {
		{ Palette::White,		 2.00 },
		{ Palette::White,		 0.90 },
		{ Palette::Sandybrown,	 0.85 },
		{ Palette::Sandybrown,	 0.80 },
		{ Palette::Green,		 0.65 },
		{ Palette::Green,		 0.60 },
		{ Palette::Blue,		 0.5999 },
		{ Palette::Blue,		 0.45 },
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
	for (auto& p : step(reader.size()))
		if (voronoiMap[p.y][p.x] != -1)
			image[p.y][p.x] = nodes[voronoiMap[p.y][p.x]].clr;
	mapTexture = Texture(image);

	return true;
}