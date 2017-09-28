#pragma once

struct Path;
struct Node;

struct River
{
	double	width;
	Array<Node*>	riverNodes;

	River(const JSONValue _json);
	void	draw() const;
};
extern Array<River> rivers;