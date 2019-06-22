#include "GUI.h"


GUI::GUI()
{
}

GUI::GUI(Vector2 o, Vector2 s, bool en, GUI_Types t)
{
	this->mesh = new Mesh();
	this->model.setIdentity();
	this->origin = o;
	this->size = s;
	this->enable = en;
	//this->shader = Shader::Get("data/shaders/GUI_Vertex.vs", "data/shaders/GUI_Fragment_basic.fs");
	this->shader = t == GUI_Types::basic? Shader::Get("data/shaders/GUI_Vertex.vs", "data/shaders/GUI_Fragment_basic.fs"):
		Shader::Get("data/shaders/GUI_Vertex.vs","data/shaders/GUI_Fragment_texture.fs");

	buildQuad();
	switch (t) {
	case GUI_Types::basic:
			break;
	default:
		GetTexture(t);
		break;
	}
	this->type = (int)t;

	
	//this->mesh->createQuad(origin.x, origin.y, size.x, size.y, false);
	this->color = Vector3(1, 1, 1);
}

void GUI::GetTexture(GUI_Types t)
{
	//Runnning throught the atlas
	texture = Texture::Get("data/ATLAS_GUI.tga");
	this->mesh->uvs.clear();
	float x0, x1, y0, y1;
	switch (t)
	{
	case GUI_Types::basic:
		break;
	case GUI_Types::title:
		x0 = 0; x1 = 799.0f / 1600.0f; y0 = 601.0f / 1200.0f; y1 = 1;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::instruct_mov:
		x0 = 0; x1 = 400.0f / 1600.0f; y0 = (1200.0f-897) / 1200.0f; y1 = 599.0f/1200.0f;
		buildQuadUVS(x0, x1, y0, y1);

		//this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));
		//this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
		//this->mesh->uvs.push_back(Vector2(0.25f, 0.5f));

		//this->mesh->uvs.push_back(Vector2(0.0f, 0.25f));
		//this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
		//this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));

		break;
		
	case GUI_Types::instruct_titan:
		x0 = 401.0f / 1600.0f; x1 = 800.0f / 1600.0f; y0 = (1200.0f - 897) / 1200.0f; y1 = 599.0f / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);

		break;
		
	case GUI_Types::instruct_attack:
		x0 = 801.0f / 1600.0f; x1 = 1200.0f / 1600.0f; y0 = (1200.0f - 897) / 1200.0f; y1 = 599.0f / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
		
	case GUI_Types::instruct_building:
		x0 = 1201.0f / 1600.0f; x1 = 1; y0 = (1200.0f - 897) / 1200.0f; y1 = 599.0f / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;

	case GUI_Types::BulletKeysNC:
		//ATT: WIDTH = 228 HEIGHT = 52
		x0 = 18.0f / 1600.0f; x1 = 246.0f / 1600.0f; y0 = (1200.0f - 979.0f) / 1200.0f; y1 = (1200.0f - 927.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::BulletKeysC:
		x0 = 18.0f / 1600.0f; x1 = 246.0f / 1600.0f; y0 = (1200.0f - 1044.0f) / 1200.0f; y1 = (1200.0f - 992.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::CannonKeysNC:
		x0 = 22.0f / 1600.0f; x1 = 250.0f / 1600.0f; y0 = (1200.0f - 1112.0f) / 1200.0f; y1 = (1200.0f - 1060.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::CannonKeysC:
		x0 = 564.0f / 1600.0f; x1 = 789.0f / 1600.0f; y0 = (1200.0f - 1150.0f) / 1200.0f; y1 = (1200.0f - 934.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);

		break;
	case GUI_Types::Building:
		x0 = 281.0f / 1600.0f; x1 = 510.0f / 1600.0f; y0 = (1200.0f - 1028.0f) / 1200.0f; y1 = (1200.0f - 926.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::OverallKeys:
		break;
	case GUI_Types::TitanLife:
		x0 = 826.0f / 1600.0f; x1 = 1079.0f / 1600.0f; y0 = (1200.0f - 969.0f) / 1200.0f; y1 = (1200.0f - 945.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);

		//LIFEBAR 1 -> MAX POINT(1079,1200-945) MIN POINT(826,1200-969)
		//LIFEBAR 2 ->MAX POINT(997,1200-981) MIN POINT(826,1200-1005)
		//LIFEBAR 3 ->MAX POINT(824,1200-1041) MIN POINT(912,1200-1017)
		break;
	case GUI_Types::TitanLife2:
		x0 = 826.0f / 1600.0f; x1 = 997.0f / 1600.0f; y0 = (1200.0f - 1005.0f) / 1200.0f; y1 = (1200.0f - 981.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::TitanLife3:
		x0 = 912.0f / 1600.0f; x1 = 824.0f / 1600.0f; y0 = (1200.0f - 1017.0f) / 1200.0f; y1 = (1200.0f - 1041.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::minimap:
		break;
	default:
		this->mesh->uvs.push_back(Vector2(1.0f, 0.0f ));
		this->mesh->uvs.push_back(Vector2(0.0f,1.0f));
		this->mesh->uvs.push_back(Vector2(1.0f, 1.0f));
		this->mesh->uvs.push_back(Vector2(0.0f,0.0f));
		this->mesh->uvs.push_back(Vector2(0.0f,1.0f));
		this->mesh->uvs.push_back(Vector2(1.0f,0.0f));
		break;
	}
}

void GUI::render()
{
	this->shader->setUniform("u_color", Vector4(color.x,color.y,color.z,1.0));
	if (type == (int)GUI_Types::basic) {

	}
	else {
		this->shader->setUniform("u_texture", this->texture);
	}
	bool f = false;
	this->shader->setUniform("is3D", f);
	this->mesh->render(GL_TRIANGLES);
	//this->mesh->renderAnimated(GL_TRIANGLES, this->anim);
}

void GUI::setPositionfrom3D(Vector3 pos3D, Vector2 size, Matrix44 vpm)
{
	Vector4 pos2D = vpm*Vector4(pos3D.x, pos3D.y, pos3D.z, 1.0);
	pos2D.x /= pos2D.w;
	pos2D.y /= pos2D.w;
	this->origin = Vector2(pos2D.x, pos2D.y);
	this->size = size;
	buildQuad();

	this->isHUD = true;

}

void GUI::buildQuad()
{
	//this->mesh->createQuad(origin.x, origin.y, size.x, size.y, false);

	this->mesh->vertices.clear();
	float center_x = origin.x, center_y = origin.y, w = size.x, h = size.y;

	this->mesh->vertices.push_back(Vector3(center_x + w * 0.5f, center_y - h * 0.5f, 0.0f));
	this->mesh->vertices.push_back(Vector3(center_x - w * 0.5f, center_y + h * 0.5f, 0.0f));
	this->mesh->vertices.push_back(Vector3(center_x + w * 0.5f, center_y + h * 0.5f, 0.0f));
	this->mesh->vertices.push_back(Vector3(center_x - w * 0.5f, center_y - h * 0.5f, 0.0f));
	this->mesh->vertices.push_back(Vector3(center_x - w * 0.5f, center_y + h * 0.5f, 0.0f));
	this->mesh->vertices.push_back(Vector3(center_x + w * 0.5f, center_y - h * 0.5f, 0.0f));
}

//Get the edges vertices and build the UVS
void GUI::buildQuadUVS(float minX, float maxX, float minY, float maxY)
{

	//this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));
//this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
//this->mesh->uvs.push_back(Vector2(0.25f, 0.5f));

//this->mesh->uvs.push_back(Vector2(0.0f, 0.25f));
//this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
//this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));
	this->mesh->uvs.push_back(Vector2(maxX, minY));
	this->mesh->uvs.push_back(Vector2(minX, maxY));
	this->mesh->uvs.push_back(Vector2(maxX, maxY));

	this->mesh->uvs.push_back(Vector2(minX, minY));
	this->mesh->uvs.push_back(Vector2(minX, maxY));
	this->mesh->uvs.push_back(Vector2(maxX, minY));
}
