#include"Display.h"
#include"Urban.h"
#include"TinyCamera.h"
#include"Sound.h"
#include"Fleet.h"

RectF	Selecter::selectedRegion() const
{
	if (!selectedPos) return RectF();
	const auto& p1 = selectedPos.value();
	const auto& p2 = Cursor::PosF();

	return RectF(Min(p1.x, p2.x), Min(p1.y, p2.y), Abs(p1.x - p2.x), Abs(p1.y - p2.y));
}
void	Selecter::draw() const
{
	if (selectedPos)
	{
		const auto& color = Palette::White;
		const double thickness = 2;

		selectedRegion().drawFrame(thickness, color);
	}
}
void	Selecter::update()
{
	//ウィンドウ上の場合は機能させない
	if (Rect(0, 0, 480, 720).mouseOver() && (!selectedFleets.isEmpty() || selectedUrban != nullptr)) return;

	//RotueMaker使用時は機能させない
	if (routeMaker.targetFleet != nullptr) return;

	if (MouseL.down())
	{
		//Urban
		bool flag = false;
		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				if (u.mouseOver())
				{
					u.isSelected = true;
					flag = true;
				}
			}
		}

		//もし、Urbanを選択してなかったら
		if (!flag) selectedPos = Cursor::PosF();
	}
	if (MouseL.up())
	{
		//Urban
		Urban*	newSelectedUrban = nullptr;
		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				if (u.mouseOver() && u.isSelected) newSelectedUrban = &u;
			}
		}
		for (auto& u : urbans) u.isSelected = false;
		if (newSelectedUrban != selectedUrban)
		{
			selectedUrban = newSelectedUrban;
			if (newSelectedUrban != nullptr) displayUrban.openElapsedTime.restart();
			else displayUrban.closeElapsedTime.restart();
		}

		if (selectedPos)
		{

			//Fleet
			if (selectedUrban == nullptr)
			{
				Array<Fleet*>	newSelectedFleets;
				for (int i = 0; i < 2; ++i) {
					for (auto& f : fleets)
					{
						if (f.isInSelectedRegion() && !newSelectedFleets.include(&f)) newSelectedFleets.emplace_back(&f);
					}
				}
				if (selectedFleets != newSelectedFleets)
				{
					if (newSelectedFleets.isEmpty()) displayFleets.closeElapsedTime.restart();
					else displayFleets.openElapsedTime.restart();
				}
				selectedFleets = newSelectedFleets;
			}
			else if (!selectedFleets.isEmpty())
			{
				displayFleets.closeElapsedTime.restart();
				selectedFleets.clear();
			}

			//selectedPos
			selectedPos = none;
		}
		else if (!selectedFleets.isEmpty())
		{
			displayFleets.closeElapsedTime.restart();
			selectedFleets.clear();
		}
	}
	/*
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

		//Fleetの選択
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
	*/
}

/*
void	Display::updateSelectItem()
{	//Urbanの選択
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

		//Fleetの選択
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
*/