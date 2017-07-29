#pragma once

/*
2つの都市間のルートを表す。
pathIDsを順番にたどれば目的地に着く。
*/

struct Urban;

struct Route
{
	int		id;
	int		destinationUrbanID;	//目的地の都市
	int		originUrbanID;		//出発地の都市
	double	totalCost;			//移動コスト
	bool	isSeaRoute;			//海上ルートか否か
	Array<int>	pathIDs;		//経由するパス

	Route(int _id);
	Urban&	getDestinationUrban() const;
	Urban&	getOriginUrban() const;
	void	draw(const Color& _color) const;
};
extern Array<Route> routes;
void	makeRoute();