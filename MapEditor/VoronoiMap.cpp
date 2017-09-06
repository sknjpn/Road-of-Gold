#include"Planet.h"
#include"Node.h"


bool	Planet::loadVoronoiMap()
{
	//NodeÇÃì«Ç›çûÇ›
	{
		BinaryReader reader(L"assets/nodemap/nodeMap.bin");
		if (!reader) return false;	//ì«Ç›çûÇ›é∏îs

		int	nodesSize, pathsSize;
		reader.read(nodesSize);
		nodes.reserve(nodesSize);
		for (int i = 0; i < nodesSize; ++i)
		{
			Vec3 ePos;
			reader.read(ePos);
			nodes.emplace_back(ePos);
		}
		reader.read(pathsSize);
		for (int i = 0; i < pathsSize; ++i)
		{
			int parentNodeID, childNodeID;
			reader.read(parentNodeID);
			reader.read(childNodeID);
			nodes[parentNodeID].paths.emplace_back(parentNodeID, childNodeID);
		}
		for (auto& n : nodes)
		{
			for (auto& p : n.paths) paths.emplace_back(&p);
		}
	}


	//VoronoiMapÇÃì«Ç›çûÇ›
	{
		Image reader(L"assets/nodemap/voronoiMap.png");
		if (!reader.isEmpty())
		{
			voronoiMap.resize(reader.size());
			for (auto p : step(reader.size()))
			{
				voronoiMap[p.y][p.x] = reader[p.y][p.x].r + (reader[p.y][p.x].g << 8) + (reader[p.y][p.x].b << 16);
			}
		}
		else return false;

		//ImageÇ…êFÇì]é 
		image.resize(reader.size());
		for (auto& p : step(reader.size()))
		{
			if (voronoiMap[p.y][p.x] != -1) image[p.y][p.x] = nodes[voronoiMap[p.y][p.x]].getColor();
		}

		//ó÷äsê¸ÇÃï`âÊ
		Image outlineImage(reader.size());
		for (auto& p1 : step(reader.size()))
		{
			for (int m = 0; m < 4; m++)
			{
				Point p2 = p1;
				switch (m)
				{
				case 0: p2 = { p1.x - 1, p1.y }; break;
				case 1: p2 = { p1.x + 1, p1.y }; break;
				case 2: p2 = { p1.x, p1.y - 1 }; break;
				case 3: p2 = { p1.x, p1.y + 1 }; break;
				}
				//ó·äOîªíË
				if (p2.y < 0 || p2.y >= reader.size().y) continue;
				if (p2.x < 0) p2.x = reader.size().x - 1;
				if (p2.x >= reader.size().x) p2.x = 0;
				if (voronoiMap[p1.y][p1.x] != voronoiMap[p2.y][p2.x]) outlineImage[p1.y][p1.x] = Palette::Black;
			}
		}
		outlineTexture = Texture(outlineImage);

		mapTexture.fill(image);
		return true;
	}
}