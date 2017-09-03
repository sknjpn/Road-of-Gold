#include"Planet.h"
#include"Node.h"
#include"BiomeData.h"

void	initPlanet()
{
	//VoronoiMap‚Ì“Ç‚İ‚İ
	Image image(L"Assets/NodeMap/VoronoiMap.png");

	for (auto p : step(image.size()))
	{
		auto& n = nodes.at(image[p.y][p.x].r + (image[p.y][p.x].g << 8) + (image[p.y][p.x].b << 16));

		image[p.y][p.x] = n.color;
	}

	//mapTexture‚É“K—p
	planet.mapTexture = Texture(image);
	Log(L"MapTextrue‚Ìì¬‚ªŠ®—¹");

	//TinyCamera‚Ìİ’è
	tinyCamera.outputRegion = Window::ClientRect();
}