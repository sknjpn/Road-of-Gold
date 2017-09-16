#include"Planet.h"
#include"Incident.h"

void	updatePlanet()
{

	tinyCamera.update();
	
	//更新速度変更
	if (KeyF1.down()) planet.timeSpeed = 0;
	if (KeyF2.down() || (KeyF2.pressedDuration().count() > 250 && System::FrameCount() % 6 == 0))
	{
		planet.timeSpeed = planet.timeSpeed / 2;
		if (planet.timeSpeed < pow(0.5, 12)) planet.timeSpeed = 0;
	}
	if (KeyF3.down() || (KeyF3.pressedDuration().count() > 250 && System::FrameCount() % 6 == 0))
	{
		planet.timeSpeed = Min(1.0, planet.timeSpeed * 2);
		if (planet.timeSpeed == 0) planet.timeSpeed = pow(0.5, 12);
	}
	if (KeyF4.down()) planet.timeSpeed = 1;

	if (planet.timeSpeed > 0)
	{
		if (planet.sandglass.update())
		{
			for (auto& i : incidents) i.update();
		}
	}
}