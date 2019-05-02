#ifndef OBJETO_H
#define OBJETO_H

#include "mesh.h";

class Objeto
{
public:
	Objeto();
	~Objeto();

	Objeto(Mesh* m, int s, int t);
	Objeto(Mesh* m, int s, int t, std::string n);

	Mesh* m;
	int id_shader;
	
	std::vector<int> id_texture;
	std::string name;
	std::string tag;
	Vector3 position; //World Coordinate position

};


#endif
