#ifndef _CHARACTER_CONTROL_EVENTS_
#define _CHARACTER_CONTROL_EVENTS_

#include "PrimeEngine/Events/StandardEvents.h"

namespace CharacterControl
{
namespace Events
{
struct Event_CreateSoldierNPC : public PE::Events::Event_CREATE_MESH
{
	PE_DECLARE_CLASS(Event_CreateSoldierNPC);

	Event_CreateSoldierNPC(int &threadOwnershipMask): PE::Events::Event_CREATE_MESH(threadOwnershipMask){}
	// override SetLuaFunctions() since we are adding custom Lua interface
	static void SetLuaFunctions(PE::Components::LuaEnvironment *pLuaEnv, lua_State *luaVM);

	// Lua interface prefixed with l_
	static int l_Construct(lua_State* luaVM);

	int m_npcType;
	char m_gunMeshName[64];
	char m_gunMeshPackage[64];
	char m_patrolWayPoint[32];
};

struct Event_MoveTank_C_to_S : public PE::Events::Event, public PE::Networkable
{
	PE_DECLARE_CLASS(Event_MoveTank_C_to_S);
	PE_DECLARE_NETWORKABLE_CLASS

	Event_MoveTank_C_to_S(PE::GameContext &context);
	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);


	Matrix4x4 m_transform;
};

struct Event_MoveCar_C_to_S : public PE::Events::Event, public PE::Networkable
{
	PE_DECLARE_CLASS(Event_MoveCar_C_to_S);
	PE_DECLARE_NETWORKABLE_CLASS

	Event_MoveCar_C_to_S(PE::GameContext &context);

	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	// for temp use;
	Matrix4x4 m_matrix;

};


struct Event_MoveTank_S_to_C : public Event_MoveTank_C_to_S
{
	PE_DECLARE_CLASS(Event_MoveTank_S_to_C);
	PE_DECLARE_NETWORKABLE_CLASS

	Event_MoveTank_S_to_C(PE::GameContext &context);
	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	int m_clientTankId;
};

struct Event_MoveCar_S_to_C : public Event_MoveCar_C_to_S
{
	PE_DECLARE_CLASS(Event_MoveCar_S_to_C);
	PE_DECLARE_NETWORKABLE_CLASS

	Event_MoveCar_S_to_C(PE::GameContext &context);
	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	int m_clientTankId;
};

// Client Time Sync
struct Event_PingSync_C_to_S : public PE::Events::Event, public PE::Networkable
{
	PE_DECLARE_CLASS(Event_PingSync_C_to_S);
	PE_DECLARE_NETWORKABLE_CLASS

	Event_PingSync_C_to_S(PE::GameContext &context);

	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	// for temp use;
	float m_time;
};

struct Event_PingSync_S_to_C : public PE::Events::Event, public PE::Networkable
{
	PE_DECLARE_CLASS(Event_PingSync_S_to_C);
	PE_DECLARE_NETWORKABLE_CLASS

		Event_PingSync_S_to_C(PE::GameContext &context);

	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	// for temp use;
	float m_time;
};

// Client Ping Ack
struct Event_PingAck_C_to_S : public PE::Events::Event, public PE::Networkable
{
	PE_DECLARE_CLASS(Event_PingAck_C_to_S);
	PE_DECLARE_NETWORKABLE_CLASS

		Event_PingAck_C_to_S(PE::GameContext &context);

	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	// for temp use;
	float m_pingTime;
};

// Server Time Sync Package
struct Event_TimeSync_S_to_C : public PE::Events::Event, public PE::Networkable
{
	PE_DECLARE_CLASS(Event_TimeSync_S_to_C);
	PE_DECLARE_NETWORKABLE_CLASS
		Event_TimeSync_S_to_C(PE::GameContext &context);
	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	float m_currentServerTime;
};


// Client Side Physics Handle Event
struct Event_PhysicsProcess_C_to_S : public PE::Events::Event, public PE::Networkable
{
	PE_DECLARE_CLASS(Event_PhysicsProcess_C_to_S);
	PE_DECLARE_NETWORKABLE_CLASS
		Event_PhysicsProcess_C_to_S(PE::GameContext &context);
	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	Matrix4x4 m_dataMatrix;
};

struct Event_PhysicsProcess_S_to_C : public Event_PhysicsProcess_C_to_S
{
	PE_DECLARE_CLASS(Event_PhysicsProcess_S_to_C);
	PE_DECLARE_NETWORKABLE_CLASS
		Event_PhysicsProcess_S_to_C(PE::GameContext &context);
	// Netoworkable:
	virtual int packCreationData(char *pDataStream);
	virtual int constructFromStream(char *pDataStream);


	// Factory function used by network
	static void *FactoryConstruct(PE::GameContext&, PE::MemoryArena);

	int m_clientTankId;
	
};



// tank input controls

struct Event_Tank_Throttle : public PE::Events::Event {
	PE_DECLARE_CLASS(Event_Tank_Throttle);

	Event_Tank_Throttle(){}
	virtual ~Event_Tank_Throttle(){}

	Vector3 m_relativeMove;
};

struct Event_Tank_Turn : public PE::Events::Event {
	PE_DECLARE_CLASS(Event_Tank_Turn);

	Event_Tank_Turn(){}
	virtual ~Event_Tank_Turn(){}

	Vector3 m_relativeRotate;
};

struct Event_Stop_Timer : public PE::Events::Event {
	PE_DECLARE_CLASS(Event_Stop_Timer);

	Event_Stop_Timer(){}
	virtual ~Event_Stop_Timer(){}
};

struct Event_Reload_Car : public PE::Events::Event {
	PE_DECLARE_CLASS(Event_Reload_Car);

	Event_Reload_Car(){}
	virtual ~Event_Reload_Car(){}
};

}; // namespace Events
}; // namespace CharacterControl

#endif
