#include"TinyCamera.h"
#include"Fleet.h"
#include"ItemData.h"
#include"VehicleData.h"
#include"Vehicle.h"
#include"Display.h"
#include"Urban.h"
#include"Route.h"

void	drawFleets()
{
	for (auto* sf : selecter.selectedFleets)
	{
		Urban* u = sf->nowUrban;

		int cnt = 0;
		for (int i = int(sf->chain.readerPos); cnt < int(sf->chain.size()); cnt++)
		{
			auto& ring = sf->chain.rings[i];
			if (ring.code == Code::Move)
			{
				for (auto* r : u->ownRoutes)
				{
					if (r->toUrban->id() == ring.value && r->movingCost < sf->data.range && r->isSeaRoute == sf->data.isShip)
					{
						r->drawFlag = true;
						u = r->toUrban;
					}
				}
				i++;
			}
			else if (ring.code == Code::Jump) i = ring.value;
			else i++;
			if (i == int(sf->chain.readerPos) || i >= int(sf->chain.size())) break;
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		for (const auto& f : fleets)
		{
			auto pos = f.pos();

			f.ownVehicles.front()->data().icon.resized(0.005).rotated(f.angle()).drawAt(pos);

		}
	}
	for (int i = 0; i < 2; ++i)
	{
		for (auto& f : fleets)
		{
			if (f.isInSelectedRegion())
			{
				const auto transformer = tinyCamera.createTransformer(i);
				f.shape().draw(Palette::Lightgreen).drawFrame(4 / 5000.0, Palette::White);
			}
			else if (selecter.selectedFleets.includes(&f))
			{
				const auto transformer = tinyCamera.createTransformer(i);
				f.shape().draw(Palette::Red).drawFrame(4 / 5000.0, Palette::Pink);
			}
			else
			{
				const auto transformer = tinyCamera.createTransformer(i);
				f.shape().draw(Palette::Darkgreen).drawFrame(4 / 5000.0, Palette::Green);
			}
			{
				double r = 5000.0;
				RectF rect(f.shape().pos*r, f.shape().size*r);
				const auto transformer = tinyCamera.createTransformer(i, r);
				(*globalFonts[16])(f.name).draw(rect.pos.movedBy(4, 1));
			}
		}
	}
}
bool	Fleet::mouseOver() const
{
	return shape().mouseOver();
}
RectF	Fleet::shape() const
{
	return RectF(pos(), Vec2(160, 24) / 5000.0);
}
bool	Fleet::isInSelectedRegion() const
{
	//‘I‘ð‚³‚ê‚»‚¤‚ÈUrban‚ª‚ ‚é‚©
	auto f = []() {
		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);
			for (auto& u : urbans)
			{
				if (u.mouseOver() && u.isSelected) return true;
			}
		}
		return false;
	};

	if (selecter.selectedPos && !f())
	{
		for (int i = 0; i < 2; ++i)
		{
			const auto& p1 = tinyCamera.getMat3x2(i).inversed().transform(selecter.selectedPos.value());
			const auto& p2 = tinyCamera.getMat3x2(i).inversed().transform(Cursor::PosF());
			const auto& pos = Vec2(Min(p1.x, p2.x), Min(p1.y, p2.y));
			const auto& size = Vec2(Abs(p1.x - p2.x), Abs(p1.y - p2.y));

			if (RectF(pos, size).intersects(shape())) return true;
		}
	}
	return false;
}