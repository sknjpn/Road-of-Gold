#pragma once
#include "Pos.h"

struct Node;

//自然環境の生成とマップ上の視点移動を行う
struct Planet
{
	Planet();
	void	updateTransform();
	void	updateViewPointSliding();
	bool	loadVoronoiMap();
	void	updateImage(Array<Node*> _nodeList);
	Pos		getCursorPos() const;
	Transformer2D createTransformer(int _delta = 0) const;

	Grid<int>	voronoiMap;
	RectF	drawingRegion;
	RectF	smoothDrawingRegion;
	DynamicTexture mapTexture;
	Texture outlineTexture;
	Image	image;
	Optional<Pos>	gazePoint;
};
extern Planet planet;

extern int		selectedBiome;
extern int		selectedBrush;