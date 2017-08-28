#include"Urban.h"
#include"Group.h"
#include"Vehicle.h"
#include"Node.h"
#include"Planet.h"
#include"Route.h"
#include"UI.h"
#include"Sound.h"
#include"Scuttle.h"
#include"Incident.h"

#include"BiomeData.h"
#include"EnergyData.h"
#include"CitizenData.h"
#include"VehicleData.h"
#include"ItemData.h"

void Main()
{
	JSONReader json(L"起動設定.json");
	if (json[L"FullScreen"].getOr<bool>(false)) Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
	else Window::Resize(1280, 720);
	Window::SetTitle(L"Road of Gold v2.00");

	Log(L"WindowSIze:", Window::Size());
	Log(L"FullScreen:", Window::GetState().fullScreen);

	//Fontの展開
	Array<Font> fonts;
	for (auto i : step(int(1024))) fonts.emplace_back(i);
	for (auto i : step(int(fonts.size()))) ui.fonts.emplace_back(&fonts.at(i));
	Log(L"fontsの展開が完了 size = ", fonts.size());

	System::SetExitEvent(WindowEvent::CloseButton);

	loadItemData();
	loadBiomeData();
	loadEnergyData();
	loadCitizenData();
	loadVehicleData();

	initSounds();

	initNodes();

	loadMap();

	initRoutes();

	initPlanet();

	initGroups();

	Log(L"MainLoopの開始");

	auto bgmItems = FileSystem::DirectoryContents(L"Assets/BGM/").filter([](const String& s) { return FileSystem::IsFile(s) && FileSystem::Extension(s) == L"mp3"; });


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
		//drawRotues();
		drawVehicles();
		drawUrbans();
		drawGroups();
		drawUI();
		drawScuttles();
	}
}
