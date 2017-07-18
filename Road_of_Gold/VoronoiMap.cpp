#include"Planet.h"
#include"Node.h"



bool	Planet::loadVoronoiMap()
{
	const bool useOutlineEnabled = false;
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



	//Image‚ÉF‚ð“]ŽÊ
	Image image(reader.size());
	for (auto& p : step(reader.size()))
		if (voronoiMap[p.y][p.x] != -1)
			image[p.y][p.x] = nodes[voronoiMap[p.y][p.x]].color;

	if (useOutlineEnabled)
	{
		for (auto& p1 : step(reader.size()))
		{
			for (int m = 0; m < 4; m++)
			{
				Point p2 = p1;
				switch (m)
				{
				case 0: p2 = { p1.x - 1,p1.y }; break;
				case 1: p2 = { p1.x + 1,p1.y }; break;
				case 2: p2 = { p1.x ,p1.y - 1 }; break;
				case 3: p2 = { p1.x ,p1.y + 1 }; break;
				}
				//—áŠO”»’è
				if (p2.y < 0 || p2.y >= reader.size().y) continue;
				if (p2.x < 0) p2.x = reader.size().x - 1;
				if (p2.x >= reader.size().x) p2.x = 0;
				if (voronoiMap[p1.y][p1.x] != voronoiMap[p2.y][p2.x]) image[p1.y][p1.x] = Palette::Black;
			}
		}
	}

	mapTexture = Texture(image);
	return true;
}