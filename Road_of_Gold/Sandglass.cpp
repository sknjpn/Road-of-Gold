#include"Sandglass.h"
#include"Planet.h"

Sandglass::Sandglass(double _timer)
	: timer(_timer)
{}

bool	Sandglass::update()
{
	timer += planet.timeSpeed;
	if (int(timer) != int(timer - planet.timeSpeed)) return true;

	return false;
}