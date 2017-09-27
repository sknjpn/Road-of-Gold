#include"Display.h"
#include"Planet.h"
#include"TinyCamera.h"

void	updateDisplay()
{
	selecter.update();
	selecter.draw();
	displayUrban.update();
	displayFleets.update();
	routeMaker.update();

	const auto fColor = Color(20);
	const auto bColor = Color(60);
	double thickness = 4;
	//タイムスケールの変更
	{
		const auto& font = *globalFonts[24];
		const Rect rect(Window::Size().x - 300, 36, 300, 36);
		rect.draw(bColor).drawFrame(thickness, fColor);
		font(L"速度").draw(rect.pos);
		const auto t1 = Triangle(24).rotated(30_deg).movedBy(rect.pos + Vec2(64, 18));
		const auto t2 = Triangle(24).rotated(90_deg).movedBy(rect.pos + Vec2(284, 18));
		const auto c1 = Circle(rect.pos + Vec2(64, 18), 14);
		const auto c2 = Circle(rect.pos + Vec2(284, 18), 14);
		c1.draw(Color(100));
		c2.draw(Color(100));
		t1.draw(c1.mouseOver() ? Color(200) : Color(60));
		t2.draw(c2.mouseOver() ? Color(200) : Color(60));
		if (c1.leftClicked())
		{
			planet.timeSpeed = planet.timeSpeed / 2;
			if (planet.timeSpeed < pow(0.5, 12)) planet.timeSpeed = 0;
		}
		if (c2.leftClicked())
		{
			planet.timeSpeed = Min(1.0, planet.timeSpeed * 2);
			if (planet.timeSpeed == 0) planet.timeSpeed = pow(0.5, 12);
		}
		Rect(rect.pos + Point(82, 10), Point(182, 16)).draw(Palette::Red);
		if (planet.timeSpeed > 0) Rect(rect.pos + Point(82, 10), Point(182 - int(14 * log2(1 / planet.timeSpeed)), 16)).draw(Palette::Green);
		for (int i = 0; i < 13; i++) Rect(rect.pos + Point(82 + 14 * i, 10), Point(14, 16)).drawFrame(2);
	}
	//グローバル時間の表示
	{
		const Point pos(Window::Size().x - 300, 0);
		const auto& s = planet.sandglass;
		const auto& font = *globalFonts[24];

		//年表示
		{
			const auto rect = Rect(0, 0, 84, 36).movedBy(pos);
			const String text = Format(int(s.timer / 360), L"年");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(thickness, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0));
		}

		//月表示
		{
			const auto rect = Rect(84, 0, 54, 36).movedBy(pos);
			const String text = Format(int(s.timer / 30) % 12 + 1, L"月");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(thickness, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0));
		}

		//日表示
		{
			const auto rect = Rect(138, 0, 54, 36).movedBy(pos);
			const String text = Format(int(s.timer) % 30 + 1, L"日");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(thickness, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0));
		}

		//時表示
		{
			const auto rect = Rect(192, 0, 54, 36).movedBy(pos);
			const String text = Format(int(s.timer * 24) % 24, L"時");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(thickness, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0));
		}

		//分表示
		{
			const auto rect = Rect(246, 0, 54, 36).movedBy(pos);
			const String text = Format(int(s.timer * 24 * 60) % 60, L"分");
			const int width = (int)font(text).region().size.x;

			rect.draw(bColor).drawFrame(thickness, fColor);
			font(text).draw(rect.tr().movedBy(-width, 0));
		}
	}
}