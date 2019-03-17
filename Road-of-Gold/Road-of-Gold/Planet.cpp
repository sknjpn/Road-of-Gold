#include"Planet.h"
#include"Scuttle.h"
#include"Urban.h"
#include"EnergyData.h"
#include"ItemData.h"
#include<lua.hpp>
#include<lauxlib.h>
#include<lualib.h>

Planet::Planet()
	: mapTexture()
	, timeSpeed(0)
	, sandglass()
{
	incidentsLua = luaL_newstate();
	luaL_openlibs(incidentsLua);


	//日付の取得
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		lua_pushinteger(l, planet.sandglass.year());
		lua_pushinteger(l, planet.sandglass.month());
		lua_pushinteger(l, planet.sandglass.day());
		return 3;
	});
	lua_setglobal(incidentsLua, "getDate");

	//Scuttle追加関数
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		const auto& title = Unicode::FromUTF8(lua_tostring(l, 1));
		const auto& document = Unicode::FromUTF8(lua_tostring(l, 2));
		const auto& button = Unicode::FromUTF8(lua_tostring(l, 3));
		scuttles.emplace_back(title, document, button);
		return 0;
	});
	lua_setglobal(incidentsLua, "addScuttle");

	//Print
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		switch (lua_type(l, 1))
		{
		case LUA_TNIL:
			Print << U"NIU";
			break;
		case LUA_TBOOLEAN:
			Print << U"BOOLEAN :" << (lua_toboolean(l, 1) == 1 ? U"true" : U"false");
			break;
		case LUA_TLIGHTUSERDATA:
			Print << U"LIGHTUSERDATA";
			break;
		case LUA_TNUMBER:
			Print << U"NUMBER :" << int(lua_tonumber(l, 1));
			break;
		case LUA_TSTRING:
			Print << U"STRING :" << Unicode::FromUTF8(lua_tostring(l, 1));
			break;
		case LUA_TTABLE:
			Print << U"TABLE";
			break;
		case LUA_TFUNCTION:
			Print << U"FUNCTION";
			break;
		case LUA_TUSERDATA:
			Print << U"USERDATA";
			break;
		case LUA_TTHREAD:
			Print << U"THREAD";
			break;
		}
		return 0;
	});
	lua_setglobal(incidentsLua, "print");

	//clearPrint
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		ClearPrint();
		l;	//警告避け
		return 0;
	});
	lua_setglobal(incidentsLua, "clearPrint");

	//Logger
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		switch (lua_type(l, 1))
		{
		case LUA_TNIL:
			Logger << U"NIU";
			break;
		case LUA_TBOOLEAN:
			Logger << U"BOOLEAN :" << (lua_toboolean(l, 1) == 1 ? U"true" : U"false");
			break;
		case LUA_TLIGHTUSERDATA:
			Logger << U"LIGHTUSERDATA";
			break;
		case LUA_TNUMBER:
			Logger << U"NUMBER :" << int(lua_tonumber(l, 1));
			break;
		case LUA_TSTRING:
			Logger << U"STRING :" << Unicode::FromUTF8(lua_tostring(l, 1));
			break;
		case LUA_TTABLE:
			Logger << U"TABLE";
			break;
		case LUA_TFUNCTION:
			Logger << U"FUNCTION";
			break;
		case LUA_TUSERDATA:
			Logger << U"USERDATA";
			break;
		case LUA_TTHREAD:
			Logger << U"THREAD";
			break;
		}
		return 0;
	});
	lua_setglobal(incidentsLua, "Logger");


	//マップディレクトリを渡す
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		lua_pushstring(l, planet.mapPath.narrow().c_str());
		return 1;
	});
	lua_setglobal(incidentsLua, "getMapPath");

	//音の再生
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		planet.audios.emplace_back(Unicode::FromUTF8(lua_tostring(l, 1)));
		planet.audios.back().play();
		lua_pushboolean(l, planet.audios.back().isPlaying());
		return 1;
	});
	lua_setglobal(incidentsLua, "playAudio");

	//BGMの設定
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		planet.bgm = Audio(Unicode::FromUTF8(lua_tostring(l, 1)));
		planet.bgm.setLoop(true);
		planet.bgm.setVolume(lua_isnumber(l, 2) ? lua_tonumber(l, 2) : 1.0);
		planet.bgm.play();
		lua_pushboolean(l, planet.bgm.isPlaying());
		return 1;
	});
	lua_setglobal(incidentsLua, "setBGM");

	//getNumEnergy(energyType,urban)
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		auto* urban = getUrban(Unicode::FromUTF8(lua_tostring(l, 2)));
		if (urban == nullptr) lua_pushinteger(l, 0);
		else
		{
			int energyType = getEnergyType(Unicode::FromUTF8(lua_tostring(l, 1)));
			if (energyType == -1) lua_pushinteger(l, 0);
			else
			{
				for (auto& e : urban->energies)
				{
					if (e.energyType == energyType)
					{
						lua_pushinteger(l, e.numEnergy);
						return 1;
					}
				}
				lua_pushinteger(l, 0);
			}
		}
		return 1;
	});
	lua_setglobal(incidentsLua, "getNumEnergy");

	//setEnergy(energyType,urban,num)
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		auto* urban = getUrban(Unicode::FromUTF8(lua_tostring(l, 2)));
		if (urban == nullptr) lua_pushboolean(l, false);
		else
		{
			int energyType = getEnergyType(Unicode::FromUTF8(lua_tostring(l, 1)));
			if (energyType == -1) lua_pushboolean(l, false);
			else
			{
				urban->energies.remove_if([&energyType](Energy& e) { return e.energyType == energyType; });
				urban->energies.emplace_back(energyType, int(lua_tointeger(l, 3)));
				lua_pushboolean(l, true);
			}
		}
		return 1;
	});
	lua_setglobal(incidentsLua, "setEnergy");
	
	//getNumProductionPerDay(itemType,urban)
	lua_pushcfunction(incidentsLua, [](lua_State* l) {
		auto* urban = getUrban(Unicode::FromUTF8(lua_tostring(l, 2)));
		if (urban == nullptr) lua_pushinteger(l, 0);
		else
		{
			int itemType = getItemType(Unicode::FromUTF8(lua_tostring(l, 1)));
			if (itemType == -1) lua_pushinteger(l, 0);
			else lua_pushinteger(l, urban->shelves[itemType].tradeLog.numProduction[1]);
		}
		return 1;
	});
	lua_setglobal(incidentsLua, "getNumProductionPerDay");


}