#include"Scuttle.h"

void	updateScuttles()
{
	if (KeyEscape.down() && !scuttles.isEmpty()) scuttles.pop_back();

	if (MouseL.down())
	{
		Scuttle* target = nullptr;
		for (auto& s : scuttles)
		{
			s.isGrabbed = false;
			if (s.rect.mouseOver()) target = &s;
		}
		if (target != nullptr)
		{
			if (target->buttonRegion.movedBy(target->rect.pos).mouseOver())
			{
				//‘ÎÛScuttle‚Ìíœ
				scuttles.remove_if([target](const Scuttle& s) { return &s == target; });
			}
			else
			{
				target->isGrabbed = true;
			}
		}
	}
	else if (MouseL.up())
	{
		for (auto& s : scuttles) s.isGrabbed = false;
	}
	else if (MouseL.pressed())
	{
		for (auto& s : scuttles)
		{
			if (s.isGrabbed)
			{
				s.rect.pos += Cursor::DeltaF();
				if (s.rect.pos.x < 0) s.rect.pos.x = 0;
				if (s.rect.br().x > Window::Size().x) s.rect.pos.x = Window::Size().x - s.rect.size.x;
				if (s.rect.pos.y < 0) s.rect.pos.y = 0;
				if (s.rect.br().y > Window::Size().y) s.rect.pos.y = Window::Size().y - s.rect.size.y;
			}
		}
	}

	if (KeyEnter.down())
	{
		if (!scuttles.isEmpty()) scuttles.pop_back();
	}
}