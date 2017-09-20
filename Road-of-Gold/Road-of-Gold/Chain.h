#pragma once

struct Fleet;

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
	size_t	adress;
	Code	code;
	int		value;

	Ring(size_t	_adress, Code _code, int _value)
		: adress(_adress)
		, code(_code)
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
};