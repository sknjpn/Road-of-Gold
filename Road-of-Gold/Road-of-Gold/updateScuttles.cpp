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
			if (target->buttonRegion.movedBy(target->rect.pos).mouseOver()) target->isPushed = true;
			else target->isGrabbed = true;
		}
	}
	else if (MouseL.up())
	{
		//ëŒè€ScuttleÇÃçÌèú
		scuttles.remove_if([](const Scuttle& s) { return s.isPushed && s.buttonRegion.movedBy(s.rect.pos).mouseOver(); });
		for (auto& s : scuttles)
		{
			s.isGrabbed = false;
			s.isPushed = false;
		}
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