#pragma once
#include"Pos.h"


//自然環境の生成とマップ上の視点移動を行う
struct Planet
{
	Grid<int>	voronoiMap;
	Texture		mapTexture;

	Planet();
	void	setRegions();
	void	makeGroupsRandom();
	bool	loadNodeMap();
	bool	loadBiome();
	bool	loadVoronoiMap();
};

struct TinyCamera2D
{
	TinyCamera2D();
	void	update();
	void	draw() const;
	Pos		getCursorPos() const;
	Mat3x2	getMat3x2(int _delta = 0) const;
	Transformer2D createTransformer(int _delta = 0) const;

	RectF	restrictedRegion;
	RectF	drawingRegion;
	RectF	smoothDrawingRegion;
	Optional<Pos>	gazePoint;
};

extern Planet planet;
extern TinyCamera2D tinyCamera2D;