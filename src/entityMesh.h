#ifndef ENTITYMESH_H
#define ENTITYMESH_H

#include "mesh.h";
#include "camera.h"
#include "entity.h"
#include "texture.h"
#include "shader.h"


enum class mat_types {

	rock,
	tree,
	house,
	plane,
	sky,
	airplane,
	tower,
	player,
	bullet,
	cannon,
	buildable,
	tower1,
	tower2

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

	void setPosition(Vector3 pos);
	int type;
	int index_propsvector;
	int chargeditem = -1;
	Vector3 getPosition();

	// model matrix 
	Mesh* mesh;
	Mesh* mesh2;
	Mesh* lowmesh;
	Material mat;

	//DEPRACATED -- NOW USING ENTITY PLAYER CLASS
	//Firs person camera
	Camera * camera;
	Vector3 front;
	Vector3 Direction;


};


#endif
