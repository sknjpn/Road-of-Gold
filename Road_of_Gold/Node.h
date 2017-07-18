#pragma once
#include "Pos.h"

enum class Biome
{
	Ice,
	Tundra,
	Grassland,
	Desert,
	Savanna,
	SeasonalForest,
	TropicalRainforest,
	TemperateRainforest,
	BorealForest,
	Woodland,

	Sea,
	Ocean,
};

const Array<String> BiomeName = {
	L"Ice",
	L"Tundra",
	L"Grassland",
	L"Desert",
	L"Savanna",
	L"SeasonalForest",
	L"TropicalRainforest",
	L"TemperateRainforest",
	L"BorealForest",
	L"Woodland",
	
	L"Sea",
	L"Ocean",
};

struct Node;
struct Region;

struct Path
{
	Path::Path(int _parentNodeID, int _childNodeID);
	Node&	getChild() const;
	Node&	getParent() const;
	Line	getLine() const;

	int		id;
	double	len;
	int		parentNodeID;
	int		childNodeID;

};
extern Array<Path*> paths;

struct Node
{
	Node::Node(int _id, const Pos& _pos);
	bool	isCoast() const;
	void	draw(const Color& _color) const;
	Region&	getJoinedRegion() const;

	int		temperatureLevel;
	int		moistureLevel;
	Biome	biome;

	int		id;
	int		joinedRegionID;
	int		ownUrbanID;
	bool	isSea;
	Pos		pos;
	bool	isScaned, isInQueue;
	double	cost;
	int		fromNodeID;
	Color	color;
	Array<Path> paths;
};
extern Array<Node> nodes;
bool	loadNodeMap();
void	setPlanetToNodes();

struct Region
{
	Region(int _id);

	Color	color;
	int		id;
	int		numNodes;
	bool	hasCity;
};
extern Array<Region> regions;
