#pragma once

struct Wallet;

struct Group
{
	int		walletID;
	Array<int>	ownVehicleIDs;

	Group();
};

extern Array<Group>	groups;

void	updateGroups();
void	drawGroups();
void	initGroups();