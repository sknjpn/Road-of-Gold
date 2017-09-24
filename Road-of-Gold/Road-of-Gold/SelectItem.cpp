#include"Display.h"
#include"Urban.h"
#include"TinyCamera.h"
#include"Sound.h"
#include"Fleet.h"

void	Display::updateSelectItem()
{
	//Urban‚Ì‘I‘ð
	if (MouseL.down() && (!Rect(480, Window::Size().y).mouseOver() || (selectedUrban == nullptr && selectedFleetID == -1)))
	{
		if (displayUrban.selectedUrban != nullptr)
		{
			displayUrban.selectedUrban = nullptr;
			displayUrban.closeElapsedTime.restart();
		}
		selectedUrban = nullptr;

		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);

			for (auto j : step(int(urbans.size())))
			{
				if (urbans[j].mouseOver())
				{
					selectedFleetID = -1;
					selectedUrban = &urbans[j];
					displayUrban.selectedUrban = &urbans[j];
					displayUrban.openElapsedTime.restart();
					elapsedTime.restart();
					sounds.at(0).play();
				}
			}
		}

		//Fleet‚Ì‘I‘ð
		selectedFleetID = -1;
		fleetNameTextBox.setActive(false);
		if (selectedUrban == nullptr)
		{
			for (int i = 0; i < 2; ++i) {
				const auto transformer = tinyCamera.createTransformer(i);

				for (auto j : step(int(fleets.size())))
				{
					if (fleets[j].mouseOver())
					{
						selectedFleetID = j;
						sounds.at(0).play();
						newChain = fleets[j].chain;
					}
				}
			}
		}
	}
}