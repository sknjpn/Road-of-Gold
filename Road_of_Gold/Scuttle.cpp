#include"Scuttle.h"
#include"UI.h"

void	Scuttle::draw() const
{
	backGround.draw(rect.pos);
	rect.drawFrame(4, Palette::Black);
	buttonRegion.movedBy(rect.pos).draw(Palette::Forestgreen).drawFrame(4, Palette::Black);
	//titleRegion.movedBy(rect.pos).draw(Palette::Blue).drawFrame(4, Palette::Black);
	//descriptionRegion.movedBy(rect.pos).draw(Palette::Red).drawFrame(4, Palette::Black);
	auto bPos = buttonRegion.movedBy(rect.pos).center();
	auto tPos = titleRegion.movedBy(rect.pos).center();
	auto dPos = descriptionRegion.movedBy(rect.pos).center();
	(*ui.fonts[32])(title).drawAt(tPos, Palette::Black);
	(*ui.fonts[24])(button).drawAt(bPos, Palette::Black);
	(*ui.fonts[16])(description).drawAt(dPos, Palette::Black);
}

void	updateScuttles()
{
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
			}
		}
	}
}
void	drawScuttles()
{
	for (auto& s : scuttles) s.draw();
}