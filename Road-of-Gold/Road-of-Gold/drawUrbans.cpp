#include"Planet.h"
#include"Urban.h"
#include"Display.h"

void	drawUrbanIcon()
{
	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		for (const auto& u : urbans)
		{
			const auto color = (u.isSelected && u.mouseOver()) ? Palette::Darkorange : (!MouseL.pressed() && u.mouseOver()) ? Palette::Orange : Palette::Red;

			u.shape().draw(color).drawFrame(0.002, 0.000, Palette::Black);
		}
	}
}
void	drawUrbanName()
{
	for (int i = 0; i < 2; ++i)
	{
		double r = 2500.0;
		const auto transformer = tinyCamera.createTransformer(i, r);

		for (const auto& u : urbans)
		{
			for (double angle = 0; angle < 360_deg; angle += 10_deg)
			{
				(*globalFonts[128])(u.name).drawAt((u.pos().mPos*r).movedBy(0, -96).movedBy(Vec2(10, 0).rotated(angle)), Palette::White);
			}
			(*globalFonts[128])(u.name).drawAt((u.pos().mPos*r).movedBy(0, -96), Palette::Black);
		}
	}
}