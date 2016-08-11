#include "PhysicsNode.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"
#include "PrimeEngine/Scene/MeshInstance.h"
#include "PrimeEngine/Scene/DefaultAnimationSM.h"
#include "PrimeEngine/Scene/SkeletonInstance.h"

#include "PrimeEngine/Scene/DebugRenderer.h"

namespace PE {
namespace Components {
// ServerPhysicsNode Implementation
void ServerPhysicsNode::do_INITIALIZATION(){
	// remember also to edit here if the initialization attributes have changed
	m_type = UNKNOWN;
	m_state = NA;

	u = 0.13f;
	g = 0.5f;
	rr = 0.4f;

	accerleration = 0.0f;
	velocity = 0.0f;
	rotation = 0.0f;
	decreaseRotation = false;
}

void ServerPhysicsNode::do_RELOAD(){
	accerleration = 0.0f;
	velocity = 0.0f;
	rotation = 0.0f;
	decreaseRotation = false;

	m_state = STOP;
}

PE_IMPLEMENT_CLASS1(PhysicsNode, Component);

PhysicsNode *PhysicsNode::s_pRootPhysicsNode = 0;

// Constructor -------------------------------------------------------------
PhysicsNode::PhysicsNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself) :  Component(context, arena, hMyself), m_inheritPositionOnly(false)
{

}

void PhysicsNode::addDefaultComponents()
{
	Component::addDefaultComponents();
	
	
	// Data components

	// event handlers
//	PE_REGISTER_EVENT_HANDLER(Events::Event_CREATE_AABB, PhysicsNode::do_CREATE_AABB);
//	PE_REGISTER_EVENT_HANDLER(Events::Event_CREATE_SPHERE, PhysicsNode::do_CREATE_SPHERE);
//
//}
//
//void PhysicsNode::do_CREATE_AABB(Events::Event *pEvt)
//{
//    Events::Event_CREATE_AABB *pRealEvent = (Events::Event_CREATE_AABB *)(pEvt);
//	
//	m_base = pSceneNode->m_base;
//	m_pos = m_base.getPos();
//	
//	OutputDebugStringA("PhysicsNode::do_CREATE_AABB/n");
//}
//
//void PhysicsNode::do_CREATE_SPHERE(Events::Event *pEvt)
//{
//	Events::Event_CREATE_SPHERE *pRealEvent = (Events::Event_CREATE_SPHERE *)(pEvt);
//	
//	OutputDebugStringA("PhysicsNode::do_CREATE_SPHERE/n");
}


void PhysicsNode::do_INITIALIZATION()
{
	OutputDebugStringA("PhysicsNode::do_INITIALIZATION.\n");

	m_type = UNKNOWN;
	m_state = NA;

	u = 0.13f;
	g = 0.5f;
	rr = 0.4f;

	accerleration = 0.0f;
	velocity = 0.0f;
	rotation = 0.0f;
	decreaseRotation = false;
	
}

void PhysicsNode::do_RELOAD()
{
	accerleration = 0.0f;
	velocity = 0.0f;
	rotation = 0.0f;
	decreaseRotation = false;

	this->m_state = STOP;
}

void PhysicsNode::setTransformMatrix(Matrix4x4 &matrix)
{
	m_base = matrix;
	m_pos = m_base.getPos();
}


bool PhysicsNode::do_PHYSICS_BOUNDING()
{
	//float minX,minZ,maxX,maxZ;
	//minX = -12.5f;
	//maxX = 12.5f;
	//minZ = -25.0f;
	//maxZ = 25.0f;

	//float delta = 0.01f + radius ;

	//float x = this->m_pos.getX();
	//float z = this->m_pos.getZ();

	//Vector3 v = this->velocity;

	//float offset = 0;

	//// data for ball holes
	//float z1, z2, z3, z4, x1, x2;
	//float w = 1.8f;

	//z1 = maxZ - w;
	//z2 = w;
	//z3 = - w;
	//z4 = minZ + w;
	//x1 = maxX - w;
	//x2 = minX + w;

	//if ( (x > (minX + delta) && x < (maxX - delta)) && (z > (minZ + delta) && z < (maxZ - delta)))
	//{
	//	// no collision with bounding
	//	// OutputDebugStringA("PhysicsNode::do_PHYSICS_REBOUND: No collisition with Bounding ***********************************  \n");
	//	return false;
	//}
	//else if ( x <= minX || x >= maxX || z <= minZ || z >= maxZ )
	//{
	//	// do fall down
	//	OutputDebugStringA("PhysicsNode::do_PHYSICS_REBOUND: Need to be falling down !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  \n");
	//	this->isFalling = true;
	//	return false;
	//}
	//// determine whether ball is kicked into hole HERE !!!!!!!!!

	//if ( x <= (minX + delta) || x >= (maxX - delta) )
	//{
	//	// hole area
	//	if ( z >= z1 || ( z <= z2 && z >= z3 ) || z <= z4) return false;
	//	// OutputDebugStringA("PhysicsNode::do_PHYSICS_REBOUND: Collision with Bounding XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  \n");

	//	if ( x <= (minX + delta) ) offset = minX + delta - x;
	//	if ( x >= (maxX - delta) ) offset = x - maxX + delta;
	//	this->velocity = Vector3( - v.getX(), v.getY(), v.getZ() );
	//	this->m_pos = this->m_pos + offset * this->velocity;
	//}
	//if ( z <= (minZ + delta) || z >= (maxZ - delta) )
	//{
	//	// hole area
	//	if ( x >= x1 || x <= x2 ) return false;
	//	// OutputDebugStringA("PhysicsNode::do_PHYSICS_REBOUND: Collision with Bounding ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ  \n");

	//	if ( z <= (minZ + delta) ) offset = minZ + delta - z;
	//	if ( z >= (maxZ - delta) ) offset = z - maxZ + delta;
	//	this->velocity = Vector3( v.getX(), v.getY(), - v.getZ() );
	//	this->m_pos = this->m_pos + offset * this->velocity;
	//}

	//// OutputDebugStringA("PhysicsNode::do_PHYSICS_REBOUND: Collision with Bounding ----------------------------------  \n");

	return true;
}

bool PhysicsNode::do_PHYSICS_COLLISION(PhysicsNode *m_pPhysicsNode)
{
	//OutputDebugStringA("PhysicsNode::do_PHYSICS_COLLISION.\n");

	float range = 1.01;
	// N: forward
	// U: right
	// V: up
	this->forward_left = this->m_pos + this->m_base.getN() * this->length * 0.5 * range - this->m_base.getU() * this->width * 0.5 * range;
	this->forward_right = this->m_pos + this->m_base.getN() * this->length * 0.5 * range + this->m_base.getU() * this->width * 0.5 * range;
	this->back_left = this->m_pos - this->m_base.getN() * this->length * 0.5 * range - this->m_base.getU() * this->width * 0.5 * range;
	this->back_right = this->m_pos - this->m_base.getN() * this->length * 0.5 * range + this->m_base.getU() * this->width * 0.5 * range;

	m_pPhysicsNode->forward_left = m_pPhysicsNode->m_pos + m_pPhysicsNode->m_base.getN() * m_pPhysicsNode->length * 0.5 * range - m_pPhysicsNode->m_base.getU() * m_pPhysicsNode->width * 0.5 * range;
	m_pPhysicsNode->forward_right = m_pPhysicsNode->m_pos + m_pPhysicsNode->m_base.getN() * m_pPhysicsNode->length * 0.5 * range + m_pPhysicsNode->m_base.getU() * m_pPhysicsNode->width * 0.5 * range;
	m_pPhysicsNode->back_left = m_pPhysicsNode->m_pos - m_pPhysicsNode->m_base.getN() * m_pPhysicsNode->length * 0.5 * range - m_pPhysicsNode->m_base.getU() * m_pPhysicsNode->width * 0.5 * range;
	m_pPhysicsNode->back_right = m_pPhysicsNode->m_pos - m_pPhysicsNode->m_base.getN() * m_pPhysicsNode->length * 0.5 * range + m_pPhysicsNode->m_base.getU() * m_pPhysicsNode->width * 0.5 * range;

	// Back_clockwise
	// forward_left -> back_left
	Vector3 dir_left = m_pPhysicsNode->back_left - m_pPhysicsNode->forward_left;
	dir_left.normalize();
	// back_right -> forward_right
	Vector3 dir_right = m_pPhysicsNode->forward_right - m_pPhysicsNode->back_right;
	dir_right.normalize();
	// forward_right -> forward_left
	Vector3 dir_forward = m_pPhysicsNode->forward_left - m_pPhysicsNode->forward_right;
	dir_forward.normalize();
	// back_left -> beck_right
	Vector3 dir_back = m_pPhysicsNode->back_right - m_pPhysicsNode->back_left;
	dir_back.normalize();

	bool isCollision = false;
	for ( int i = 0; i < 4 && !isCollision; i ++)
	{
		Vector3 test;
		
		// Rotation > 0 : turn left
		// Rotation < 0 : turn right
		if ( this->rotation >= 0 && this->velocity >= 0 )
		{
			switch (i)
			{
			case 0: test = this->forward_left; break;
			case 1: test = this->forward_right; break;
			case 2: test = this->back_left; break;
			case 3: test = this->back_right; break;
			}
		}
		else if ( this->rotation < 0 && this->velocity >= 0 )
		{
			switch (i)
			{
			case 0: test = this->forward_right; break;
			case 1: test = this->forward_left; break;
			case 2: test = this->back_right; break;
			case 3: test = this->back_left; break;
			}
		}
		else if ( this->rotation < 0 && this->velocity < 0 )
		{
			switch (i)
			{
			case 0: test = this->back_right; break;
			case 1: test = this->back_left; break;
			case 2: test = this->forward_right; break;
			case 3: test = this->forward_left; break;
			}
		}
		else if ( this->rotation < 0 && this->velocity < 0 )
		{
			switch (i)
			{
			case 0: test = this->back_left; break;
			case 1: test = this->back_right; break;
			case 2: test = this->forward_left; break;
			case 3: test = this->forward_right; break;
			}
		}
		if ( this->onSameSide(dir_left, m_pPhysicsNode->forward_left, m_pPhysicsNode->m_pos, test) )
			if ( this->onSameSide(dir_right, m_pPhysicsNode->back_right, m_pPhysicsNode->m_pos, test) )
				if ( this->onSameSide(dir_forward, m_pPhysicsNode->forward_right, m_pPhysicsNode->m_pos, test) )
					if ( this->onSameSide(dir_back, m_pPhysicsNode->back_left, m_pPhysicsNode->m_pos, test) )
					{
						float d[4];
						Vector3 dir = this->m_pos - test; //Vector3(0.0, 0.0, 0.0);
						dir.normalize();
						d[0] = this->distanceToPlane(test, m_pPhysicsNode->forward_left, dir_left);
						d[1] = this->distanceToPlane(test, m_pPhysicsNode->back_right, dir_right);
						d[2] = this->distanceToPlane(test, m_pPhysicsNode->forward_right, dir_forward);
						d[3] = this->distanceToPlane(test, m_pPhysicsNode->back_left, dir_back);

						float min_d = m_pPhysicsNode->length;
						int min_id = 4;
						for ( int i = 0; i < 4; i++ )
						{
							if ( d[i] <= min_d )
							{
								min_d = d[i];
								min_id = i;
							}
						}
						switch (min_id)
						{
						case 0:
							{
								// float sin = dir.crossProduct(dir_left).length();
								//min_d = d[i] / sin;
								dir = - m_pPhysicsNode->m_base.getU();
								break;
							}
						case 1:
							{
								//float sin = dir.crossProduct(dir_right).length();
								//min_d = d[i] / sin;
								dir = m_pPhysicsNode->m_base.getU();
								break;
							}
						case 2:
							{
								//float sin = dir.crossProduct(dir_forward).length();
								//min_d = d[i] / sin;
								dir = m_pPhysicsNode->m_base.getN();
								break;
							}
						case 3:
							{
								//float sin = dir.crossProduct(dir_back).length();
								//min_d = d[i] / sin;
								dir = - m_pPhysicsNode->m_base.getN();
								break;
							}
						}

						// Offset of current car's position
						dir.normalize();
						this->m_pos += dir * d[min_id] * range;
						isCollision = true;
					}
	}

	//if (isCollision )
	//	OutputDebugStringA("PhysicsNode::do_PHYSICS_COLLISION: Collision with cars !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!.\n");
	//else
	//	OutputDebugStringA("PhysicsNode::do_PHYSICS_COLLISION: No collision *******************************************.\n");

	//Vector3 vectori = ( m_pPhysicsNode->m_pos - this->m_pos ); // vector O1->O2
	//vectori.m_y = 0.0;
	//Vector3 vectorj;

	//float distance = vectori.length();
	//float delta = 0.05;
	//// There is not collision between these two balls
	//if (distance > (this->radius + m_pPhysicsNode->radius + delta)) return false;
	//
	//// Debug : Balls collision
	//float offset = this->radius + m_pPhysicsNode->radius + delta - distance;

	//vectori.normalize(); // unit vector 
	//vectorj = Vector3( - vectori.getZ(), 0, vectori.getX());

	//float dx1 = this->velocity.dotProduct(vectori);
	//float dx2 = m_pPhysicsNode->velocity.dotProduct(vectori);

	//// decompose velocity in center bisects and tangent directions
	//Vector3 x1 = dx1 * vectori;
	//Vector3 y1 = this->velocity - x1 ;
	//Vector3 x2 = dx2 * vectori;
	//Vector3 y2 = m_pPhysicsNode->velocity - x2;

	//bool exchange = false;

	//// Two velocities in opposite direction
	//if (dx1 * dx2 < 0)
	//{
	//	// need to exchange velocity on center bisects direction
	//	exchange = true;
	//}
	//else if ( dx1 <= 0 && dx2 <= 0 )
	//{
	//	if ( dx2 < dx1 ) exchange = true;
	//}
	//else if (dx1 >= 0 && dx2 >= 0 )
	//{
	//	if ( dx1 > dx2 ) exchange = true;
	//}

	//if ( exchange )
	//{
	//	this->velocity = x2 + y1;
	//	m_pPhysicsNode->velocity = x1 + y2;
	//}
	//else
	//{
	//	// there is no collision between these two balls
	//	// this->velocity = x1 + y1;
	//	// m_pPhysicsNode->velocity = x2 + y2;
	//	// OutputDebugStringA("PhysicsNode::do_PHYSICS_COLLISION: No collision with ball ************************************************.\n");
	//	return false;
	//}

	//// Debug : Balls collision
	//this->m_pos = this->m_pos + offset * this->velocity;

	//// OutputDebugStringA("PhysicsNode::do_PHYSICS_COLLISION: Collision with ball !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!.\n");
	return isCollision;
}

bool PhysicsNode::do_PHYSICS_GRAVITY()
{
	//OutputDebugStringA("PhysicsNode::do_PHYSICS_GRAVITY ------------------.\n");

	//this->accerleration = this->g * Vector3(0, -1, 0);

	return true;
}

// Check whether object should be falling
// True : should be falling
// False : should not
bool PhysicsNode::do_PHYSICS_GRAVITY(PhysicsNode *m_pPhysicsNode)
{
	//OutputDebugStringA("PhysicsNode::do_PHYSICS_GRAVITY.\n");
	
	if ( m_pPhysicsNode->m_type != GRASS ) return false;

	float minX,minZ,maxX,maxZ;
	//float offset = 5.0f;
	//minX = m_pPhysicsNode->AABB_min.getX() - offset;
	//maxX = m_pPhysicsNode->AABB_max.getX() + offset;
	//minZ = m_pPhysicsNode->AABB_min.getZ() - offset;
	//maxZ = m_pPhysicsNode->AABB_max.getZ() + offset;
	minX = m_pPhysicsNode->AABB_min.getX();
	maxX = m_pPhysicsNode->AABB_max.getX();
	minZ = m_pPhysicsNode->AABB_min.getZ();
	maxZ = m_pPhysicsNode->AABB_max.getZ();
	
	float x = m_pos.getX();
	float z = m_pos.getZ();
	float y = (m_base * AABB_min).getY();
	if ( (x > minX && x < maxX) && (z > minZ && z < maxZ))
	//if ( (x > minX && x < maxX) && (z > minZ && z < maxZ) && y ==0)
	{
		//OutputDebugStringA("PhysicsNode::do_PHYSICS_GRAVITY: Object is on the ground.\n");
		return false;
	}
	else
	{
		OutputDebugStringA("PhysicsNode::do_PHYSICS_GRAVITY: Soldier should be falling down.\n");
		m_state = FALLING;
		Vector3 dir = this->m_base.getN();
		dir.normalize();
		fallingVelocity = this->velocity * dir;
		return true;
	}
}

float PhysicsNode::distanceToPlane(Vector3 pos, Vector3 point, Vector3 normal)
{
	normal.normalize();

	return (pos - point).dotProduct(normal); 
}

bool PhysicsNode::onSameSide(Vector3 normal, Vector3 p, Vector3 pos, Vector3 test)
{
	Vector3 v0 = pos - p;
	Vector3 v1 = test - p;

	Vector3 res0 = v0.crossProduct(normal);
	Vector3 res1 = v1.crossProduct(normal);

	if ( res0.dotProduct(res1) >=0 ) return true;
	else return false;
}

}; // namespace Components
}; // namespace PE
