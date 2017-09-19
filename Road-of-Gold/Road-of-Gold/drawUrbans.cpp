#include"Planet.h"
#include"Urban.h"
#include"UI.h"

void	drawUrbans()
{
	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		for (const auto& u : urbans)
		{
			const auto color = u.mouseOver() ? Palette::Orange : Palette::Red;

			u.shape().draw(color).drawFrame(0.002, 0.000, Palette::Black);
		}
	}
}