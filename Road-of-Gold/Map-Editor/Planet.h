#pragma once
#include "Pos.h"

struct Node;

//自然環境の生成とマップ上の視点移動を行う
struct Planet
{
	Grid<int>	voronoiMap;
	DynamicTexture mapTexture;
	Texture outlineTexture;
	Image	image;

	Planet();
	bool	loadVoronoiMap();
	void	updateImage(Array<Node*> _nodeList);
	void	generateBiome();
};

extern Planet planet;

extern int		selectedBiome;
extern int		selectedBrush;