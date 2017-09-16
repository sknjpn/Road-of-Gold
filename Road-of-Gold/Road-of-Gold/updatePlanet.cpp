#include"Planet.h"
#include"Incident.h"

void	updatePlanet()
{
	if (planet.sandglass.update())
	{
		for (auto& i : incidents) i.update();
	}
}