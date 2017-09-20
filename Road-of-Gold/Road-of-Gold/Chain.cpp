#include"Chain.h"
#include"Urban.h"
#include"ItemData.h"
#include"Vehicle.h"
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
bool	Chain::update(Vehicle* _v)
{
	for (int cnt = 0;; cnt++)
	{
		if (readerPos < 0 || readerPos >= rings.size() || cnt > rings.size()) isError = true;
		if (isError) return true;
		if (_v->route != nullptr || _v->sleepTimer > 0) return false;
		auto& ring = rings[readerPos];

		switch (ring.code)
		{
		case Code::None:
			readerPos++;
			break;
		case Code::Move:
			for (auto& r : _v->nowUrban->ownRoutes)
			{
				if (r->isSeaRoute == _v->data().isShip && r->toUrban->id() == ring.value && r->movingCost < _v->data().range)
				{
					_v->route = r;
					r->addVehicle();
					break;
				}
			}
			if (_v->route == nullptr) readerPos++;
			break;
		case Code::Wait:
			if (ring.value == 0) readerPos++;
			else _v->sleepTimer = ring.value / 24.0;
			break;
		case Code::Jump:
			readerPos = ring.value;
			break;
		case Code::Buy:
		{
			bool flag = false;
			int volume = int(_v->data().volume / itemData[ring.value].volume);

			for (auto& b : _v->nowUrban->buyers)
			{
				if (b.walletID == _v->walletID)
				{
					b.progress = 0;
					_v->cargo = b.casket;
					b.casket.numItem = 0;
					b.target = volume;
					b.period = int(_v->period);
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				auto* u = _v->nowUrban;
				u->buyers.emplace_back(_v->walletID, ring.value, int(_v->period), volume);
			}
			_v->sleepTimer = 0.5;
			break;
		}
		case Code::Sell:
			if (_v->cargo.numItem > 0)
			{
				_v->nowUrban->sellers.emplace_back(_v->walletID, _v->cargo, int(_v->period));
			}
			_v->cargo.numItem = 0;
			_v->sleepTimer = 0.5;
			break;
		case Code::ERR:
			isError = true;
			break;
		}
	}
}