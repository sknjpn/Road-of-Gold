#include"Chain.h"
#include"Urban.h"
#include"ItemData.h"
#include"Fleet.h"
#include"Route.h"
#include"VehicleData.h"

void	Ring::set(Code _code, int _value)
{
	code = _code;
	value = _value;
}
String	Ring::valueText() const
{
	switch (code)
	{
	case Code::None:return Format(adress, L"番地にジャンプ");
	case Code::Move:return Format(urbans[value].name, L"に移動");
	case Code::Jump:return Format(value, L"番地にジャンプ");
	case Code::Wait:
		if (value >= 24 && value % 24 == 0) return Format(value / 24, L"日待つ");
		if (value >= 24 && value % 24 != 0) return Format(value / 24, L"日と", value % 24, L"時間待つ");
		if (value < 24) return Format(value, L"時間待つ");
	case Code::Buy:	return Format(itemData[value].name, L"を購入");
	case Code::Sell:return Format(L"アイテムをすべて売却");
	case Code::ERR:	return Format(L"異常なコード");
	default:		return Format(L"異常なコード");
	}
}
String	Ring::codeText() const
{
	switch (code)
	{
	case Code::None:return L"None";
	case Code::Move:return L"Move";
	case Code::Jump:return L"Jump";
	case Code::Wait:return L"Wait";
	case Code::Buy:	return L"Buy";
	case Code::Sell:return L"Sell";
	case Code::ERR:	return L"ERR";
	default:		return L"ERR";
	}
}
void	Chain::clear()
{
	rings.clear();
	readerPos = 0;
	isError = false;
}
Chain&	Chain::operator =(const Chain& _chain) {
	rings = _chain.rings;
	readerPos = _chain.readerPos;
	isError = _chain.isError;
	return *this;
}
bool	Chain::update(Fleet* _f)
{
	for (int cnt = 0;; cnt++)
	{
		if (readerPos < 0 || readerPos >= rings.size() || cnt > int(rings.size())) isError = true;
		if (isError) return true;
		if (_f->route != nullptr || _f->sleepTimer > 0) return false;
		auto& ring = rings[readerPos];

		switch (ring.code)
		{
		case Code::None:
			readerPos++;
			break;
		case Code::Move:
			for (auto& r : _f->nowUrban->ownRoutes)
			{
				if (r->isSeaRoute == _f->data.isShip && r->toUrban->id() == ring.value && r->movingCost < _f->data.range)
				{
					_f->route = r;
					r->addVehicle();
					break;
				}
			}
			if (_f->route == nullptr) readerPos++;
			break;
		case Code::Wait:
			if (ring.value == 0) readerPos++;
			else _f->sleepTimer = ring.value / 24.0;
			break;
		case Code::Jump:
			readerPos = ring.value;
			break;
		case Code::Buy:
		{
			bool flag = false;
			int volume = int(_f->data.volume / itemData[ring.value].volume);

			for (auto& b : _f->nowUrban->buyers)
			{
				if (b.walletID == _f->walletID)
				{
					b.progress = 0;
					_f->cargo = b.casket;
					b.casket.numItem = 0;
					b.target = volume;
					b.period = int(_f->period);
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				auto* u = _f->nowUrban;
				u->buyers.emplace_back(_f->walletID, ring.value, int(_f->period), volume);
			}
			_f->sleepTimer = 0.5;
			break;
		}
		case Code::Sell:
			if (_f->cargo.numItem > 0)
			{
				_f->nowUrban->sellers.emplace_back(_f->walletID, _f->cargo, int(_f->period));
			}
			_f->cargo.numItem = 0;
			_f->sleepTimer = 0.5;
			break;
		case Code::ERR:
			isError = true;
			break;
		}
	}
}