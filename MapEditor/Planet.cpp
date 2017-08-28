#include"Planet.h"
#include"Pi.h"
#include"Node.h"
#include"BiomeData.h"

Planet::Planet()
	: mapTexture()
{}

void	Planet::updateImage(Array<Node*> _nodeList)
{
	for (auto n : _nodeList)
	{
		const auto& p = (n->pos.mPos / TwoPi).movedBy(0.5, 0.25)*voronoiMap.size().x;
		Array<Point> temp;
		temp.emplace_back(int(p.x), int(p.y));
		while (!temp.isEmpty())
		{
			for (int m = 0; m < 4; m++)
			{
				Point p1 = temp.front();
				Point p2 = temp.front();
				switch (m)
				{
				case 0: p2 = { p1.x - 1,p1.y }; break;
				case 1: p2 = { p1.x + 1,p1.y }; break;
				case 2: p2 = { p1.x ,p1.y - 1 }; break;
				case 3: p2 = { p1.x ,p1.y + 1 }; break;
				}
				//—áŠO”»’è
				if (p2.y < 0 || p2.y >= voronoiMap.size().y) continue;
				if (p2.x < 0) p2.x = voronoiMap.size().x - 1;
				if (p2.x >= voronoiMap.size().x) p2.x = 0;
				if (voronoiMap[p1.y][p1.x] == voronoiMap[p2.y][p2.x] && image[p2.y][p2.x] != biomeData[n->biomeType].color)
				{
					temp.emplace_back(p2);
					image[p2.y][p2.x] = biomeData[n->biomeType].color;
				}
			}
			temp.pop_front();
		}
	}
	if (!_nodeList.isEmpty()) mapTexture.fill(image);
}