#pragma once

struct Scuttle
{
	bool	isGrabbed;
	RectF	rect;
	String	title;
	String	description;
	String	button;
	RectF	titleRegion;
	RectF	descriptionRegion;
	RectF	buttonRegion;
	Texture	backGround;
	bool	isPushed;

	Scuttle(String _title, String _description, String _button);
};

extern Array<Scuttle>	scuttles;

void	updateScuttles();
void	drawScuttles();