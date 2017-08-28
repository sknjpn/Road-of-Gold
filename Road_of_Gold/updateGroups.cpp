#include"Planet.h"
#include"Group.h"
#include"Vehicle.h"
#include"Wallet.h"
#include"Planet.h"
double tmr = 0.0;
void	updateGroups()
{

	tmr += planet.timeSpeed;
	if (tmr > 0.0)
	{
		tmr -= 50.0;
		//‰ºˆÊ1/4‚ÌŽ–‹ÆŒ©’¼‚µ
		for (int i = 0; i < int(vehicles.size() / 10); i++)
		{
			int min = 0;
			Vehicle* target = nullptr;
			for (auto& v : vehicles)
			{
				if (!v.stopFlag && (target == nullptr || min > v.wallet().money))
				{
					target = &v;
					min = v.wallet().money;
				}
			}
			if (i == 0) Log(L"Å’á:", min);
			if (target != nullptr) target->stopFlag = true;
		}
		int avg = 0;
		for (auto& v : vehicles) avg += v.wallet().money;
		Log(L"•½‹Ï:", avg / double(vehicles.size()));

		for (auto& v : vehicles)
		{
			if (v.wallet().money <= 0) v.stopFlag = true;
			v.wallet().money = 0;
			v.wallet().expenses = 0;
			v.wallet().income = 0;
		}
	}
}