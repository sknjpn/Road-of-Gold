#pragma once

struct Node;

//自然環境の生成とマップ上の視点移動を行う
struct Planet
{
	DynamicTexture mapTexture;
	Texture outlineTexture;

	Planet();
	bool	loadVoronoiMap();
	void	generateBiome();
};

extern Planet planet;