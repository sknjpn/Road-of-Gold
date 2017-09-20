#include"UI.h"
#include"Urban.h"
#include"TinyCamera.h"
#include"Sound.h"
#include"Fleet.h"

void	selectItem()
{
	//Urban‚Ì‘I‘ð
	if (MouseL.down() && (!Rect(480, Window::Size().y).mouseOver() || (ui.selectedUrbanID == -1 && ui.selectedFleetID == -1)))
	{
		ui.selectedUrbanID = -1;

		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);

			for (auto j : step(int(urbans.size())))
			{
				if (urbans[j].mouseOver())
				{
					ui.selectedFleetID = -1;
					ui.selectedUrbanID = j;
					sounds.at(0).play();
				}
			}
		}

		//Fleet‚Ì‘I‘ð
		ui.selectedFleetID = -1;
		if (ui.selectedUrbanID == -1)
		{
			for (int i = 0; i < 2; ++i) {
				const auto transformer = tinyCamera.createTransformer(i);

				for (auto j : step(int(fleets.size())))
				{
					if (fleets[j].mouseOver())
					{
						ui.selectedFleetID = j;
						sounds.at(0).play();
						ui.newChain = fleets[j].chain;
					}
				}
			}
		}
	}

}