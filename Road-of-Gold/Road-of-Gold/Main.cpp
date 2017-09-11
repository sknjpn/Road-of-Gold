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

void Main()
{
	{
		Window::SetTitle(L"Road of Gold");

		INIReader iniReader(L"assets/config.ini");
		if (iniReader.getOr<bool>(L"Window", L"FullScreen", false)) Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
		else Window::Resize(iniReader.getOr<Size>(L"Window", L"WindowSize", Size(1280, 720)));
	}

	Log(L"WindowSize:", Window::Size());
	Log(L"FullScreen:", Window::GetState().fullScreen);

	//Fontの展開
	Array<Font> fonts;
	for (auto i : step(int(1024))) fonts.emplace_back(i);
	for (auto i : step(int(fonts.size()))) ui.fonts.emplace_back(&fonts.at(i));
	Log(L"fontsの展開が完了 size = ", fonts.size());

	loadData();

	initSounds();

	initNodes();

	selectMap();

	initRoutes();

	initGroups();

	Log(L"MainLoopの開始");

	auto bgmItems = FileSystem::DirectoryContents(L"assets/BGM/").filter([](const String& s) { return FileSystem::IsFile(s) && FileSystem::Extension(s) == L"mp3"; });

	while (System::Update())
	{
		if (!bgm.isPlaying() && !bgmItems.isEmpty())
		{
			bgm = Audio(bgmItems.choice());
			bgm.play();
		}

		updatePlanet();
		updateVehicles();
		updateUrbans();
		updateGroups();
		updateVehicles();
		updateScuttles();

		drawPlanet();
		drawRotues();
		drawVehicles();
		drawUrbans();
		drawGroups();
		drawUI();
		drawScuttles();
	}
}
