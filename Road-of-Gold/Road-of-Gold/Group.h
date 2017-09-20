#pragma once

struct Wallet;

struct Group
{
	String	name;
	int		walletID;
	Array<int>	ownVehicleIDs;

	Group();
	Group(const JSONValue& _json);
};

extern Array<Group>	groups;

void	updateGroups();
void	initGroups();