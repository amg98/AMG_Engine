#include <stdio.h>
#include <stdlib.h>

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "MeshData.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Model.h"
using namespace AMG;

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Shader *basic;
Renderer *window;

Camera *cam;
Model *link;
AnimatedSprite *sprite;

void render(){

	basic->enable();
	window->setCamera(cam);

	link->draw(window, basic);
	link->objects[0]->rootBone->children[0]->children[2]->children[0]->children[0]->axis = vec3(1.0f, 0.0f, 0.0f);
	link->objects[0]->rootBone->children[0]->children[2]->children[0]->children[0]->angle += 0.05f;

	window->set3dMode(false);
	sprite->rotation += 0.005f;
	sprite->draw(window);
	window->set3dMode(true);
}

int main(int argc, char **argv){

	window = new Renderer(1024, 768, "Window1", true);
	window->setRenderCallback(render);

	cam = new Camera(NO_MOVE_CAMERA);

	//sprite = new Sprite("Data/Texture/texture.dds");
	sprite = new AnimatedSprite("Data/Texture/font.dds", 32, 32);
	sprite->x = 300.0f;
	sprite->y = 300.0f;
	sprite->currentFrame = 65.0f;
	//sprite->color = vec4(1.0f, 0.0f, 0.0f, 0.7f);

	basic = new Shader("Data/Shader/texture.vs", "Data/Shader/texture.fs");

	link = new Model("Data/Model/model2.amd", basic);
	link->objects[0]->scale = vec3(0.1f, 0.1f, 0.1f);
	link->objects[0]->position = vec3(0.0f, -0.5f, 3.0f);
	link->objects[0]->angle = -3.141592f/2.0f;
	link->objects[0]->axis = vec3(1.0f, 0.0f, 0.0f);

	do {
		window->update();
	}while(window->running());

	return Renderer::exitProcess();
}
