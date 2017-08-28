#pragma once

struct Pos;

struct Urban
{
	int		numCitizens;
	double	productivity;
	String	name;
	int		joinedNodeID;
	Array<int>	energies;

	Urban();
	Urban(const JSONValue _json);
	Urban(int _joinedNodeID);
	void	update();
	void	draw() const;
	Pos&	getPos() const;
	String	getTimeAsString() const;
	int		id() const;

};
extern Array<Urban> urbans;