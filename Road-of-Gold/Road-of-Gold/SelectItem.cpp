#include"UI.h"
#include"Urban.h"
#include"TinyCamera.h"
#include"Sound.h"

void	selectItem()
{
	//Urban‚Ì‘I‘ð
	if (MouseL.down())
	{
		ui.selectedUrbanID = -1;

		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);

			for (auto j : step(int(urbans.size())))
			{
				if (urbans[j].mouseOver())
				{
					ui.selectedVehicleID = -1;
					ui.selectedUrbanID = j;
					sounds.at(0).play();
				}
			}
		}
	}

	//Vehicle‚Ì‘I‘ð
	if (MouseL.down())
	{
		if (!Rect(480, Window::Size().y).mouseOver() || ui.selectedVehicleID == -1)
		{
			ui.selectedVehicleID = -1;
			if (ui.selectedUrbanID == -1)
			{
				for (int i = 0; i < 2; ++i) {
					const auto transformer = tinyCamera.createTransformer(i);

					for (auto j : step(int(vehicles.size())))
					{
						if (vehicles[j].mouseOver())
						{
							ui.selectedVehicleID = j;
							sounds.at(0).play();
							ui.newChain = vehicles[j].chain;
						}
					}
				}
			}
		}
	}
}