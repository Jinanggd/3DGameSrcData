#ifndef OBJETO_H
#define OBJETO_H

#include "mesh.h";

class Objeto
{
public:
	Objeto();
	~Objeto();

	Mesh* m;
	int id_shader;
	int id_texture;
	int a = 0;

};


#endif
