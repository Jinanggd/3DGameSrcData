#ifndef ENTITYMESH_H
#define ENTITYMESH_H

#include "mesh.h";
#include "camera.h"
#include "entity.h"


enum class mat_types {

	rock,
	tree,
	house,
	plane,
	sky

};

struct Material {

	Texture* texture;
	Shader* shader;

};


class EntityMesh : public Entity
{


public:
	EntityMesh();
	~EntityMesh();

	EntityMesh(Mesh* m, mat_types type);
	void render();
	void setPosition(Vector3 pos);
	Vector3 getPosition();
	// model matrix 
	Matrix44 m;
	Mesh* mesh;
	Material mat;


};


#endif
