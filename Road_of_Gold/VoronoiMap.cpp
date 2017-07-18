#include"Planet.h"
#include"Node.h"
#include"Pi.h"


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

	for (auto& n : nodes)
	{
		if (n.isSea)
		{
			if (getHeight(n.pos) < 0.30) n.biome = Biome::Ocean;
			else  n.biome = Biome::Sea;
		}
		else
		{
			const Biome biomeType[6][6]{
				//COLDEST     //COLDER       //COLD               //HOT                       //HOTTER                    //HOTTEST
				{ Biome::Ice, Biome::Tundra, Biome::Grassland,    Biome::Desert,              Biome::Desert,              Biome::Desert },              //DRYEST
				{ Biome::Ice, Biome::Tundra, Biome::Grassland,    Biome::Desert,              Biome::Desert,              Biome::Desert },              //DRYER
				{ Biome::Ice, Biome::Tundra, Biome::Woodland,     Biome::Woodland,            Biome::Savanna,             Biome::Savanna },             //DRY
				{ Biome::Ice, Biome::Tundra, Biome::BorealForest, Biome::Woodland,            Biome::Savanna,             Biome::Savanna },             //WET
				{ Biome::Ice, Biome::Tundra, Biome::BorealForest, Biome::SeasonalForest,      Biome::TropicalRainforest,  Biome::TropicalRainforest },  //WETTER
				{ Biome::Ice, Biome::Tundra, Biome::BorealForest, Biome::TemperateRainforest, Biome::TropicalRainforest,  Biome::TropicalRainforest }   //WETTEST
			};
			n.biome = biomeType[n.moistureLevel][n.temperatureLevel];
		}
		switch (n.biome)
		{
		case Biome::Ice: n.color = Color(255, 255, 255); break;
		case Biome::Desert: n.color = Color(238, 218, 130); break;
		case Biome::Savanna: n.color = Color(177, 209, 110); break;
		case Biome::TropicalRainforest: n.color = Color(66, 123, 25); break;
		case Biome::Tundra: n.color = Color(96, 131, 112); break;
		case Biome::TemperateRainforest: n.color = Color(29, 73, 40); break;
		case Biome::Grassland: n.color = Color(164, 225, 99); break;
		case Biome::SeasonalForest: n.color = Color(73, 100, 35); break;
		case Biome::BorealForest: n.color = Color(95, 115, 62); break;
		case Biome::Woodland: n.color = Color(139, 175, 90); break;
		case Biome::Sea: n.color = Color(L"#0F285A"); break;
		case Biome::Ocean: n.color = Color(L"#0F1E50"); break;
		}
	}


	//F‚Ì‚Ú‚©‚µ
	for (auto& n : nodes)
	{
		const auto h = getHeight(n.pos);
		n.color = n.color.lerp(RandomColor(), 0.05);
	}

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