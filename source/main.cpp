#include <stdio.h>
#include <stdlib.h>

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "MeshData.h"
#include "Sprite.h"
using namespace AMG;

Shader *basic;
Renderer *window;
Camera *cam;
Texture *tex;
MeshData *cube;
Sprite *sprite;

void render(){

	basic->enable();
	tex->enable();

	window->setTransformation(glm::vec3(0, 0, -5.0f), sprite->rotation, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	window->updateMVP(basic, cam);

	cube->draw();

	window->set3dMode(false);
	sprite->rotation += 0.005f;
	sprite->draw(window);
	window->set3dMode(true);
}

int main(int argc, char **argv){

	window = new Renderer();
	window->setup(1024, 768, "Window1", true);
	window->setRenderCallback(render);

	cam = new Camera(NO_MOVE_CAMERA);

	tex = new Texture();
	tex->load("Data/Texture/texture.dds");

	sprite = new Sprite("Data/Texture/texture.dds");
	sprite->x = 300.0f;
	sprite->y = 300.0f;

	static const int indices[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		31, 32, 33, 34, 35
	};

	static const GLfloat g_vertex_buffer_data[] = {
	    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
	    -1.0f,-1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f, // triangle 1 : end
	    1.0f, 1.0f,-1.0f, // triangle 2 : begin
	    -1.0f,-1.0f,-1.0f,
	    -1.0f, 1.0f,-1.0f, // triangle 2 : end
	    1.0f,-1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f,
	    1.0f,-1.0f,-1.0f,
	    1.0f, 1.0f,-1.0f,
	    1.0f,-1.0f,-1.0f,
	    -1.0f,-1.0f,-1.0f,
	    -1.0f,-1.0f,-1.0f,
	    -1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f,
	    1.0f,-1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f,
	    -1.0f, 1.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f,
	    1.0f,-1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,
	    1.0f,-1.0f,-1.0f,
	    1.0f, 1.0f,-1.0f,
	    1.0f,-1.0f,-1.0f,
	    1.0f, 1.0f, 1.0f,
	    1.0f,-1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f,-1.0f,
	    -1.0f, 1.0f,-1.0f,
	    1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f,-1.0f,
	    -1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f,
	    1.0f,-1.0f, 1.0f
	};

	static const GLfloat g_uv_buffer_data[] = {
	    0.000059f, 1.0f-0.000004f,
	    0.000103f, 1.0f-0.336048f,
	    0.335973f, 1.0f-0.335903f,
	    1.000023f, 1.0f-0.000013f,
	    0.667979f, 1.0f-0.335851f,
	    0.999958f, 1.0f-0.336064f,
	    0.667979f, 1.0f-0.335851f,
	    0.336024f, 1.0f-0.671877f,
	    0.667969f, 1.0f-0.671889f,
	    1.000023f, 1.0f-0.000013f,
	    0.668104f, 1.0f-0.000013f,
	    0.667979f, 1.0f-0.335851f,
	    0.000059f, 1.0f-0.000004f,
	    0.335973f, 1.0f-0.335903f,
	    0.336098f, 1.0f-0.000071f,
	    0.667979f, 1.0f-0.335851f,
	    0.335973f, 1.0f-0.335903f,
	    0.336024f, 1.0f-0.671877f,
	    1.000004f, 1.0f-0.671847f,
	    0.999958f, 1.0f-0.336064f,
	    0.667979f, 1.0f-0.335851f,
	    0.668104f, 1.0f-0.000013f,
	    0.335973f, 1.0f-0.335903f,
	    0.667979f, 1.0f-0.335851f,
	    0.335973f, 1.0f-0.335903f,
	    0.668104f, 1.0f-0.000013f,
	    0.336098f, 1.0f-0.000071f,
	    0.000103f, 1.0f-0.336048f,
	    0.000004f, 1.0f-0.671870f,
	    0.336024f, 1.0f-0.671877f,
	    0.000103f, 1.0f-0.336048f,
	    0.336024f, 1.0f-0.671877f,
	    0.335973f, 1.0f-0.335903f,
	    0.667969f, 1.0f-0.671889f,
	    1.000004f, 1.0f-0.671847f,
	    0.667979f, 1.0f-0.335851f
	};

	cube = new MeshData(12*3);
	cube->addBuffer((void*)g_vertex_buffer_data, sizeof(g_vertex_buffer_data), 3);
	cube->addBuffer((void*)g_uv_buffer_data, sizeof(g_uv_buffer_data), 2);
	cube->setIndexBuffer((void*)indices, sizeof(indices));

	basic = new Shader();
	basic->load("Data/Shader/texture.vs", "Data/Shader/texture.fs");

	do {
		window->update();
	}while(window->running());

	delete window;
	return Renderer::exitProcess();
}
