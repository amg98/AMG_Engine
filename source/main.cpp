// Includes C/C++
#include <stdio.h>
#include <stdlib.h>

// Own includes
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "Model.h"
#include "Light.h"
#include "Skybox.h"
#include "Font.h"
#include "ParticleSource.h"
#include "World.h"
#include "LensFlare.h"
#include "ShadowRenderer.h"
#include "WaterTile.h"
#include "DeferredRendering.h"
#include "BloomEffect.h"
#include "GaussianBlur.h"
#include "MotionBlur.h"
using namespace AMG;

// Definition of objects
Camera *cam = NULL;
Model *link = NULL, *bullet = NULL, *barrel = NULL;
Skybox *skybox = NULL;
Shader *s0 = NULL, *s1 = NULL, *s2 = NULL, *s3 = NULL, *s4 = NULL, *s5 = NULL, *s6 = NULL, *s7 = NULL, *s00 = NULL, *s60 = NULL, *s10 = NULL, *s70 = NULL;
Text *hello = NULL;
Sprite *sprite = NULL;
ParticleSource *source = NULL;
Texture *cubeMap = NULL;
LensFlare *lens = NULL;
Light *light = NULL, *spot = NULL;
Font *font = NULL;
WaterTile *water = NULL;

void renderSimple(){

	s00->enable();
	link->draw();

	s2->enable();
	skybox->draw();
}

void renderShadows(){
	link->drawSimple();
	bullet->drawSimple();
	barrel->drawSimple();
}

void renderWater(vec4 plane){
	s00->enable();
	s00->setWaterClipPlane(plane);
	link->draw();
	s00->disableWaterClipPlane();

	s70->enable();
	s70->setWaterClipPlane(plane);
	barrel->draw();
	s70->disableWaterClipPlane();

	s60->enable();
	s60->setWaterClipPlane(plane);
	cubeMap->bind(0);
	bullet->draw();
	s60->disableWaterClipPlane();

	s2->enable();
	s2->setWaterClipPlane(plane);
	skybox->draw();
	s2->disableWaterClipPlane();

	s5->enable();
	s5->setWaterClipPlane(plane);
	source->draw(GL_ONE);
	s5->disableWaterClipPlane();
}

void render(){

	Renderer::updateCamera(cam);

	ShadowRenderer::updateShadowMap(renderShadows, light);

	DeferredRendering::start();

	s0->enable();
	link->animate(0, 0);
	link->draw();

	s7->enable();
	barrel->draw();
	barrel->getObject(0)->getRotation() *= quat(vec3(0, 0.01f, 0));

	s6->enable();
	cubeMap->bind(0);
	bullet->draw();

	DeferredRendering::end();

	s2->enable();
	skybox->draw();

	water->prepare(renderWater);
	water->draw();

	s5->enable();
	source->update();
	source->draw(GL_ONE);

	Object *clicked = Renderer::getWorld()->getClickingObject(20.0f);
	if(clicked == bullet->getObject(2)){
		btRigidBody *b = Renderer::getWorld()->getRigidBody(clicked);
		b->setActivationState(1);
		b->setLinearVelocity(btVector3(0, 3, 0));
	}

	if(Renderer::getKey(GLFW_KEY_Q)){
		source->getParticles().push_back(Particle(vec3(0, 0, 0), vec3(0, 5, 2), 1, 5, 0, 1));
	}
}

void render2d(){
	s3->enable();
	lens->draw(cam, Renderer::getLights()[0]);
	sprite->draw();
	s4->enable();
	hello->draw();
}

void post(){
	MotionBlur::render(Renderer::get3dFramebuffer(), s3)->blitToScreen();
}

void unload(){
	MotionBlur::finish();
	ShadowRenderer::finish();
	WaterTile::finish();
	DeferredRendering::finish();
	AMG_DELETE(water);
	AMG_DELETE(cam);
	AMG_DELETE(barrel);
	AMG_DELETE(link);
	AMG_DELETE(bullet);
	AMG_DELETE(skybox);
	AMG_DELETE(s0);
	AMG_DELETE(s1);
	AMG_DELETE(s2);
	AMG_DELETE(s3);
	AMG_DELETE(s4);
	AMG_DELETE(s5);
	AMG_DELETE(s6);
	AMG_DELETE(s7);
	AMG_DELETE(s00);
	AMG_DELETE(s60);
	AMG_DELETE(s10);
	AMG_DELETE(s70);
	AMG_DELETE(hello);
	AMG_DELETE(sprite);
	AMG_DELETE(cubeMap);
	AMG_DELETE(source);
	AMG_DELETE(lens);
	AMG_DELETE(light);
	AMG_DELETE(font);
	AMG_DELETE(spot);
}

