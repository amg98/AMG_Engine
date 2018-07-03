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
using namespace AMG;

// Definition of objects
Renderer *window;
Camera *cam;
Model *link;
Sprite *sprite;
Terrain *terrain;

void render(){

	window->setCamera(cam);

	link->animate(0, 0, window->getDelta());
	link->draw(window);

	//link->objects[0]->position.z -= 0.02f;
	//link->objects[0]->rotation *= quat(vec3(0, 0, 0.05f));

	terrain->draw(window);

	window->set3dMode(false);
	sprite->rotation += 0.05f;
	sprite->draw(window);
	window->set3dMode(true);
}

int main(int argc, char **argv){

	window = new Renderer(1024, 768, "Window1", true, false, 60);
	window->setRenderCallback(render);
	window->fogDensity = 0.1f;
	window->fogGradient = 5.0f;

	cam = new Camera(FPS_CAMERA);

	sprite = new Sprite("Data/Texture/font.dds", 32, 32);
	sprite->x = 300.0f;
	sprite->y = 300.0f;
	sprite->currentFrame = 65.0f;

	Light *light = new Light(vec3(0, 1.0f, 0), vec3(1, 1, 1));
	Renderer::shader->lights.push_back(light);

	link = new Model("Data/Model/model2.amd");
	link->objects[0]->scale = vec3(0.1f, 0.1f, 0.1f);
	link->objects[0]->position = vec3(0.0f, 0.0f, 3.0f);
	link->objects[0]->rotation = quat(vec3(-3.141592f/2.0f, 0, 0));

	terrain = new Terrain(-0.5f, 0, "grassy2.dds");
	Terrain::terrainShader->enableOptions(AMG_USE_TEXTURE(5));
	terrain->materials[0]->addTexture("grassFlowers.dds");
	terrain->materials[0]->addTexture("mud.dds");
	terrain->materials[0]->addTexture("path.dds");
	terrain->materials[0]->addTexture("blendMap.dds");
	Terrain::terrainShader->lights.push_back(light);

	window->update();

	return Renderer::exitProcess();
}
