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

		JSONReader json(L"起動設定.json");
		if (json[L"Window"][L"SetFullScreen"].get<bool>())
		{
			Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
		}
		else
		{
			Size size(1280, 720);
			auto s = json[L"Window"][L"WindowSize"].arrayView();
			if (json[L"Window"][L"WindowSize"].arrayCount() == 2)
			{
				if (s[0].getOr<int>(0) > 0 && s[1].getOr<int>(0) > 0)
				{
					size.x = s[0].getOr<int>(0);
					size.y = s[1].getOr<int>(0);
				}
			}
			Window::Resize(size);
		}
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
