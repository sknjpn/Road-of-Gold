#include"Planet.h"
#include"Node.h"
#include"BiomeData.h"

void	initPlanet()
{
	//VoronoiMap‚Ì“Ç‚İ‚İ
	Image image(L"Assets/VoronoiMap.png");
	const bool useOutlineEnabled = false;

	for (auto p : step(image.size()))
	{
		auto& n = nodes.at(image[p.y][p.x].r + (image[p.y][p.x].g << 8) + (image[p.y][p.x].b << 16));

		image[p.y][p.x] = n.color;
	}

	//—ÖŠsü‚Ì•`‰æ
	if (useOutlineEnabled)
	{
		Image original(image);

		for (auto& p1 : step(image.size()))
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
				if (p2.y < 0 || p2.y >= image.size().y) continue;
				if (p2.x < 0) p2.x = image.size().x - 1;
				if (p2.x >= image.size().x) p2.x = 0;
				if (original[p1.y][p1.x] != original[p2.y][p2.x]) image[p1.y][p1.x] = Palette::Black;
			}
		}
	}

	//mapTexture‚É“K—p
	planet.mapTexture = Texture(image);
	Log(L"MapTextrue‚Ìì¬‚ªŠ®—¹");

	//TinyCamera‚Ìİ’è
	tinyCamera.outputRegion = Window::ClientRect();
}