#pragma once
#include "Pos.h"

struct Node;

//自然環境の生成とマップ上の視点移動を行う
struct Planet
{
	Planet();
	bool	loadVoronoiMap();
	void	updateImage(Array<Node*> _nodeList);
	void	generateBiome();

	Grid<int>	voronoiMap;
	RectF	drawingRegion;
	RectF	smoothDrawingRegion;
	DynamicTexture mapTexture;
	Texture outlineTexture;
	Image	image;
	Optional<Pos>	gazePoint;
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

extern int		selectedBiome;
extern int		selectedBrush;