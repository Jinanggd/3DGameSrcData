#include "Objeto.h"



Objeto::Objeto()
{
}


Objeto::~Objeto()
{
}

Objeto::Objeto(Mesh * m, int s, int t)
{
	this->m = m;
	id_shader = s;
	id_texture.push_back(t);
	name = "UNDEF";
	tag = "UNDEF";
}

Objeto::Objeto(Mesh * m, int s, int t, std::string n)
{
	this->m = m;
	id_shader = s;
	id_texture.push_back(t);
	name = n;
	tag = "UNDEF";
}
