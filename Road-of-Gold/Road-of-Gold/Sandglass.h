#pragma once

struct Sandglass
{
	double	timer;

	Sandglass(double _timer = 0);
	bool	update();	//‚à‚µ“ú•t‚ğ‚Ü‚½‚¢‚¾ê‡true
	int		year() const { return int(timer / 360); }
	int		month() const { return int(timer / 30) % 12 + 1; }
	int		day() const { return int(timer) % 30 + 1; }
	int		hour() const { return int(timer * 24) % 24; }
	int		minute() const { return int(timer * 24 * 60) % 60; }
	void	set(int _year, int _month = 1, int _day = 1, int _hour = 0, int _minute = 0)
	{
		timer = ((_year * 12 + _month - 1) * 30 + _day - 1) + (double(_hour) + double(_minute) / 60.0) / 24.0;
	}
};