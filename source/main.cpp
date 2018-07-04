// Includes C/C++
#include <stdio.h>
#include <stdlib.h>

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own includes
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "MeshData.h"
#include "Sprite.h"
#include "Model.h"
#include "Light.h"
#include "Terrain.h"
#include "Skybox.h"
using namespace AMG;

// Definition of objects
Renderer *window;
Camera *cam;
Model *link;
Sprite *sprite;
Terrain *terrain;
Skybox *skybox;
Shader *s0, *s1, *s2;

void render(){

	window->setCamera(cam);

	link->animate(0, 0);
	link->draw();

	link->objects[0]->rotation *= quat(vec3(0, 0, 0.05f));

	terrain->draw();

	skybox->draw();

	window->set3dMode(false);
	sprite->rotation += 0.05f;
	sprite->draw();
	window->set3dMode(true);
}

int main(int argc, char **argv){

	window = new Renderer(1024, 768, "Window1", true, false, 60);
	window->setRenderCallback(render);
	window->fogDensity = 0.1f;
	window->fogGradient = 5.0f;

	cam = new Camera(FPS_CAMERA);

	s0 = new Shader("Data/Shader/default.vs", "Data/Shader/default.fs", AMG_USE_LIGHTING(1) | AMG_USE_FOG | AMG_USE_SKINNING);
	s1 = new Shader("Data/Shader/terrain.vs", "Data/Shader/terrain.fs", AMG_USE_LIGHTING(1) | AMG_USE_FOG | AMG_USE_TEXTURE(5));
	s2 = new Shader("Data/Shader/skybox.vs", "Data/Shader/skybox.fs", 0);

	sprite = new Sprite("Data/Texture/font.dds", 32, 32);
	sprite->x = 300.0f;
	sprite->y = 300.0f;
	sprite->currentFrame = 65.0f;

	Light *light = new Light(vec3(0, 1.0f, 0), vec3(1, 1, 0), vec3(0.1f, 0, 1));
	s0->lights.push_back(light);
	s1->lights.push_back(light);

	link = new Model("Data/Model/model2.amd", s0);
	link->objects[0]->scale = vec3(0.1f, 0.1f, 0.1f);
	link->objects[0]->position = vec3(0.0f, 0.0f, 3.0f);
	link->objects[0]->rotation = quat(vec3(-3.141592f/2.0f, 0, 0));

	terrain = new Terrain(-0.5f, 0, "grassy2.dds", s1);
	terrain->materials[0]->addTexture("grassFlowers.dds");
	terrain->materials[0]->addTexture("mud.dds");
	terrain->materials[0]->addTexture("path.dds");
	terrain->materials[0]->addTexture("blendMap.dds");

	skybox = new Skybox("Data/Texture/sky", s2);

	window->update();

	return Renderer::exitProcess();
}
