// Includes C/C++
#include <stdio.h>
#include <stdlib.h>

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
#include "Framebuffer.h"
#include "World.h"
#include "BloomEffect.h"
using namespace AMG;

// Definition of objects
Renderer *window;
Camera *cam;
Model *link, *bullet;
Terrain *terrain;
Skybox *skybox;
Shader *s0, *s1, *s2, *s3, *s4, *s5, *s6;
Text *hello;
Sprite *sprite;
ParticleSource *source;
Framebuffer *fb;
Sprite *screen;

void render(){

	fb->bind();

	window->setCamera(cam);

	Object *clicked = window->getWorld()->getClickingObject(20.0f);
	if(clicked == bullet->getObject(2)){
		btRigidBody *b = window->getWorld()->getRigidBody(clicked);
		b->setActivationState(1);
		b->setLinearVelocity(btVector3(0, 3, 0));
	}

	s0->enable();
	link->animate(0, 0);
	link->draw();

	s6->enable();
	bullet->draw();

	link->getObject(0)->getRotation() *= quat(vec3(0, 0.05f, 0));

	s1->enable();
	terrain->draw();

	s2->enable();
	skybox->draw();

	if(window->getKey(GLFW_KEY_Q)){
		source->getParticles().push_front(new Particle(vec3(0, 0, 0), vec3(0, 5, 2), 1, 5, 0, 1));
	}

	s5->enable();
	source->update();
	source->draw(GL_ONE);

	fb->unbind();

	window->set3dMode(false);
	screen->set(BloomEffect::render(fb)->getColorTexture());
	s3->enable();
	screen->draw();
	sprite->draw();
	s4->enable();
	hello->draw();
	window->set3dMode(true);
}

int main(int argc, char **argv){

	window = new Renderer(1440, 900, "Window1", false, false, 0);
	window->createWorld();
	window->setRenderCallback(render);
	window->getFogDensity() = 0.1f;
	window->getFogGradient() = 5.0f;

	cam = new Camera(FPS_CAMERA);
	cam->getPosition().y = 3.0f;

	s0 = new Shader("default.vs", "default.fs", NULL, AMG_USE_LIGHTING(1) | AMG_USE_FOG | AMG_USE_SKINNING);
	s1 = new Shader("terrain.vs", "terrain.fs", NULL, AMG_USE_LIGHTING(1) | AMG_USE_FOG | AMG_USE_TEXTURE(5));
	s2 = new Shader("skybox.vs", "skybox.fs", NULL, 0);
	s3 = new Shader("shader2d.vs", "shader2d.fs", NULL, AMG_USE_2D | AMG_USE_TEXANIM);
	s4 = new Shader("text2d.vs", "text2d.fs", NULL, AMG_USE_2D | AMG_USE_TEXT);
	s5 = new Shader("particles.vs", "particles.fs", NULL, AMG_USE_TEXANIM | AMG_USE_INSTANCES);
	s6 = new Shader("bullet.vs", "bullet.fs", NULL, AMG_USE_LIGHTING(1) | AMG_USE_FOG);

	Light *light = new Light(vec3(1, 1.0f, 3), vec3(1, 1, 0), vec3(0.0f, 0, 1));
	s0->getLights().push_back(light);
	s1->getLights().push_back(light);
	s6->getLights().push_back(light);

	link = new Model("model2.amd", s0);
	link->getObject(0)->getScale() = vec3(0.1f, 0.1f, 0.1f);
	link->getObject(0)->getPosition() = vec3(0.0f, 3.0f, 3.0f);

	bullet = new Model("bullet.amd", s0);
	bullet->getObject(1)->getScale() = vec3(0.2f, 0.2f, 0.2f);
	bullet->getObject(2)->getScale() = vec3(0.5f, 0.5f, 0.5f);

	terrain = new Terrain(-0.5f, 0, "grassy2.dds");
	terrain->getMaterial(0)->addTexture("grassFlowers.dds");
	terrain->getMaterial(0)->addTexture("mud.dds");
	terrain->getMaterial(0)->addTexture("path.dds");
	terrain->getMaterial(0)->addTexture("blendMap.dds");

	skybox = new Skybox("sky");

	Font *font = new Font("candara.dds", "candara.fnt");

	float tbx = 300;
	float tby = 300;

	int remaining = 0;
	char text[512];
	sprintf(text, "Cada vez que escribo una entrada en este blog mis companeros de estudio se burlan un poco. Dicen que sufro de incontinencia, que no manejo adecuadamente los codigos de un blog, que me extiendo demasiado");
	hello = font->createText(text, 32, tbx, tby, &remaining);
	hello->getPosition().x = 200;
	hello->getPosition().y = 600;
	hello->getColor() = vec4(1, 0, 0, 1);

	sprite = new Sprite("texture.dds");
	sprite->getPosition().x = hello->getPosition().x + tbx/2;
	sprite->getPosition().y = hello->getPosition().y - tby/2 + font->getLineHeight();
	sprite->getScaleX() = tbx / 256;
	sprite->getScaleY() = tby / 256;

	source = new ParticleSource("cosmic.dds", 32, 32);

	window->getWorld()->addObjectBox(bullet->getObject(0), 0.0f);
	window->getWorld()->addObjectBox(bullet->getObject(1), 5.0f);
	window->getWorld()->addObjectConvexHull(bullet->getObject(2), 7.0f);

	fb = new Framebuffer(1440, 900);
	fb->createColorTexture(0);
	BloomEffect::initialize(1440, 900);

	screen = new Sprite();
	screen->getPosition() = vec3(720, 450, 0);
	screen->getScaleY() = -1.0f;

	window->update();

	return Renderer::exitProcess();
}
