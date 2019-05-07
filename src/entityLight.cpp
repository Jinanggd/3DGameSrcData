#include "entityLight.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

EntityLight::EntityLight() : Entity()
{

}


EntityLight::~EntityLight()
{

}



EntityLight::EntityLight(float dif, float spec, Vector3 pos)
{
	this->diffuse_factor = dif;
	this->specular_factor = spec;
	this->model.setIdentity();
	this->model.translate(pos.x,pos.y,pos.z);

	this->color = Vector3(0.8, 0.8, 0.8);

}

void EntityLight::setPosition(Vector3 pos) {

	model.setIdentity();
	model.translate(pos.x,pos.y,pos.z);


}

void EntityLight::setColor(Vector3 col)
{
	this->color = col;
}

Vector3 EntityLight::getPosition() {

	return model.getTranslation();


}
