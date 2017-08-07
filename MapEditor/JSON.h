#pragma once

bool loadJSONData();

struct BData
{
	String	name;
	Color	color;

	BData(const JSONValue _json);
};
extern Array<BData> bData;

struct RData
{
	String name;

	RData(const JSONValue _json);
};
extern Array<RData> rData;