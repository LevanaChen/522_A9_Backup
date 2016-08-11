#ifndef _CHARACTER_CONTROL_GAME_SERVER_OBJ_MANAGER_ADDON_
#define _CHARACTER_CONTROL_GAME_SERVER_OBJ_MANAGER_ADDON_

#include "GameObjectMangerAddon.h"
#include "Events/Events.h"
#include "WayPoint.h"
#include "../PrimeEngine/PhysicsNode.h"

namespace CharacterControl
{
namespace Components
{

// This struct will be added to GameObjectManager as component
// as a result events sent to game object manager will be able to get to this component
// so we can create custom game objects through this class
struct ServerGameObjectManagerAddon : public GameObjectManagerAddon
{
	PE_DECLARE_SINGLETON_CLASS(ServerGameObjectManagerAddon); // creates a static handle and GteInstance*() methods. still need to create construct

	ServerGameObjectManagerAddon(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself) : GameObjectManagerAddon(context, arena, hMyself)
	{}

	// sub-component and event registration
	virtual void addDefaultComponents() ;

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_MoveTank);
	virtual void do_MoveTank(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_MoveCar);
	virtual void do_MoveCar(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PingSync);
	virtual void do_PingSync(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PingAck);
	virtual void do_PingAck(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PhysicsProcess);
	virtual void do_PhysicsProcess(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_UPDATE);
	virtual void do_UPDATE(PE::Events::Event *pEvt);

	//////////////////////////////////////////////////////////////////////////
	// Game Specific functionality
	//////////////////////////////////////////////////////////////////////////
	//
	//PE::Components::PhysicsNode m_simulatedPhysicsNode[2];
	Matrix4x4 m_simulatedBase[2];
	PE::Components::ServerPhysicsNode m_physicsNode[2];
	float m_pingTime[2];
	float m_currentTime;
	float m_timeSyncInterval;
	float m_timeSyncInterval2;
};


}
}

#endif
