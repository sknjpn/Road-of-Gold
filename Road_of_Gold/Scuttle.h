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
	Audio	audio;
	Texture	backGround;

	Scuttle(String _title, String _description, String _button)
		: isGrabbed(false)
		, title(_title)
		, description(_description)
		, button(_button)
		, backGround(L"Assets/Image/Scuttle.png")
	{
		rect = RectF(640, 480).setCenter(Window::Center());
		titleRegion = RectF(480, 48).setCenter(320, 40);
		descriptionRegion = RectF(560, 320).setCenter(320, 240);
		buttonRegion = RectF(480, 32).setCenter(320, 440);
	}
	void	draw() const;
};

extern Array<Scuttle>	scuttles;

void	updateScuttles();
void	drawScuttles();