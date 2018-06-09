/*
 * Sprite.cpp
 *
 *  Created on: 2 jun. 2018
 *      Author: Andrés
 */

#include "Sprite.h"
#include "Shader.h"

namespace AMG {

bool Sprite::internalBuffersInit = false;
static GLuint verticesId;
static GLuint vertexDataId;
static GLuint uvDataId;
static Shader *shader2d;

static float spr_vertices[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f
};

static float uv_vertices[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 0,
	1, 1,
	0, 1
};

Sprite::Sprite(const char *path) {
	this->texture = new Texture();
	this->texture->load(path);
	this->x = 0.0f;
	this->y = 0.0f;
	this->rotation = 0.0f;
	this->sx = 1.0f;
	this->sy = 1.0f;
	this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	if(!internalBuffersInit){
		internalBuffersInit = true;
		glGenVertexArrays(1, &verticesId);
		glBindVertexArray(verticesId);
		glGenBuffers(1, &vertexDataId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexDataId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(spr_vertices), spr_vertices, GL_STATIC_DRAW);
		glGenBuffers(1, &uvDataId);
		glBindBuffer(GL_ARRAY_BUFFER, uvDataId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uv_vertices), uv_vertices, GL_STATIC_DRAW);
		shader2d = new Shader();
		shader2d->load("Data/Shader/shader2d.vs", "Data/Shader/shader2d.fs");
	}
}

void Sprite::draw(Renderer *renderer){
	shader2d->enable();
	renderer->setTransformation(glm::vec3(x, y, 0), rotation, glm::vec3(0, 0, 1), glm::vec3(sx * texture->getWidth(), sy * texture->getHeight(), 1.0f));
	renderer->updateMVP(shader2d, NULL);
	texture->enable();
	glBindVertexArray(verticesId);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexDataId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvDataId);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
}

} /* namespace AMG */
