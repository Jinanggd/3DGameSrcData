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
	texture = Texture::Get("data/GUI.tga");
	this->mesh->uvs.clear();
	float x0, x1, y0, y1;
	switch (t)
	{
	case GUI_Types::basic:
		break;
	case GUI_Types::title:
		this->mesh->uvs.push_back(Vector2(0.5f, 0.5f));
		this->mesh->uvs.push_back(Vector2(0.0f, 1.0f));
		this->mesh->uvs.push_back(Vector2(0.5f, 1.0f));

		this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
		this->mesh->uvs.push_back(Vector2(0.0f, 1.0f));
		this->mesh->uvs.push_back(Vector2(0.5f, 0.5f));
		break;
	case GUI_Types::instruct_mov:

		this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));
		this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
		this->mesh->uvs.push_back(Vector2(0.25f, 0.5f));

		this->mesh->uvs.push_back(Vector2(0.0f, 0.25f));
		this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
		this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));

		break;
	case GUI_Types::instruct_attack:
		this->mesh->uvs.push_back(Vector2(0.75f, 0.25f));
		this->mesh->uvs.push_back(Vector2(0.5f, 0.5f));
		this->mesh->uvs.push_back(Vector2(0.75f, 0.5f));

		this->mesh->uvs.push_back(Vector2(0.5f, 0.25f));
		this->mesh->uvs.push_back(Vector2(0.5f, 0.5f));
		this->mesh->uvs.push_back(Vector2(0.75f, 0.25f));
		break;
	case GUI_Types::instruct_building:
		this->mesh->uvs.push_back(Vector2(1.0f, 0.25f));
		this->mesh->uvs.push_back(Vector2(0.75f, 0.5f));
		this->mesh->uvs.push_back(Vector2(1.0f, 0.5f));

		this->mesh->uvs.push_back(Vector2(0.75f, 0.25f));
		this->mesh->uvs.push_back(Vector2(0.75f, 0.5f));
		this->mesh->uvs.push_back(Vector2(1.0f, 0.25f));
		break;
	case GUI_Types::instruct_titan:
		this->mesh->uvs.push_back(Vector2(0.5f, 0.25f));
		this->mesh->uvs.push_back(Vector2(0.25f, 0.5f));
		this->mesh->uvs.push_back(Vector2(0.5f, 0.5f));

		this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));
		this->mesh->uvs.push_back(Vector2(0.25f, 0.5f));
		this->mesh->uvs.push_back(Vector2(0.5f, 0.25f));
		break;
	case GUI_Types::BulletKeysNC:
		x1 = 166.0f / 3200.0f, y0 = 1854.0f / 2400.0f;
		this->mesh->uvs.push_back(Vector2(x1, y0));
		this->mesh->uvs.push_back(Vector2(0.0f, 0.25f));
		this->mesh->uvs.push_back(Vector2(x1, 0.25f));

		this->mesh->uvs.push_back(Vector2(0.0f, y0));
		this->mesh->uvs.push_back(Vector2(0.0f, 0.25f));
		this->mesh->uvs.push_back(Vector2(x1, y0));
		break;
	case GUI_Types::BulletKeysC:
		x0 = 166.0f / 3200.0f, y0 = 1854.0f / 2400.0f, x1 = 372.0f / 3200.0f;
		this->mesh->uvs.push_back(Vector2(x1, y0));
		this->mesh->uvs.push_back(Vector2(x0, 0.25f));
		this->mesh->uvs.push_back(Vector2(x1, 0.25f));

		this->mesh->uvs.push_back(Vector2(x0, y0));
		this->mesh->uvs.push_back(Vector2(x0, 0.25f));
		this->mesh->uvs.push_back(Vector2(x1, y0));
		break;
	case GUI_Types::CannonKeysNC:
		x0 = 372.0f / 3200.0f, y0 = 1854.0f / 2400.0f, x1 = 588.0f / 3200.0f;
		this->mesh->uvs.push_back(Vector2(x1, y0));
		this->mesh->uvs.push_back(Vector2(x0, 0.25f));
		this->mesh->uvs.push_back(Vector2(x1, 0.25f));

		this->mesh->uvs.push_back(Vector2(x0, y0));
		this->mesh->uvs.push_back(Vector2(x0, 0.25f));
		this->mesh->uvs.push_back(Vector2(x1, y0));
		break;
	case GUI_Types::CannonKeysC:
		x0 = 637.0f / 3200.f, y1 = 1818.0f / 2400.0f, x1 = 2230.0f / 3200.0f, y0 = 2078.0f / 2400.0f;
		this->mesh->uvs.push_back(Vector2(x1, y0));
		this->mesh->uvs.push_back(Vector2(x0, y1));
		this->mesh->uvs.push_back(Vector2(x1, y1));

		this->mesh->uvs.push_back(Vector2(x0, y0));
		this->mesh->uvs.push_back(Vector2(x0, y1));
		this->mesh->uvs.push_back(Vector2(x1, y0));
		break;
	case GUI_Types::Building:
		x1 = 230.0f / 3200.0f, y0 = 1977.0f / 2400.0f, y1 = 1856.0f / 2400.0f;
		this->mesh->uvs.push_back(Vector2(x1, y0));
		this->mesh->uvs.push_back(Vector2(0.0f, y1));
		this->mesh->uvs.push_back(Vector2(x1, y1));

		this->mesh->uvs.push_back(Vector2(0.0f, y0));
		this->mesh->uvs.push_back(Vector2(0.0f, y1));
		this->mesh->uvs.push_back(Vector2(x1, y0));
		break;
	case GUI_Types::OverallKeys:
		break;
	case GUI_Types::TitanLife:
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
	this->shader->setUniform("is3D", isHUD);
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
	this->mesh->createQuad(origin.x, origin.y, size.x, size.y, false);
}
