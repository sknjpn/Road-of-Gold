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
#include"VehicleData.h"

void Main()
{
	{
		Window::SetTitle(L"Road of Gold");

		INIReader iniReader(L"assets/config.ini");
		if (iniReader.getOr<bool>(L"Window", L"FullScreen", false)) Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
		else Window::Resize(iniReader.getOr<Size>(L"Window", L"WindowSize", Size(1280, 720)));
	}

	LOG_INFO(L"WindowSize:", Window::Size());
	LOG_INFO(L"FullScreen:", Window::GetState().fullScreen);

	//Fontの展開
	Array<Font> fonts;
	for (auto i : step(int(1024))) fonts.emplace_back(i);
	for (auto i : step(int(fonts.size()))) ui.fonts.emplace_back(&fonts.at(i));
	LOG_INFO(L"fontsの展開が完了 size = ", fonts.size());

	initSounds();

	if (!selectMap()) return;

	initGroups();

	LOG_INFO(L"MainLoopの開始");

	auto bgmItems = FileSystem::DirectoryContents(L"assets/BGM/").filter([](const String& s) { return FileSystem::IsFile(s) && FileSystem::Extension(s) == L"mp3"; });

	while (System::Update())
	{
		if (!bgm.isPlaying() && !bgmItems.isEmpty())
		{
			bgm = Audio(bgmItems.choice());
			bgm.play();
		}

		if (KeyB.down())
		{
			for (int i = 0; i < 100; i++) vehicles.emplace_back(vehicleData.choice().id(), &urbans.choice());
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
