#ifndef _CHARACTER_CONTROL_CLIENT_GAME_OBJ_MANAGER_ADDON_
#define _CHARACTER_CONTROL_CLIENT_GAME_OBJ_MANAGER_ADDON_

#include "GameObjectMangerAddon.h"
#include "Events/Events.h"
#include "PrimeEngine/NavMeshManager.h"
#include "WayPoint.h"
#include "./Tank/ClientTank.h"

namespace CharacterControl
{
namespace Components
{
	struct Record{

		Record(){
			m_isCounting = false;
			m_id = -1;
			m_time = 0.0f;
			m_passedRecord[0] = 0.0f;
			m_passedRecord[1] = 0.0f;
			m_passedRecord[2] = 0.0f;
		}

		bool m_isCounting;
		int m_id;
		float m_time;
		float m_lastRecord;
		float m_passedRecord[3];
	};

// This struct will be added to GameObjectManager as component
// as a result events sent to game object manager will be able to get to this component
// so we can create custom game objects through this class
struct ClientGameObjectManagerAddon : public GameObjectManagerAddon
{
	PE_DECLARE_CLASS(ClientGameObjectManagerAddon); // creates a static handle and GteInstance*() methods. still need to create construct

	ClientGameObjectManagerAddon(PE::GameContext &context, PE::MemoryArena arena, PE::Handle hMyself) : GameObjectManagerAddon(context, arena, hMyself)
	{}

	// sub-component and event registration
	virtual void addDefaultComponents() ;

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_CreateSoldierNPC);
	virtual void do_CreateSoldierNPC(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_CREATE_WAYPOINT);
	virtual void do_CREATE_WAYPOINT(PE::Events::Event *pEvt);

	//will activate tank when local client is connected
	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_SERVER_CLIENT_CONNECTION_ACK);
	virtual void do_SERVER_CLIENT_CONNECTION_ACK(PE::Events::Event *pEvt);

	// sent from server, sets position of non-local client tanks
	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_MoveTank);
	virtual void do_MoveTank(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_MoveCar);
	virtual void do_MoveCar(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PingSync);
	virtual void do_PingSync(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_TimeSync);
	virtual void do_TimeSync(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PhysicsProcess);
	virtual void do_PhysicsProcess(PE::Events::Event *pEvt);

	PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_UPDATE);
	virtual void do_UPDATE(PE::Events::Event *pEvt);


	// no need to implement this as eent since tank creation will be hardcoded
	void createTank(int index, int &threadOwnershipMask);

	void createSoldierNPC(Vector3 pos, int &threadOwnershipMask);
	void createSoldierNPC(Events::Event_CreateSoldierNPC *pTrueEvent);


	//////////////////////////////////////////////////////////////////////////
	// Game Specific functionality
	//////////////////////////////////////////////////////////////////////////
	//
	// waypoint search
	WayPoint *getWayPoint(const char *name);
	TankController *getTankController(const int index);

	Record m_records[2];
	int m_activatedCarID; // record the id for the activated car
	float m_currentTime;
	float m_networkPing;
	float m_pingRefreshInterval;
	bool m_waitingForPing;
	float m_u;
	float m_v;
};

}
}

#endif
