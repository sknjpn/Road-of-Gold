#pragma once

/*
2つの都市間のルートを表す。
pathIDsを順番にたどれば目的地に着く。
*/

struct Urban;

struct Route
{
	Route(int _id);
	Urban&	getDestinationUrban() const;
	Urban&	getOriginUrban() const;
	void	draw(const Color& _color) const;

	int		id;
	int		destinationUrbanID;
	int		originUrbanID;
	Array<int>	pathIDs;
	double	totalLength;
	bool	isSeaRoute;
};
extern Array<Route> routes;
void	makeRoute();