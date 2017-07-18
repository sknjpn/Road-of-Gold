#pragma once

/*
2つの都市間のルートを表す。
pathIDsを順番にたどれば目的地に着く。
*/

struct Urban;

struct Route
{
	int		id;
	int		destinationUrbanID;
	int		originUrbanID;
	double	totalLength;
	bool	isSeaRoute;
	Array<int>	pathIDs;

	Route(int _id);
	Urban&	getDestinationUrban() const;
	Urban&	getOriginUrban() const;
	void	draw(const Color& _color) const;
};
extern Array<Route> routes;
void	makeRoute();