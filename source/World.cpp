/**
 * @file World.cpp
 * @brief Physics worlds with Bullet Physics
 */

// Includes Bullet
#include <BulletCollision/CollisionShapes/btShapeHull.h>

// Own includes
#include "Renderer.h"
#include "Debug.h"
#include "World.h"

namespace AMG {

/**
 * @brief Constructor for a World
 */
World::World() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	objects = std::tr1::unordered_map<Object*, btCollisionObject*>();
	shapes = std::vector<btCollisionShape*>();
}

/**
 * @brief Add an Object to this world, in a Box shape
 * @param obj Object to be added
 * @param mass Mass for this object
 */
void World::addObjectBox(Object *obj, float mass){
	vec3 &bbox = obj->getBBox();
	btCollisionShape* shape = new btBoxShape(btVector3(bbox.x, bbox.y, bbox.z));
	addObject(obj, mass, shape);
}

/**
 * @brief Add an Object to this world, in a Sphere shape
 * @param obj Object to be added
 * @param mass Mass for this object
 */
void World::addObjectSphere(Object *obj, float mass){
	vec3 &bbox = obj->getBBox();
	btCollisionShape* shape = new btSphereShape(bbox.x);
	addObject(obj, mass, shape);
}

/**
 * @brief Add an Object to this world, in a Convex Hull shape
 * @param obj Object to be added
 * @param mass Mass for this object
 */
void World::addObjectConvexHull(Object *obj, float mass){
	float *data = obj->getVertices();
	if(data == NULL){
		Debug::showError(NO_VERTEX_DATA, NULL);
	}

	btConvexHullShape *convexHullShape = new btConvexHullShape(data, obj->getNVertices(), 3 * sizeof(float));
	convexHullShape->setMargin(0);
	btShapeHull* hull = new btShapeHull(convexHullShape);
	hull->buildHull(0);
	btConvexHullShape* pConvexHullShape = new btConvexHullShape((const btScalar*)hull->getVertexPointer(), hull->numVertices(), sizeof(btVector3));
	addObject(obj, mass, pConvexHullShape);
	delete hull;
	delete convexHullShape;
}

/**
 * @brief Add an Object to this world
 * @param obj Object to be added
 * @param mass Mass for this object
 * @param shape Collision shape for this new object
 */
void World::addObject(Object *obj, float mass, btCollisionShape *shape){

	vec3 &position = obj->getPosition();
	vec3 &scale = obj->getScale();
	quat &rot = obj->getRotation();

	shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	shapes.push_back(shape);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));
	transform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.0f)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setUserPointer(obj);

	dynamicsWorld->addRigidBody(body);
	objects[obj] = body;
}

/**
 * @brief Retrieves the rigid body associated to an Object
 * @param obj Object to get the rigid body from
 * @return The linked rigid body, NULL if it does not exist
 */
btRigidBody *World::getRigidBody(Object *obj){
	std::tr1::unordered_map<Object*, btCollisionObject*>::const_iterator got = objects.find(obj);
	if(got != objects.end()){
		return btRigidBody::upcast(got->second);
	}
	return NULL;
}

/**
 * @brief Removes an Object from the World
 * @param obj Which object do we want to remove
 */
void World::removeObject(Object *obj){
	std::tr1::unordered_map<Object*, btCollisionObject*>::const_iterator got = objects.find(obj);
	if(got != objects.end()){
		btRigidBody* body = btRigidBody::upcast(got->second);
		if (body && body->getMotionState()){
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(got->second);
		objects.erase(got);
		delete got->second;
	}
}

/**
 * @brief Performs a Ray test to check if the user touches any Object
 * @param rayLength Length of the ray
 * @return Whether the ray test succeeded or not
 * @note The object must exist in this World, ensure that a Camera is set
 */
Object *World::getClickingObject(float rayLength){

	Camera *cam = Renderer::getCamera();
	vec3 &camPos = cam->getPosition();
	vec3 rayEnd = camPos + cam->getRay() * rayLength;
	btVector3 start = btVector3(camPos.x, camPos.y, camPos.z);
	btVector3 end = btVector3(rayEnd.x, rayEnd.y, rayEnd.z);

	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
	dynamicsWorld->rayTest(start, end, rayCallback);
	if(rayCallback.hasHit()) {
		btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (body){
			return (Object*) body->getUserPointer();
		}
	}

	return NULL;
}

/**
 * @brief Does a simulation step
 * @param delta Delta time between steps, the lower the more precise
 */
void World::update(float delta){
	dynamicsWorld->stepSimulation(delta);

	for(int i=0;i<dynamicsWorld->getNumCollisionObjects();i++){
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		Object *o = (Object*) body->getUserPointer();
		btTransform &t = body->getWorldTransform();
		btVector3 &pos = t.getOrigin();
		btQuaternion rot = t.getRotation();
		o->getPosition() = vec3(pos.x(), pos.y(), pos.z());
		o->getRotation() = quat(rot.x(), rot.y(), rot.z(), rot.w());
	}
}

/**
 * @brief Destructor for a World
 */
World::~World() {

	for(int i=0;i<dynamicsWorld->getNumCollisionObjects();i++){
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()){
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	for(unsigned int i=0;i<shapes.size();i++){
		delete shapes[i];
	}

	delete dynamicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfiguration;
}

}
