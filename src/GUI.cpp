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

	switch (t) {
	case GUI_Types::basic:
			break;
	default:
		GetTexture(t);
		break;
	}
	this->type = (int)t;
	buildQuad();
	
	//this->mesh->createQuad(origin.x, origin.y, size.x, size.y, false);
	this->color = Vector3(1, 1, 1);
}

void GUI::GetTexture(GUI_Types t)
{
	//Runnning throught the atlas
	texture = Texture::Get("data/bullet.tga");
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
