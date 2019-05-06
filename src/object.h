#ifndef OBJECT_H
#define OBJECT_H

#include "mesh.h";
#include "camera.h"


enum class mat_types {

	rock,
	tree,
	house,
	plane

};

struct Material {

	Texture* texture;
	Shader* shader;

};


class Object
{


public:
	Object();
	~Object();

	Object(Mesh* m, mat_types type);
	void render();
	void setPosition(Vector3 pos);
	// model matrix 
	Matrix44 m;
	Mesh* mesh;
	Material mat;


};


#endif
