#include "RootPhysicsNode.h"

//#include "Light.h"
//#include "DrawList.h"

#include "PrimeEngine/APIAbstraction/Effect/EffectManager.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Render/ShaderActions/SetPerFrameConstantsShaderAction.h"
namespace PE {
namespace Components {

PE_IMPLEMENT_CLASS1(RootPhysicsNode, PhysicsNode);

Handle RootPhysicsNode::s_hTitleInstance;
Handle RootPhysicsNode::s_hInstance;
Handle RootPhysicsNode::s_hCurInstance;

void RootPhysicsNode::Construct(PE::GameContext &context, PE::MemoryArena arena)
{
	Handle h("ROOT_PHYSICS_NODE", sizeof(RootPhysicsNode));
	RootPhysicsNode *pRootPhysicsNode = new(h) RootPhysicsNode(context, arena, h);
	pRootPhysicsNode->addDefaultComponents();
	SetInstance(h);
	
	s_hTitleInstance = Handle("ROOT_PHYSICS_NODE", sizeof(RootPhysicsNode));
	RootPhysicsNode *pTitleRootPhysicsNode = new(s_hTitleInstance) RootPhysicsNode(context, arena, h);
	pTitleRootPhysicsNode->addDefaultComponents();
	
	SetTitleAsCurrent();
}

void RootPhysicsNode::addDefaultComponents()
{
	PhysicsNode::addDefaultComponents();

	//PE_REGISTER_EVENT_HANDLER(Events::Event_PHYSICS_COLLISION, RootPhysicsNode::do_PHYSICS_COLLISION);

	//PE_REGISTER_EVENT_HANDLER(Events::Event_PHYSICS_GRAVITY, RootPhysicsNode::do_PHYSICS_GRAVITY);
}

//void RootPhysicsNode::do_PHYSICS_COLLISION(Events::Event *pEvt)
//{
//	Events::Event_PHYSICS_COLLISION *pRealEvent = (Events::Event_PHYSICS_COLLISION *)(pEvt);
//	
//	OutputDebugStringA("RootPhysicsNode::do_PHYSICS_COLLISION/n");
//}
//
//void RootPhysicsNode::do_PHYSICS_GRAVITY(Events::Event *pEvt)
//{
//	Events::Event_PHYSICS_GRAVITY *pRealEvent = (Events::Event_PHYSICS_GRAVITY *)(pEvt);
//	
//	OutputDebugStringA("RootPhysicsNode::do_PHYSICS_GRAVITY/n");
//}

//void do_PHYSICS_COLLISION()
//{
//
//}
//
//void do_PHYSICS_GRAVITY()
//{
//
//}

}; // namespace Components
}; // namespace PE