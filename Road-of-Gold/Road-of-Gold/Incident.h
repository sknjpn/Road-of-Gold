#pragma once


struct Incident
{
	JSONValue	json;	//ì‡óe

	Incident(JSONValue _json)
		: json(_json)
	{}
	void	action();	//é¿çs
	void	addScuttle();
	void	update();
};
extern JSONReader incidentsJson;
extern Array<Incident>	incidents;
extern Array<String>	flagTexts;