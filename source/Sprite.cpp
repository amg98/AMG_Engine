/**
 * @file Sprite.cpp
 * @brief Creation and rendering of sprites
 * Used as a template to make more complicated sprite types (e.g. animated sprites)
 */

#include "Sprite.h"
#include "Shader.h"

namespace AMG {

bool Sprite::internalBuffersInit = false;
static GLuint verticesId;					/**< Primitive OpenGL buffer ID */
static GLuint vertexDataId;					/**< Vertices OpenGL buffer ID */
static GLuint uvDataId;						/**< Texture coordinates OpenGL buffer ID */
static Shader *shader2d;					/**< Internal shader to allow 2D drawing */

static float spr_vertices[] = {				/**< The actual vertex buffer contents */
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f
};

static float uv_vertices[] = {				/**< The actual texture coordinates buffer contents */
	0, 1,
	1, 1,
	1, 0,
	0, 1,
	1, 0,
	0, 0
};


/**
 * @brief Constructor for an Sprite, extends a Texture
 * @param path Location of the sprite image (*.dds)
 */
Sprite::Sprite(const char *path, int frameWidth, int frameHeight) : Texture(path, frameWidth, frameHeight) {

	// Initialise data
	this->x = 0.0f;
	this->y = 0.0f;
	this->rotation = 0.0f;
	this->sx = 1.0f;
	this->sy = 1.0f;
	this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Initialise internal buffers and load 2D shader
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
		shader2d = new Shader("Data/Shader/shader2d.vs", "Data/Shader/shader2d.fs", AMG_USE_2D);
	}
}

/**
 * @brief Draw an Sprite in the proper window
 * @param renderer The window in which the sprite is going to be drawn
 */
void Sprite::draw(Renderer *renderer){
	Shader *shader = Renderer::shader;
	shader2d->enable();
	shader2d->setUniform("AMG_SprColor", color);
	renderer->setTransformation(glm::vec3(x, y, 0), glm::quat(glm::vec3(0, 0, rotation)), glm::vec3(sx * texScale.x * width, sy * texScale.y * height, 1.0f));
	renderer->updateMVP();
	enable(0);			// Enable texture
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
	shader->enable();
}

/**
 * @brief Destructor for a Sprite, the same as for a Texture
 */
Sprite::~Sprite() {

}

}
