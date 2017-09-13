#include"Scuttle.h"

Scuttle::Scuttle(String _title, String _description, String _button)
	: isGrabbed(false)
	, title(_title)
	, description(_description)
	, button(_button)
	, backGround(L"assets/image/scuttle.png")
{
	rect = RectF(640, 480).setCenter(Window::Center());
	titleRegion = RectF(480, 48).setCenter(320, 40);
	descriptionRegion = RectF(560, 320).setCenter(320, 240);
	buttonRegion = RectF(480, 32).setCenter(320, 440);
}