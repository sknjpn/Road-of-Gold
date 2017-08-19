#include"Sound.h"

Array<Audio> sounds;
Audio	bgm;

void	initSounds()
{
	auto items = FileSystem::DirectoryContents(L"Assets/Sound/");
	for (auto item : items)
		sounds.emplace_back(item);
}