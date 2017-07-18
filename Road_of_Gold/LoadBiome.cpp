#include"Planet.h"
#include"Node.h"

bool	Planet::loadBiome()
{
	auto items = FileSystem::DirectoryContents(L"Map/");

	for (auto& item : items)
	{
		if (item.indexOf(L".bin") != FilePath::npos)
		{
			BinaryReader reader(item);
			for (auto& n : nodes)
			{
				int t;
				reader.read(t);
				n.biomeType = t;
				n.color = bData[n.biomeType].color.lerp(RandomColor(),0.05);
			}
			return true;
		}
	}
	return false;
}
