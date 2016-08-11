#ifndef __PYENGINE_2_0_ROOTPHYSICSNODE_H__
#define __PYENGINE_2_0_ROOTPHYSICSNODE_H__

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Utils/Array/Array.h"
#include "PrimeEngine/APIAbstraction/Effect/Effect.h"

// Sibling/Children includes
#include "PrimeEngine/PhysicsNode.h"

namespace PE{
namespace Components {

struct RootPhysicsNode : public PhysicsNode
{
	PE_DECLARE_CLASS(RootPhysicsNode)

	static void Construct (PE::GameContext &context, PE::MemoryArena arena);

	// Constructor -------------------------------------------------------------
	// same
	RootPhysicsNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself) : PhysicsNode(context, arena, hMyself)
	{
		m_components.reset(512);
	}

	virtual ~RootPhysicsNode(){}

	// component
	virtual void addDefaultComponents();
	// Individual events -------------------------------------------------------
	// this method will set up some global gpu constants like game time, frame time
	// it will also set light source gpu constants

	//PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PHYSICS_COLLISION);
	//virtual void do_PHYSICS_COLLISION(Events::Event *pEvt);

	//PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PHYSICS_GRAVITY);
	//virtual void do_PHYSICS_GRAVITY(Events::Event *pEvt);

	//void do_PHYSICS_COLLISION();
	//void do_PHYSICS_GRAVITY();

	static RootPhysicsNode *Instance() {return s_hInstance.getObject<RootPhysicsNode>();}
	static RootPhysicsNode *TitleInstance() {return s_hTitleInstance.getObject<RootPhysicsNode>();}
	static Handle InstanceHandle() {return s_hInstance;}
	static Handle TitleInstanceHandle() {return s_hTitleInstance;}
	static RootPhysicsNode *CurInstance() {return s_hCurInstance.getObject<RootPhysicsNode>();}
	static Handle CurInstanceHandle() {return s_hCurInstance;}
	static void SetTitleAsCurrent(){ s_hCurInstance = s_hTitleInstance; }
	static void SetGameAsCurrent() { s_hCurInstance = s_hInstance; }
	static bool TitleIsCurrent() { return s_hCurInstance == s_hTitleInstance;}

	static void SetInstance(Handle h){s_hInstance = h;}
private:
	static Handle s_hInstance;
	static Handle s_hTitleInstance;
	static Handle s_hCurInstance;

};

}; // namespace Components
}; // namespace PE

#endif // file guard