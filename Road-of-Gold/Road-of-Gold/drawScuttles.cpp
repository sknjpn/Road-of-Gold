#include"Scuttle.h"
#include"Display.h"

void	drawScuttles()
{
	for (auto& s : scuttles)
	{
		const auto buttonPos = s.buttonRegion.movedBy(s.rect.pos).center();
		const auto titlePos = s.titleRegion.movedBy(s.rect.pos).center();
		const auto descriptionPos = s.descriptionRegion.movedBy(s.rect.pos).center();

		s.backGround.draw(s.rect.pos);
		s.rect.drawFrame(4, Palette::Black);
		s.buttonRegion.movedBy(s.rect.pos).draw(Palette::Forestgreen).drawFrame(4, Palette::Black);

		(*globalFonts[32])(s.title).drawAt(titlePos, Palette::Black);
		(*globalFonts[24])(s.button).drawAt(buttonPos, Palette::Black);
		(*globalFonts[16])(s.description).drawAt(descriptionPos, Palette::Black);
	}
}