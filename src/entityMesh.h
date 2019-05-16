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
	sky,
	airplane,
	tower

};

struct Material {

	Texture* texture;
	Texture* texture2;
	Shader* shader;
};


class EntityMesh : public Entity
{


public:
	EntityMesh();
	~EntityMesh();

	EntityMesh( mat_types type);
	void render();
	void update(float elapsed_time);
	void move(Vector3 delta);
	void rotate(float angle, Vector3 axis);

	void setPosition(Vector3 pos);
	int type;
	Vector3 getPosition();

	// model matrix 
	Mesh* mesh;
	Mesh* mesh2;
	Mesh* lowmesh;
	Material mat;

	//Firs person camera
	Camera * camera;
	Vector3 front;


};


#endif
