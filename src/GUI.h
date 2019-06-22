#ifndef GUI_H
#define GUI_H

#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"

enum class GUI_Types {
	//Test
	basic,
	//Interface
	title,
	instruct_mov,
	instruct_attack,
	instruct_building,
	instruct_titan,
	//Hints to the players
	BulletKeysNC,
	BulletKeysC,
	CannonKeysNC,
	CannonKeysC,
	Building,
	OverallKeys,
	//Titan
	TitanLife,
	//Minimap
	minimap
};

class GUI
{

public:

	GUI();
	GUI(Vector2 o, Vector2 s, bool en,GUI_Types t);
	
	void GetTexture(GUI_Types t);
	void render();
	void setPositionfrom3D(Vector3 pos3D, Vector2 size, Matrix44 vpm);
	void buildQuad();
	void buildQuadUVS(float minX, float maxX, float minY, float maxY);

	// model matrix 
	Matrix44 model;
	Texture* texture;
	Mesh* mesh;
	Shader* shader;
	bool enable, isHUD = false;
	int type,index=-1;
	float starttime=-1.0f;
	float duration = 2.0f;

	Vector2 origin, size;
	Vector3 color;
};


#endif
