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

	RectF	outputRegion;		//画面描画先領域
	RectF	restrictedRegion;	//視点移動制限領域
	RectF	drawingRegion;		//仮想描画領域
	RectF	smoothDrawingRegion;//実描画領域
	Optional<Pos>	gazePoint;	//注視点
};

extern Planet planet;
extern TinyCamera2D tinyCamera2D;