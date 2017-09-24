#include"TinyCamera.h"
#include"Fleet.h"
#include"ItemData.h"
#include"VehicleData.h"
#include"Vehicle.h"
#include"UI.h"

void	drawFleets()
{
	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		for (const auto& f : fleets)
		{
			auto pos = f.pos();

			f.ownVehicles.front()->data().icon.resize(0.005).rotate(f.angle()).drawAt(pos);
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		double r = 5000.0;
		const auto transformer = tinyCamera.createTransformer(i,r);

		for (auto& f : fleets)
		{
			auto pos = f.pos();
			RectF rect(pos*r, Size(160, 24));
			
			if (ui.selectedFleets.include(&f)) rect.draw(Palette::Lightgreen).drawFrame(4, Palette::White);
			else rect.draw(Palette::Darkgreen).drawFrame(4, Palette::Green);

			(*ui.fonts[16])(f.name).draw(rect.pos.movedBy(4, 1));
		}
	}
}