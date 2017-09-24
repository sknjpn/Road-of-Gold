#include"Urban.h"
#include"Group.h"
#include"Vehicle.h"
#include"Node.h"
#include"Planet.h"
#include"Route.h"
#include"UI.h"
#include"Sound.h"
#include"Scuttle.h"
#include"Data.h"
#include"Fleet.h"
#include"VehicleData.h"
#include<lua.hpp>
#include<luaconf.h>

void Main()
{
	{
		Window::SetTitle(L"Road of Gold");

		INIReader iniReader(L"assets/config.ini");
		if (iniReader.getOr<bool>(L"Window", L"FullScreen", false)) Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
		else Window::Resize(iniReader.getOr<Size>(L"Window", L"WindowSize", Size(1280, 720)));
	}

	Output << L"WindowSize:", Window::Size();
	Output << L"FullScreen:", Window::GetState().fullScreen;

	//Fontの展開
	Array<Font> fonts;
	{
		size_t size = 1024;
		fonts.reserve(size);
		ui.fonts.reserve(size);
		for (auto i : step(size)) fonts.emplace_back(i);
		for (auto i : step(size)) ui.fonts.emplace_back(&fonts.at(i));
	}
	ui.fleetNameTextBox = TextBox(*ui.fonts[22], 0, 0, 240);

	initSounds();

	if (!selectMap()) return;

	initGroups();

	Output << L"MainLoopの開始";

	auto bgmItems = FileSystem::DirectoryContents(L"assets/BGM/").filter([](const String& s) { return FileSystem::IsFile(s) && FileSystem::Extension(s) == L"mp3"; });

	lua_getglobal(planet.incidentsLua, "init");
	lua_pcall(planet.incidentsLua, 0, 0, 0);

	while (System::Update())
	{
		if (!bgm.isPlaying() && !bgmItems.isEmpty())
		{
			bgm = Audio(bgmItems.choice());
			bgm.play();
		}

		if (KeyB.down() && !ui.keyControlBlocked)
		{
			for (int j = 0; j < 100; j++)
			{
				for (;;)
				{
					auto* u = &urbans.choice();
					int i = vehicleData.choice().id();
					bool flag = false;
					for (auto* r : u->ownRoutes)
					{
						if (r->movingCost < vehicleData[i].range || r->isSeaRoute == vehicleData[i].isShip)
						{
							flag = true;
							fleets.emplace_back(i, u);
							break;
						}
					}
					if (flag) break;
				}
			}
		}

		tinyCamera.update();
		updateTimeSpeed();
		updateScuttles();
		selectItem();

		updatePlanet();
		updateGroups();
		updateFleets();
		updateUrbans();

		drawPlanet();
		drawRotues();
		drawFleets();
		drawUrbanIcon();
		drawUrbanName();
		drawUI();
		drawScuttles();
	}
}
