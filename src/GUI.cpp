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
	texture = t==GUI_Types::scope? Texture::Get("data/crosshair.tga") : Texture::Get("data/ATLAS_GUI.tga");
	this->mesh->uvs.clear();
	float x0, x1, y0, y1;
	switch (t)
	{
	case GUI_Types::basic:
		x0 = 0; x1 = 1; y0 = 0; y1 = 1;
		buildQuadUVS(x0,x1,y0,y1);
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
	case GUI_Types::instruct_help:
		x0 = 0; x1 = 400.0f / 1600.0f; y0 = 0; y1 = (1200.0f - 900.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::BulletKeysNC:
		//ATT: WIDTH = 228 HEIGHT = 52
		x0 = 821.0f / 1600.0f; x1 = 894.0f / 1600.0f; y0 = (1200.0f - 1120.0f) / 1200.0f; y1 = (1200.0f - 1047.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::BulletKeysC:
		x0 = 897.0f / 1600.0f; x1 =970.0f / 1600.0f; y0 = (1200.0f - 1121.0f) / 1200.0f; y1 = (1200.0f - 1048.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::CannonKeysNC:
		x0 = 821.0f / 1600.0f; x1 = 894.0f / 1600.0f; y0 = (1200.0f - 1120.0f) / 1200.0f; y1 = (1200.0f - 1047.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::CannonKeysC1:
		x0 = 1059.0f / 1600.0f; x1 = 1283.0f / 1600.0f; y0 = (1200.0f - 1194.0f) / 1200.0f; y1 = (1200.0f - 1045.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::CannonKeysC2:
		x0 = 1468.0f / 1600.0f; x1 = 1590.0f / 1600.0f; y0 = (1200.0f - 1194.0f) / 1200.0f; y1 = (1200.0f - 1045.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;

	case GUI_Types::Building:
		x0 = 823.0f / 1600.0f; x1 = 971.0f / 1600.0f; y0 = (1200.0f - 1197.0f) / 1200.0f; y1 = (1200.0f - 1124.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
		break;
	case GUI_Types::OverallKeys:
		x0 = 980.f / 1600.0f; x1 = 1053.0f / 1600.0f; y0 = (1200.0f - 1122.0f) / 1200.0f; y1 = (1200.0f - 1049.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
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
	case GUI_Types::scope:
		buildQuadUVS(0, 1, 0, 1);
		break;
	case GUI_Types::BuildingT1:
		x0 = 821.0f / 1600.0f; x1 = 894.0f / 1600.0f; y0 = (1200.0f - 1120.0f) / 1200.0f; y1 = (1200.0f - 1047.0f) / 1200.0f;
		buildQuadUVS(x0, x1, y0, y1);
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
	if((type <(int)GUI_Types::BulletKeysNC && type >= (int)GUI_Types::title ) && (type != (int)GUI_Types::basic))
		this->shader->setUniform("u_color", Vector4(color.x,color.y,color.z,1.0));
	else
		this->shader->setUniform("u_color", Vector4(color.x, color.y, color.z, 0.5));

	if (type == (int)GUI_Types::basic) {
		this->shader->setUniform("u_color", Vector4(0,0.7f,0, 1.0));
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