int main(int argc, char **argv){

	Renderer::initialize(1440, 900, "Window1", false, 4);
	Renderer::createWorld();
	Renderer::setRenderCallback(render);
	Renderer::setRender2dCallback(render2d);
	Renderer::setUnloadCallback(unload);
	Renderer::setPostCallback(post);
	Renderer::getFogDensity() = 0.1f;
	Renderer::getFogGradient() = 5.0f;

	MotionBlur::initialize();

	ShadowRenderer::initialize(2048, 10.0f, 100.0f);

	DeferredRendering::initialize(true, 32);

	WaterTile::initialize();
	water = new WaterTile("waterNormalMap.dds", "waterDUDV.dds", vec3(0, 2.5f, -10), 5.0f);

	cam = new Camera(vec3(-4, 3, 5));

	s0 = new Shader("default");
	s1 = new Shader("terrain");
	s2 = new Shader("skybox");
	s3 = new Shader("shader2d");
	s4 = new Shader("text2d");
	s5 = new Shader("particles");
	s6 = new Shader("bullet");
	s7 = new Shader("barrel");
	s00 = new Shader("_default");
	s70 = new Shader("_barrel");
	s60 = new Shader("_bullet");
	s10 = new Shader("_terrain");

	light = new Light(vec3(100000, 100000, 100000), vec3(1, 1, 0), vec3(0.0f, 0, 1));
	spot = new Light(vec3(0, 3, 0), vec3(0, 0, 1), vec3(0, 0, 1));
	spot->setSpotLight(vec3(0, 0, 1), M_PI/3.0f);
	Renderer::getLights().push_back(light);
	DeferredRendering::lights.push_back(light);
	DeferredRendering::lights.push_back(spot);
	DeferredRendering::lights.push_back(new Light(vec3(0, 10, 0), vec3(0, 1, 0), vec3(0.1f, 0, 1)));

	link = new Model("model2.amd");
	link->getObject(0)->getScale() = vec3(0.1f, 0.1f, 0.1f);
	link->getObject(0)->getPosition() = vec3(0.0f, 3.0f, -10.0f);

	bullet = new Model("bullet.amd");
	bullet->getObject(0)->getPosition().y += 2.0f;
	bullet->getObject(1)->getScale() = vec3(0.2f, 0.2f, 0.2f);
	bullet->getObject(2)->getScale() = vec3(0.5f, 0.5f, 0.5f);

	barrel = new Model("barrel.amd", true);
	barrel->getObject(0)->getScale() = vec3(0.1f, 0.1f, 0.1f);
	barrel->getObject(0)->getPosition() = vec3(0.0f, 3.0f, -3.0f);

	font = new Font("candara.dds", "candara.fnt");

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

	source = new ParticleSource("cosmic.dds", 32, 32, 1000);

	Renderer::getWorld()->addObjectBox(bullet->getObject(0), 0.0f);
	Renderer::getWorld()->addObjectBox(bullet->getObject(1), 5.0f);
	Renderer::getWorld()->addObjectConvexHull(bullet->getObject(2), 7.0f);

	skybox = new Skybox("sky");
	vec3 cmapPos = vec3(bullet->getObject(2)->getPosition());
	cmapPos.y = 0.0f;
	cubeMap = Renderer::createCubeMap(renderSimple, s6, 256, cmapPos);

	float lens_scale[] = {
		0.5f, 0.23f, 0.1f, 0.05f, 0.06f, 0.07f, 0.2f, 0.07f, 0.3f, 0.4f, 0.6f
	};
	lens = new LensFlare("lens", 0.4f, lens_scale);

	Renderer::update();

	return Renderer::exitProcess();
}
