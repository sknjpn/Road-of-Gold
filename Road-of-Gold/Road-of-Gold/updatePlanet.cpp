#include"Planet.h"

#include<lua.hpp>

void	updatePlanet()
{
	planet.sandglass.update();

	//Lua
	lua_getglobal(planet.incidentsLua, "update");
	lua_pcall(planet.incidentsLua, 0, 0, 0);
}