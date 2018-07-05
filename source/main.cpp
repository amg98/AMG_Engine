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
#include "Font.h"
using namespace AMG;

// Definition of objects
Renderer *window;
Camera *cam;
Model *link;
Terrain *terrain;
Skybox *skybox;
Shader *s0, *s1, *s2;
Text *hello;
Sprite *sprite;

void render(){

	window->setCamera(cam);

	link->animate(0, 0);
	link->draw();

	link->objects[0]->rotation *= quat(vec3(0, 0, 0.05f));

	terrain->draw();

	skybox->draw();

	window->set3dMode(false);
	sprite->draw();
	hello->draw();
	window->set3dMode(true);
}

int main(int argc, char **argv){

	window = new Renderer(1024, 768, "Window1", true, false, 60);
	window->setRenderCallback(render);
	window->fogDensity = 0.1f;
	window->fogGradient = 5.0f;

	cam = new Camera(FPS_CAMERA);

	s0 = new Shader("default.vs", "default.fs", AMG_USE_LIGHTING(1) | AMG_USE_FOG | AMG_USE_SKINNING);
	s1 = new Shader("terrain.vs", "terrain.fs", AMG_USE_LIGHTING(1) | AMG_USE_FOG | AMG_USE_TEXTURE(5));
	s2 = new Shader("skybox.vs", "skybox.fs", 0);

	Light *light = new Light(vec3(0, 1.0f, 0), vec3(1, 1, 0), vec3(0.1f, 0, 1));
	s0->lights.push_back(light);
	s1->lights.push_back(light);

	link = new Model("model2.amd", s0);
	link->objects[0]->scale = vec3(0.1f, 0.1f, 0.1f);
	link->objects[0]->position = vec3(0.0f, 0.0f, 3.0f);
	link->objects[0]->rotation = quat(vec3(-3.141592f/2.0f, 0, 0));

	terrain = new Terrain(-0.5f, 0, "grassy2.dds", s1);
	terrain->materials[0]->addTexture("grassFlowers.dds");
	terrain->materials[0]->addTexture("mud.dds");
	terrain->materials[0]->addTexture("path.dds");
	terrain->materials[0]->addTexture("blendMap.dds");

	skybox = new Skybox("sky", s2);

	Font *font = new Font("font.dds", "font.fnt");

	float tbx = 300;
	float tby = 300;

	int remaining = 0;
	char text[256];
	sprintf(text, "Cada vez que escribo una entrada en este blog mis compañeros de estudio se burlan un poco. Dicen que sufro de incontinencia, que no manejo adecuadamente los códigos de un blog, que me extiendo demasiado");
	hello = font->createText((const char*)text, 32, tbx, tby, &remaining);
	hello->position.x = 200;
	hello->position.y = 600;
	hello->color = vec4(1, 0, 0, 1);

	sprite = new Sprite("texture.dds");
	sprite->x = hello->position.x + tbx/2;
	sprite->y = hello->position.y - tby/2 + 39;
	sprite->sx = tbx / 256;
	sprite->sy = tby / 256;

	window->update();

	return Renderer::exitProcess();
}
