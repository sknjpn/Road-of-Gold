#include"Urban.h"
#include"Group.h"
#include"Vehicle.h"
#include"Node.h"
#include"Planet.h"
#include"Route.h"
#include"Display.h"
#include"Sound.h"
#include"Scuttle.h"
#include"Data.h"
#include"Fleet.h"
#include"VehicleData.h"
#include"River.h"
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

	Output << L"WindowSize:" << Window::Size();
	Output << L"FullScreen:" << Window::GetState().fullScreen;

	//Fontの展開
	Array<Font> efonts;
	{
		size_t size = 1024;
		globalFonts.reserve(size);
		efonts.reserve(size);
		for (auto i : step(size)) efonts.emplace_back(int(i));
		for (auto i : step(size)) globalFonts.emplace_back(&efonts.at(i));
	}

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

		tinyCamera.update();
		if (routeMaker.targetFleet == nullptr)
		{
			updateTimeSpeed();
			updatePlanet();
			updateGroups();
			updateFleets();
			updateUrbans();
		}

		updateScuttles();
		drawPlanet();
		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);
			for (auto& r : rivers) r.draw();
		}
		for (auto& r : routes)
		{
			if (r.drawFlag)
			{
				r.drawFlag = false;
				r.draw();
			}
		}
		drawFleets();
		drawUrbanIcon();
		drawUrbanName();
		updateDisplay();
		drawScuttles();

		if (KeyF11.down())
		{
			if (Window::GetState().fullScreen)
			{
				INIReader iniReader(L"assets/config.ini");
				Graphics::SetFullScreen(false, iniReader.getOr<Size>(L"Window", L"WindowSize", Size(1280, 720)));
			}
			else Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
			System::Update();
		}
	}
}
