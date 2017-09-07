#include"Scuttle.h"
#include"UI.h"

void	drawScuttles()
{
	for (auto& s : scuttles)
	{
		s.backGround.draw(s.rect.pos);
		s.rect.drawFrame(4, Palette::Black);
		s.buttonRegion.movedBy(s.rect.pos).draw(Palette::Forestgreen).drawFrame(4, Palette::Black);
		auto bPos = s.buttonRegion.movedBy(s.rect.pos).center();
		auto tPos = s.titleRegion.movedBy(s.rect.pos).center();
		auto dPos = s.descriptionRegion.movedBy(s.rect.pos).center();
		(*ui.fonts[32])(s.title).drawAt(tPos, Palette::Black);
		(*ui.fonts[24])(s.button).drawAt(bPos, Palette::Black);
		(*ui.fonts[16])(s.description).drawAt(dPos, Palette::Black);
	}
}