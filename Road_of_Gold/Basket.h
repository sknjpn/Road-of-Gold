#pragma once
#include"Ring.h"

struct ItemData;
struct Casket;
struct Urban;

struct TradeLog
{
	Array<int>	numTrade;		//æˆø¬—§”
	Array<int>	price;			//•½‹Ïæˆø‰¿Ši
	Array<int>	numProduction;	//s–¯‚É‚æ‚é¶Y—Ê
	Array<int>	numConsumption;	//s–¯‚É‚æ‚éÁ”ï
	int		time;	//•Û‘¶‚µ‚Ä‚¢‚éŠúŠÔ

	TradeLog()
		: time(120)
	{
		numTrade.resize(time);
		price.resize(time);
		numProduction.resize(time);
		numConsumption.resize(time);
	}

	void	push()
	{
		numTrade.push_front(0);
		price.push_front(price.front());
		numProduction.push_front(0);
		numConsumption.push_front(0);
		time++;
	}

	void	addTrade(int _price, int _num = 1);
};
void	drawGraph(const Rect& _rect, Array<int> _log, int _timeScale, Color _color);

struct Basket
{
	int		numItem;
	Urban*	joinedUrban;
	TradeLog	tradeLog;
	Array<Ring>	rings;

	Basket();
	int		itemType() const;
	ItemData	data() const;
};