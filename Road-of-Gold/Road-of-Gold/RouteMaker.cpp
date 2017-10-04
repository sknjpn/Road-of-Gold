#include"Urban.h"
#include"Display.h"
#include"Fleet.h"
#include"TinyCamera.h"
#include"ItemData.h"
#include"Route.h"

void	RouteMaker::update()
{
	if (targetFleet == nullptr) return;

	{
		Window::ClientRect().drawFrame(16, Color(Palette::Red, 128));
		Rect rect(Window::ClientRect().size.x, 48);
		rect.draw(Color(Palette::Red, 128));
		(*globalFonts[32])(L"ルート作成モード").drawAt(rect.center());
	}

	//erase
	if (MouseR.down())
	{
		wayPoints.pop_back();
		if (wayPoints.isEmpty())
		{
			selecter.selectedFleets.emplace_back(targetFleet);
			displayFleets.openElapsedTime.restart();
			targetFleet = nullptr;
			return;
		}
	}

	//add
	if (MouseL.down())
	{
		for (int i = 0; i < 2; i++)
		{
			auto t1 = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				if (u.mouseOver()) u.isSelected = true;
			}
		}
	}
	if (MouseL.up())
	{
		Urban* targetUrban = nullptr;
		for (int i = 0; i < 2; i++)
		{
			auto t1 = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				if (targetFleet->canMoveTo(u) && u.mouseOver() && u.isSelected && wayPoints.back().urban != &u) targetUrban = &u;
			}
		}
		for (auto& u : urbans) u.isSelected = false;
		if (targetUrban != nullptr)
		{
			if (wayPoints.include_if([&targetUrban](const WayPoint& wp) { return wp.urban == targetUrban; }))
			{
				wayPoints.emplace_back(targetUrban);
				//chain追加
				{
					auto* tf = targetFleet;
					tf->chain.clear();
					tf->chain.rings.emplace_back(Code::Move, wayPoints[0].urban->id());
					for (int i = 0; i < int(wayPoints.size() - 1); i++)
					{
						tf->chain.rings.emplace_back(Code::Sell, 0);
						if (wayPoints[i].buyItemType) tf->chain.rings.emplace_back(Code::Buy, wayPoints[i].buyItemType.value());
						tf->addMoveTo(wayPoints[i].urban, wayPoints[i + 1].urban);
					}
					for (int i = 0; i < int(tf->chain.rings.size() - 1); i++)
					{
						auto& ring = tf->chain.rings[i];
						if (ring.code == Code::Move && ring.value == wayPoints.back().urban->id())
						{
							tf->chain.rings.emplace_back(Code::Jump, i + 1);
							break;
						}
					}
				}
				wayPoints.clear();
				selecter.selectedFleets.emplace_back(targetFleet);
				displayFleets.openElapsedTime.restart();
				targetFleet = nullptr;
				return;
			}
			else wayPoints.emplace_back(targetUrban);
		}
	}

	//way
	for (int i = 0; i < 2; i++)
	{
		auto t1 = tinyCamera.createTransformer(i);
		for (int j = 0; j < int(wayPoints.size() - 1); j++)
		{
			auto& wp1 = wayPoints[j];
			auto& wp2 = wayPoints[j + 1];
			Line(wp1.urban->pos().mPos, wp2.urban->pos().mPos).drawArrow(0.01, Vec2(0.05, 0.05), Color(Palette::Red, 192));
		}
	}

	//newWay
	{
		Urban* targetUrban = nullptr;
		for (int i = 0; i < 2; i++)
		{
			auto t1 = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				if (u.mouseOver()) targetUrban = &u;
			}
		}
		if (targetUrban != nullptr)
		{
			if (wayPoints.back().urban != targetUrban && wayPoints.include_if([&targetUrban](const WayPoint& wp) { return wp.urban == targetUrban; }))
			{
				for (int i = 0; i < 2; i++)
				{
					auto t1 = tinyCamera.createTransformer(i);

					bool flag = false;
					for (int j = 0; j < int(wayPoints.size() - 1); j++)
					{
						auto& wp1 = wayPoints[j];
						auto& wp2 = wayPoints[j + 1];
						if (wp1.urban == targetUrban || flag)
						{
							flag = true;
							Line(wp1.urban->pos().mPos, wp2.urban->pos().mPos).drawArrow(0.01, Vec2(0.05, 0.05), Color(Palette::Skyblue, 192));
						}
					}
					Line(wayPoints.back().urban->pos().mPos, targetUrban->pos().mPos).drawArrow(0.01, Vec2(0.05, 0.05), Color(Palette::Skyblue, 128));
				}
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					auto t1 = tinyCamera.createTransformer(i);
					Line(wayPoints.back().urban->pos().mPos, targetUrban->pos().mPos).drawArrow(0.01, Vec2(0.05, 0.05), Color(Palette::Red, 128));
					if (!targetFleet->canMoveTo(*targetUrban))
					{
						auto p = (wayPoints.back().urban->pos().mPos + targetUrban->pos().mPos) / 2.0;
						Line(p.movedBy(-0.05, -0.05), p.movedBy(0.05, 0.05)).draw(0.02, Palette::Red);
						Line(p.movedBy(0.05, -0.05), p.movedBy(-0.05, 0.05)).draw(0.02, Palette::Red);
					}
				}
			}
		}
	}


	//ui
	for (int i = 0; i < 2; i++)
	{
		double r = 2000.0;
		auto t1 = tinyCamera.createTransformer(i, r);
		for (auto& wp : wayPoints)
		{
			RectF(wp.urban->pos().mPos*r, Vec2(240.0, 240.0)).draw(Color(60)).drawFrame(4.0, Color(80));
			for (int j = 0; j < int(itemData.size()); j++)
			{
				RectF rect(wp.urban->pos().mPos*r + Vec2(0, 24 * j), Vec2(240.0, 24.0));
				if (wp.buyItemType && j == wp.buyItemType.value()) rect.draw(Color(160));
				if (rect.mouseOver()) rect.draw(Color(120));
				if (rect.leftClicked())
				{
					if (wp.buyItemType == j) wp.buyItemType = none;
					else wp.buyItemType = j;
				}
				rect.drawFrame(4.0, Color(80));

				(*globalFonts[16])(itemData[j].name).draw(rect.pos.movedBy(4, 0));
			}
		}
	}
}
void	RouteMaker::set(Fleet* _targetFleet)
{
	targetFleet = _targetFleet;
	wayPoints.emplace_back(targetFleet->route != nullptr ? targetFleet->route->toUrban : targetFleet->nowUrban);
}