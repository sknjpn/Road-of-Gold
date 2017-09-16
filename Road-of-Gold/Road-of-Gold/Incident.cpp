#include"Incident.h"
#include"Scuttle.h"
#include"EnergyData.h"
#include"Urban.h"
#include"Planet.h"

Array<String>	flagTexts;

void	Incident::update()
{
	for (auto c : json[L"Conditions"].arrayView())
	{
		auto t = c[L"ConditionType"].getString();
		if (t == L"Random")
		{
			if (!RandomBool(c[L"Value"].get<double>())) return;
		}
		else if (t == L"FlagIsTrue")
		{
			if (!flagTexts.include(c[L"FlagText"].getString())) return;
		}
		else if (t == L"FlagIsFalse")
		{
			if (flagTexts.include(c[L"FlagText"].getString())) return;
		}
		else if (t == L"TimeIs")
		{
			int time = c[L"Year"].get<int>() * 360 + (c[L"Month"].get<int>() - 1) * 30 + (c[L"Day"].get<int>() - 1);
			if (int(planet.sandglass.timer) != time) return;
		}
		else Output << L"ë∂ç›ÇµÇ»Ç¢Condition";
	}
	action();
}

void	Incident::addScuttle()
{
	//planet.timeSpeed = 0.0;
	String	s;
	s += json[L"Scuttle"][L"Description"].getString() + L"\n";
	s += L"\nå¯â \n";
	for (auto a : json[L"Actions"].arrayView())
	{
		s += L"\t";
		auto t = a[L"ActionType"].getString();
		if (t == L"PullEnergy")
		{
			s += a[L"Urban"].getString();
			s += L"Ç™";
			s += Format(a[L"Num"].get<int>());
			s += a[L"EnergyName"].getString();
			s += L"è¡é∏\n";
		}
		else if (t == L"AddEnergy")
		{
			s += a[L"Urban"].getString();
			s += L"Ç…";
			s += Format(a[L"Num"].get<int>());
			s += a[L"EnergyName"].getString();
			s += L"í«â¡\n";
		}
		else if (t == L"CorrectProductivity")
		{
			s += a[L"Urban"].getString();
			s += L"ÇÃòJì≠å¯ó¶Ç…x";
			s += Format(a[L"Value"].get<double>());
			s += L"ÇÃï‚ê≥\n";
		}

	}
	scuttles.emplace_back(json[L"Scuttle"][L"Title"].getString(), s, json[L"Scuttle"][L"Button"].getString());
	if (FileSystem::Exists(json[L"Scuttle"][L"Sound"].getString()))
	{
		scuttles.back().audio = Audio(json[L"Scuttle"][L"Sound"].getString());
		scuttles.back().audio.play();
	}
}

void	Incident::action()
{
	addScuttle();
	for (auto a : json[L"Actions"].arrayView())
	{
		auto t = a[L"ActionType"].getString();
		if (t == L"PullEnergy")
		{
			auto* u = getUrban(a[L"Urban"].getString());
			u->pullEnergy(a[L"EnergyName"].getString(), a[L"Num"].get<int>());
		}
		else if (t == L"AddEnergy")
		{
			auto* u = getUrban(a[L"Urban"].getString());
			u->addEnergy(a[L"EnergyName"].getString(), a[L"Num"].get<int>());
		}
		else if (t == L"CorrectProductivity")
		{
			auto* u = getUrban(a[L"Urban"].getString());
			u->productivity *= a[L"Value"].get<double>();
		}
		else if (t == L"AddFlag")
		{
			flagTexts.emplace_back(a[L"FlagText"].getString());
		}
		else if (t == L"RemoveFlag")
		{
			flagTexts.remove(a[L"FlagText"].getString());
		}
		else Output << L"ë∂ç›ÇµÇ»Ç¢Action";
	}
}