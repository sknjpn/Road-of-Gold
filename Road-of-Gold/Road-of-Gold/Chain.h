#pragma once

struct Fleet;
struct Urban;

enum struct Code
{
	None,
	Move,	//ˆø”‚ÌID‚Ì“ss‚ÉˆÚ“®
	Jump,	//ˆø”‚Ì”Ô’n‚ÉƒWƒƒƒ“ƒv
	Wait,	//ˆê“ú‹x~
	Buy,
	Sell,
	ERR,	//ˆÙí‚È’l
};

struct Ring
{
	Code	code;
	int		value;

	Ring(Code _code, int _value)
		: code(_code)
		, value(_value)
	{}
	String	valueText() const;
	String	codeText() const;
	void	set(Code _code, int _value);
};

struct Chain
{
	Array<Ring>	rings;
	size_t	readerPos;
	bool	isError;

	Chain() : readerPos(0), isError(false) {}
	size_t	size() const { return rings.size(); }
	bool	update(Fleet* _f);
	void	clear();
	Chain&	operator =(const Chain& _chain);
	Ring&	nowRing() { return rings[readerPos]; }
};