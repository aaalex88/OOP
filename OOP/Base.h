#pragma once

#include <string>



using namespace std;


class Printable
{
public:
	virtual ~Printable() {}
	virtual string AsString() const = 0;
};

class Named : public Printable
{
public:
	Named(string _name) : m_name(_name) {};

	string AsString() const override final
	{
		return m_name + "{ " + ContentAsString() + " }";
	}

protected:
	virtual string ContentAsString() const = 0;

	string m_name;
};