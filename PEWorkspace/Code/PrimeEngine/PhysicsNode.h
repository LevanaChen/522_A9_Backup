#ifndef __PYENGINE_2_0_PHYSICNODE_H__
#define __PYENGINE_2_0_PHYSICNODE_H__

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Utils/Array/Array.h"
#include "PrimeEngine/Math/Matrix4x4.h"
#include "PrimeEngine/Math/Vector3.h"

//#define USE_DRAW_COMPONENT

namespace PE {
namespace Components {

struct ServerPhysicsNode{
	/* Now only support the car movement */
	enum ObjectType
	{
		UNKNOWN,	// For initialization recording
		ROAD,		// Object is road
		GRASS,		// Object is grass
		BOUNDING,	// Object is bounding
		CAR,		// Object is car
	};

	enum ObjectState
	{
		NA,			// state for static objects
		STOP,
		MOVING,
		FALLING,
	};
	ServerPhysicsNode(){}
	~ServerPhysicsNode(){}

	void do_INITIALIZATION();		// Initialize attributs and factors
	void do_RELOAD();				// Reload car, when car is falling

	// Object Basic Attributes
	ObjectType m_type;
	ObjectState m_state;
	int m_id;			// Record main object - car
	Matrix4x4 m_base;	// local transform
	Vector3 m_pos;
	Vector3 AABB_min;
	Vector3 AABB_max;

	// Physics related Attributes
	float u;			// friction coefficient - air friction
	float rr;			// rolling resistence
	float resistence;	// resistence amount
	float g;			// acceleration of gravity
	float accerleration;
	float velocity;
	float rotation;
	bool decreaseRotation;
	Vector3 fallingVelocity;
};

struct PhysicsNode : public Component
{
	PE_DECLARE_CLASS(PhysicsNode);

	enum ObjectType
	{
		UNKNOWN,	// For initialization recording
		ROAD,		// Object is road
		GRASS,		// Object is grass
		BOUNDING,	// Object is bounding
		CAR,		// Object is car
	};

	enum ObjectState
	{
		NA,			// state for static objects
		STOP,
		MOVING,
		FALLING,
	};

	// Constructor -------------------------------------------------------------
	PhysicsNode();

	PhysicsNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself);

	virtual ~PhysicsNode() {}

	// Component ------------------------------------------------------------

	virtual void addDefaultComponents();
	
	// Individual events -------------------------------------------------------

	//PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_CREATE_AABB);
	//virtual void do_CREATE_AABB(Events::Event *pEvt);

	//PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_CREATE_SPHERE);
	//virtual void do_CREATE_SPHERE(Events::Event *pEvt);

	Handle m_hComponentParent;

	bool m_inheritPositionOnly;

	void do_INITIALIZATION();		// Initialize attributs and factors
	void do_RELOAD();				// Reload car, when car is falling
	void setTransformMatrix(Matrix4x4 &matrix);

	bool do_PHYSICS_BOUNDING();		// Check whether the object is in the bounding
	bool do_PHYSICS_GRAVITY();		// Check whether the object is on the ground
	bool do_PHYSICS_GRAVITY(PhysicsNode *m_pPhysicsNode);
	bool do_PHYSICS_COLLISION(PhysicsNode *m_pPhysicsNode);	// Detection collision with car
	
	float distanceToPlane(Vector3 pos, Vector3 point, Vector3 normal);
	bool onSameSide(Vector3 normal, Vector3 p, Vector3 pos, Vector3 test);

	static PhysicsNode *s_pRootPhysicsNode;

	// Object Basic Attributes
	ObjectType m_type;
	ObjectState m_state;
	int m_id;			// Record main object - car
	Matrix4x4 m_base;	// local transform
	Vector3 m_pos;
	Vector3 AABB_min; 
	Vector3 AABB_max;

	// Physics related Attributes
	float u;			// friction coefficient - air friction
	float rr;			// rolling resistence
	float resistence;	// resistence amount
	float g;			// acceleration of gravity
	float accerleration;
	float velocity;
	float rotation;
	bool decreaseRotation;
	Vector3 fallingVelocity;

	// Car's border, using in Collision Detection
	float length;
	float width;
	Vector3 forward_left;
	Vector3 forward_right;
	Vector3 back_left;
	Vector3 back_right;

};

}; // namespace Components
}; // namespace PE
#endif
