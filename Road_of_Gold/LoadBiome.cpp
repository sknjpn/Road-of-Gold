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
			}
			return true;
		}
	}
	return false;
}
