#ifndef ENTITYMESH_H
#define ENTITYMESH_H

#include "mesh.h";
#include "camera.h"
#include "entity.h"
#include "shader.h"
#include "texture.h"
#include "GUI.h"



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
	tower2,
	explosion

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
	void downgrade();
	bool ExtractLife();

	bool upgrade(mat_types t,float time);
	std::vector<Vector2> buildQuadUVS(float minX, float maxX, float minY, float maxY);
	void updateQUAD();

	void setPosition(Vector3 pos);
	int type;
	int index_propsvector;
	std::vector<int> munition;
	Vector3 getPosition();

	// model matrix 
	Mesh* mesh;
	Mesh* mesh2;
	Mesh* lowmesh;
	Material mat;
	Vector3 center;
	//DEPRACATED -- NOW USING ENTITY PLAYER CLASS
	//Firs person camera
	Camera * camera;
	Vector3 front;
	Vector3 Direction;
	GUI g;
	int life = 3; 

	mat_types tobeupgrate;
	float initial_time = -1.0f;
	float duration = 2.0f;
	float rotation;

	float explosion_initial_time = -1.0f;

};


#endif
