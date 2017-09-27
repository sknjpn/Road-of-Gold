#pragma once

struct Path;
struct Node;

struct River
{
	double	width;
	Node*	lastNode;
	Array<Path*>	riverPaths;

	River(double _width, Node* _lastNode)
		: width(_width)
		, lastNode(_lastNode)
	{}
	void	draw() const;
};
extern Array<River> rivers;