#pragma once

struct Pos;

struct Urban
{
	int		id;
	int		numCitizens;
	String	name;
	int		joinedNodeID;
	Array<int>		resource;

	Urban();
	Urban(const JSONValue _json);
	Urban(int _joinedNodeID);
	void	update();
	void	draw() const;
	Pos&	getPos() const;
	String	getTimeAsString() const;

};
extern Array<Urban> urbans;