#pragma once
#include "Pos.h"


//自然環境の生成とマップ上の視点移動を行う
struct Planet
{
	Planet();
	void	setRegions();
	void	updateTransform();
	void	updateViewPointSliding();
	bool	loadBiome();
	bool	loadVoronoiMap();
	Pos		getCursorPos() const;
	Transformer2D createTransformer(int _delta = 0) const;

	Grid<int>	voronoiMap;
	RectF	drawingRegion;
	RectF	smoothDrawingRegion;
	Texture mapTexture;
	Optional<Pos>	gazePoint;
};
extern Planet planet;