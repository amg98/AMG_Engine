/**
 * @file World.h
 * @brief Physics worlds with Bullet Physics
 */

#ifndef WORLD_H_
#define WORLD_H_

// Includes C/C++
#include <tr1/unordered_map>

// Includes Bullet
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

// Own includes
#include "Entity.h"
#include "Object.h"

namespace AMG {

/**
 * @class World
 * @brief World utilities to be used with Bullet Physics library
 */
class World: public Entity {
private:
	btDefaultCollisionConfiguration* collisionConfiguration;		/**< Collision configuration */
	btCollisionDispatcher* dispatcher;								/**< Collision dispatcher */
	btBroadphaseInterface* broadphase;								/**< Broadphase (checks if AABBs collide, for each object) */
	btSequentialImpulseConstraintSolver* solver;					/**< Impulse solver */
	btDiscreteDynamicsWorld* dynamicsWorld;							/**< The actual dynamics world */
	std::tr1::unordered_map<Object*, btCollisionObject*> objects;	/**< Object <-> CollisionObject dictionary */
	std::vector<btCollisionShape*> shapes;							/**< Collision shapes */
public:
	World();
	void addObject(Object *obj, float mass, btCollisionShape *shape);
	void addObjectBox(Object *obj, float mass);
	void addObjectSphere(Object *obj, float mass);
	void addObjectConvexHull(Object *obj, float mass);
	Object *getClickingObject(float rayLength);
	btRigidBody *getRigidBody(Object *obj);
	void removeObject(Object *obj);
	void update(float delta);
	virtual ~World();
};

}

#endif
