#pragma once

struct Pos;

struct Urban
{
	int		id;
	String	name;
	int		joinedNodeID;
	Array<int>		resource;

	Urban(int _joinedNodeID);
	void	update();
	void	draw() const;
	Pos&	getPos() const;
	String	getTimeAsString() const;

};