#pragma once

struct Urban;

struct Nation
{
	String	name;
	Array<Urban*>	ownUrbans;

	Nation(const JSONValue& _json);
};

extern Array<Nation>	nations;