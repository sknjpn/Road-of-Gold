#pragma once


struct Incident
{
	JSONValue	json;	//ì‡óe

	Incident(JSONValue _json)
		: json(_json)
	{}
	void	action();	//é¿çs
	void	addScuttle();
	String	name() const;
	String	description() const;
	String	type() const;
	double	time() const;
};
extern JSONReader incidentJson;
extern Array<Incident>	incidents;
