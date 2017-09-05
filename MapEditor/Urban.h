#pragma once

struct Pos;

struct Urban
{
	int		numCitizens;
	int		joinedNodeID;
	String	name;
	double	productivity;
	Array<int>	energies;

	Urban();
	Urban(const JSONValue _json);
	Urban(int _joinedNodeID);
	Pos&	getPos() const;
	int		id() const;

};
extern Array<Urban> urbans;