#include"TinyCamera.h"
#include"Fleet.h"
#include"ItemData.h"
#include"VehicleData.h"
#include"Vehicle.h"

void	drawFleets()
{
	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		for (const auto& f : fleets)
		{
			auto pos = f.pos();
			//auto color = f.cargo.isEmpty() ? Color(0, 0) : vehicleData.front().color;
			//Circle(pos, 0.005).drawFrame(0.001, 0.000, Palette::Black);

			f.ownVehicles.front()->data().icon.resize(0.005).rotate(f.angle()).drawAt(pos);

			/*
			if (!f.cargo.isEmpty())
			{
				auto& data = f.cargo.data();

				data.icon.resize(0.01).drawAt(pos);
			}
			*/
		}
	}
}