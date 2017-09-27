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
}