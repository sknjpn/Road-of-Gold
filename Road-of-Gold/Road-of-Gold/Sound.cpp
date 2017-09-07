#include"Sound.h"

Array<Audio> sounds;
Audio	bgm;

void	initSounds()
{
	auto items = FileSystem::DirectoryContents(L"assets/sound/");
	for (auto item : items)
		sounds.emplace_back(item);
}