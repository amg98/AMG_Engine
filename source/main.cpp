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
#include "ParticleSource.h"
using namespace AMG;

// Definition of objects
Renderer *window;
Camera *cam;
Model *link;
Terrain *terrain;
Skybox *skybox;
Shader *s0, *s1, *s2, *s3, *s4, *s5;
Text *hello;
Sprite *sprite;
ParticleSource *source;

void render(){

	window->setCamera(cam);

	s0->enable();
	link->animate(0, 0);
	link->draw();

	link->objects[0]->rotation *= quat(vec3(0, 0, 0.05f));

	s1->enable();
	terrain->draw();

	s2->enable();
	skybox->draw();

	if(window->getKey(GLFW_KEY_Q)){
		source->particles.push_front(new Particle(vec3(0, 0, 0), vec3(0, 5, 2), 1, 5, 0, 1));
	}

	s3->enable();
	source->update();
	source->draw(GL_ONE);

	window->set3dMode(false);
	s3->enable();
	sprite->draw();
	s4->enable();
	hello->draw();
	window->set3dMode(true);
}

int main(int argc, char **argv){

	window = new Renderer(1024, 768, "Window1", true, false);
	window->setRenderCallback(render);
	window->fogDensity = 0.1f;
	window->fogGradient = 5.0f;

	cam = new Camera(FPS_CAMERA);

	s0 = new Shader("default.vs", "default.fs", AMG_USE_LIGHTING(1) | AMG_USE_FOG | AMG_USE_SKINNING);
	s1 = new Shader("terrain.vs", "terrain.fs", AMG_USE_LIGHTING(1) | AMG_USE_FOG | AMG_USE_TEXTURE(5));
	s2 = new Shader("skybox.vs", "skybox.fs", 0);
	s3 = new Shader("shader2d.vs", "shader2d.fs", AMG_USE_2D | AMG_USE_TEXANIM);
	s4 = new Shader("text2d.vs", "text2d.fs", AMG_USE_2D | AMG_USE_TEXT);

	Light *light = new Light(vec3(0, 1.0f, 0), vec3(1, 1, 0), vec3(0.1f, 0, 1));
	s0->lights.push_back(light);
	s1->lights.push_back(light);

	link = new Model("model2.amd", s0);
	link->objects[0]->scale = vec3(0.1f, 0.1f, 0.1f);
	link->objects[0]->position = vec3(0.0f, 0.0f, 3.0f);
	link->objects[0]->rotation = quat(vec3(-3.141592f/2.0f, 0, 0));

	terrain = new Terrain(-0.5f, 0, "grassy2.dds");
	terrain->materials[0]->addTexture("grassFlowers.dds");
	terrain->materials[0]->addTexture("mud.dds");
	terrain->materials[0]->addTexture("path.dds");
	terrain->materials[0]->addTexture("blendMap.dds");

	skybox = new Skybox("sky");

	Font *font = new Font("candara.dds", "candara.fnt");

	float tbx = 300;
	float tby = 300;

	int remaining = 0;
	char text[512];
	sprintf(text, "Cada vez que escribo una entrada en este blog mis companeros de estudio se burlan un poco. Dicen que sufro de incontinencia, que no manejo adecuadamente los codigos de un blog, que me extiendo demasiado");
	hello = font->createText(text, 32, tbx, tby, &remaining);
	hello->position.x = 200;
	hello->position.y = 600;
	hello->color = vec4(1, 0, 0, 1);

	sprite = new Sprite("texture.dds");
	sprite->x = hello->position.x + tbx/2;
	sprite->y = hello->position.y - tby/2 + font->lineHeight;
	sprite->sx = tbx / 256;
	sprite->sy = tby / 256;

	source = new ParticleSource("cosmic.dds", 32, 32);

	window->update();

	return Renderer::exitProcess();
}
