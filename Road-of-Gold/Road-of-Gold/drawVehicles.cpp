#include"TinyCamera.h"
#include"Vehicle.h"
#include"ItemData.h"
#include"VehicleData.h"
void	drawVehicles()
{
	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		for (const auto& v : vehicles)
		{
			auto pos = v.pos();
			auto color = v.cargo.isEmpty() ? Color(0, 0) : v.cargo.data().color;
			//Circle(pos, 0.005).drawFrame(0.001, 0.000, Palette::Black);

			v.data().icon.resize(0.005).rotate(v.angle()).drawAt(pos);

			/*
			if (!v.cargo.isEmpty())
			{
				auto& data = v.cargo.data();

				data.icon.resize(0.01).drawAt(pos);
			}
			*/
		}
	}
}