#pragma once

#include"TinyCamera.h"
#include"Sandglass.h"

struct Planet
{
	Sandglass	sandglass;
	Texture	mapTexture;
	double	timeSpeed;

	Planet()
		: mapTexture()
		, timeSpeed(0)
		, sandglass(1492 * 360)
	{}
};

extern Planet	planet;

void	loadMap();
void	initPlanet();
void	updatePlanet();
void	drawPlanet();