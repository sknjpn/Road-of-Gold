#include"Nation.h"
#include"Urban.h"

Nation::Nation(const JSONValue& _json)
	: name(_json[L"Name"].getString())
{
	for (auto j : _json[L"OwnUrbans"].arrayView())
	{
		auto u = getUrban(j.getString());
		if (u != nullptr) ownUrbans.emplace_back(u);
	}
}