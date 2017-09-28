#pragma once

struct Path;
struct Node;

struct River
{
	double	width;
	Node*	lastNode;
	Array<Path*>	riverPaths;
	bool	isMouseOver;

	River(double _width, Node* _lastNode)
		: width(_width)
		, lastNode(_lastNode)
		, isMouseOver(false)
	{}
	River(const JSONValue _json);
	void	draw() const;
};
extern Array<River> rivers;