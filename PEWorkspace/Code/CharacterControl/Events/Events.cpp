#include "PrimeEngine/PrimeEngineIncludes.h"
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "Events.h"

using namespace PE;
namespace CharacterControl {
namespace Events{

PE_IMPLEMENT_CLASS1(Event_CreateSoldierNPC, PE::Events::Event_CREATE_SKELETON);

void Event_CreateSoldierNPC::SetLuaFunctions(PE::Components::LuaEnvironment *pLuaEnv, lua_State *luaVM)
{
	static const struct luaL_Reg l_Event_CreateSoldierNPC[] = {
		{"Construct", l_Construct},
		{NULL, NULL} // sentinel
	};

	// register the functions in current lua table which is the table for Event_CreateSoldierNPC
	luaL_register(luaVM, 0, l_Event_CreateSoldierNPC);
}

int Event_CreateSoldierNPC::l_Construct(lua_State* luaVM)
{
    PE::Handle h("EVENT", sizeof(Event_CreateSoldierNPC));
	
	// get arguments from stack
	int numArgs, numArgsConst;
	numArgs = numArgsConst = 19;

	PE::GameContext *pContext = (PE::GameContext*)(lua_touserdata(luaVM, -numArgs--));

	// this function should only be called frm game thread, so we can use game thread thread owenrship mask
	Event_CreateSoldierNPC *pEvt = new(h) Event_CreateSoldierNPC(pContext->m_gameThreadThreadOwnershipMask);

	const char* name = lua_tostring(luaVM, -numArgs--);
	const char* package = lua_tostring(luaVM, -numArgs--);

	const char* gunMeshName = lua_tostring(luaVM, -numArgs--);
	const char* gunMeshPackage = lua_tostring(luaVM, -numArgs--);

	float positionFactor = 1.0f / 100.0f;

	Vector3 playerPos, u, v, n;
	playerPos.m_x = (float)lua_tonumber(luaVM, -numArgs--) * positionFactor;
	playerPos.m_y = (float)lua_tonumber(luaVM, -numArgs--) * positionFactor;
	playerPos.m_z = (float)lua_tonumber(luaVM, -numArgs--) * positionFactor;

	u.m_x = (float)lua_tonumber(luaVM, -numArgs--); u.m_y = (float)lua_tonumber(luaVM, -numArgs--); u.m_z = (float)lua_tonumber(luaVM, -numArgs--);
	v.m_x = (float)lua_tonumber(luaVM, -numArgs--); v.m_y = (float)lua_tonumber(luaVM, -numArgs--); v.m_z = (float)lua_tonumber(luaVM, -numArgs--);
	n.m_x = (float)lua_tonumber(luaVM, -numArgs--); n.m_y = (float)lua_tonumber(luaVM, -numArgs--); n.m_z = (float)lua_tonumber(luaVM, -numArgs--);

	pEvt->m_peuuid = LuaGlue::readPEUUID(luaVM, -numArgs--);

	const char* wayPointName = NULL;

	if (!lua_isnil(luaVM, -numArgs))
	{
		// have patrol waypoint name
		wayPointName = lua_tostring(luaVM, -numArgs--);
	}
	else
		// ignore
		numArgs--;


	// set data values before popping memory off stack
	StringOps::writeToString(name, pEvt->m_meshFilename, 255);
	StringOps::writeToString(package, pEvt->m_package, 255);

	StringOps::writeToString(gunMeshName, pEvt->m_gunMeshName, 64);
	StringOps::writeToString(gunMeshPackage, pEvt->m_gunMeshPackage, 64);
	StringOps::writeToString(wayPointName, pEvt->m_patrolWayPoint, 32);

	lua_pop(luaVM, numArgsConst); //Second arg is a count of how many to pop

	pEvt->hasCustomOrientation = true;
	pEvt->m_pos = playerPos;
	pEvt->m_u = u;
	pEvt->m_v = v;
	pEvt->m_n = n;

	LuaGlue::pushTableBuiltFromHandle(luaVM, h); 

	return 1;
}

PE_IMPLEMENT_CLASS1(Event_MoveTank_C_to_S, Event);

Event_MoveTank_C_to_S::Event_MoveTank_C_to_S(PE::GameContext &context)
: Networkable(context, this)
{

}

void *Event_MoveTank_C_to_S::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_MoveTank_C_to_S *pEvt = new (arena) Event_MoveTank_C_to_S(context);
	return pEvt;
}

int Event_MoveTank_C_to_S::packCreationData(char *pDataStream)
{
	return PE::Components::StreamManager::WriteMatrix4x4(m_transform, pDataStream);
}

int Event_MoveTank_C_to_S::constructFromStream(char *pDataStream)
{
	int read = 0;
	read += PE::Components::StreamManager::ReadMatrix4x4(&pDataStream[read], m_transform);
	return read;
}

PE_IMPLEMENT_CLASS1(Event_MoveCar_C_to_S, Event);

Event_MoveCar_C_to_S::Event_MoveCar_C_to_S(PE::GameContext &context) : Networkable(context, this)
{
	// constructor
}

int Event_MoveCar_C_to_S::packCreationData(char *pDataStream)
{
	// currently we imitate the MoveTank event
	int size = 0;
	size += PE::Components::StreamManager::WriteMatrix4x4(m_matrix, pDataStream);
	//size += PE::Components::StreamManager::WriteFloat32(m_velocity, pDataStream);
	//size += PE::Components::StreamManager::WriteFloat32(m_rotation, pDataStream);
	return size;
	//
}

int Event_MoveCar_C_to_S::constructFromStream(char *pDataStream)
{
	int read = 0;
	// currently we imitate the MoveTank event
	read += PE::Components::StreamManager::ReadMatrix4x4(&pDataStream[read], m_matrix);
	//read += PE::Components::StreamManager::ReadFloat32(pDataStream, m_velocity);
	//read += PE::Components::StreamManager::ReadFloat32(pDataStream, m_rotation);
	return read;
}

void* Event_MoveCar_C_to_S::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_MoveCar_C_to_S *pEvt = new (arena)Event_MoveCar_C_to_S(context);
	return pEvt;
}

/* Event_MoveTank_S_to_C Definition */
PE_IMPLEMENT_CLASS1(Event_MoveTank_S_to_C, Event_MoveTank_C_to_S);

Event_MoveTank_S_to_C::Event_MoveTank_S_to_C(PE::GameContext &context)
: Event_MoveTank_C_to_S(context)
{

}

void *Event_MoveTank_S_to_C::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_MoveTank_S_to_C *pEvt = new (arena) Event_MoveTank_S_to_C(context);
	return pEvt;
}

int Event_MoveTank_S_to_C::packCreationData(char *pDataStream)
{
	int size = 0;
	size += Event_MoveTank_C_to_S::packCreationData(&pDataStream[size]);
	size += PE::Components::StreamManager::WriteInt32(m_clientTankId, &pDataStream[size]);
	return size;
}

int Event_MoveTank_S_to_C::constructFromStream(char *pDataStream)
{
	int read = 0;
	read += Event_MoveTank_C_to_S::constructFromStream(&pDataStream[read]);
	read += PE::Components::StreamManager::ReadInt32(&pDataStream[read], m_clientTankId);
	return read;
}

/* Event_MoveCar_S_to_C implementation */
PE_IMPLEMENT_CLASS1(Event_MoveCar_S_to_C, Event_MoveCar_C_to_S); // Mark

Event_MoveCar_S_to_C::Event_MoveCar_S_to_C(PE::GameContext &context)
	: Event_MoveCar_C_to_S(context)
{

}

void *Event_MoveCar_S_to_C::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_MoveCar_S_to_C *pEvt = new (arena)Event_MoveCar_S_to_C(context);
	return pEvt;
}

int Event_MoveCar_S_to_C::packCreationData(char *pDataStream)
{
	int size = 0;
	size += Event_MoveCar_C_to_S::packCreationData(&pDataStream[size]); // first matrix and then id
	size += PE::Components::StreamManager::WriteInt32(m_clientTankId, &pDataStream[size]);
	return size;
}

int Event_MoveCar_S_to_C::constructFromStream(char *pDataStream)
{
	int read = 0;
	read += Event_MoveCar_C_to_S::constructFromStream(&pDataStream[read]); // first read matrix and then id
	read += PE::Components::StreamManager::ReadInt32(&pDataStream[read], m_clientTankId);
	return read;
}

// Time Sync Implementation

PE_IMPLEMENT_CLASS1(Event_PingSync_C_to_S, Event);

Event_PingSync_C_to_S::Event_PingSync_C_to_S(PE::GameContext &context) : Networkable(context, this){

}

void *Event_PingSync_C_to_S::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_PingSync_C_to_S *pEvt = new (arena)Event_PingSync_C_to_S(context);
	return pEvt;
}

int Event_PingSync_C_to_S::packCreationData(char *pDataStream){
	int size = 0;
	size += PE::Components::StreamManager::WriteFloat32(m_time, pDataStream);
	return size;
}

int Event_PingSync_C_to_S::constructFromStream(char *pDataStream){
	int read = 0;
	// currently we imitate the MoveTank event
	read += PE::Components::StreamManager::ReadFloat32(&pDataStream[read], m_time);
	return read;
}

PE_IMPLEMENT_CLASS1(Event_PingSync_S_to_C, Event);

Event_PingSync_S_to_C::Event_PingSync_S_to_C(PE::GameContext &context) : Networkable(context, this){

}

void *Event_PingSync_S_to_C::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_PingSync_S_to_C *pEvt = new (arena)Event_PingSync_S_to_C(context);
	return pEvt;
}

int Event_PingSync_S_to_C::packCreationData(char *pDataStream){
	int size = 0;
	size += PE::Components::StreamManager::WriteFloat32(m_time, pDataStream);
	return size;
}

int Event_PingSync_S_to_C::constructFromStream(char *pDataStream){
	int read = 0;
	// currently we imitate the MoveTank event
	read += PE::Components::StreamManager::ReadFloat32(&pDataStream[read], m_time);
	return read;
}

// ping ack implementation
PE_IMPLEMENT_CLASS1(Event_PingAck_C_to_S, Event);

Event_PingAck_C_to_S::Event_PingAck_C_to_S(PE::GameContext &context) : Networkable(context, this){

}

void *Event_PingAck_C_to_S::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_PingAck_C_to_S *pEvt = new (arena)Event_PingAck_C_to_S(context);
	return pEvt;
}

int Event_PingAck_C_to_S::packCreationData(char *pDataStream){
	int size = 0;
	size += PE::Components::StreamManager::WriteFloat32(m_pingTime, pDataStream);
	return size;
}

int Event_PingAck_C_to_S::constructFromStream(char *pDataStream){
	int read = 0;
	// currently we imitate the MoveTank event
	read += PE::Components::StreamManager::ReadFloat32(&pDataStream[read], m_pingTime);
	return read;
}

// server time sync implementation
PE_IMPLEMENT_CLASS1(Event_TimeSync_S_to_C, Event);

Event_TimeSync_S_to_C::Event_TimeSync_S_to_C(PE::GameContext &context) : Networkable(context, this){

}

void *Event_TimeSync_S_to_C::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_TimeSync_S_to_C *pEvt = new (arena)Event_TimeSync_S_to_C(context);
	return pEvt;
}

int Event_TimeSync_S_to_C::packCreationData(char *pDataStream){
	int size = 0;
	size += PE::Components::StreamManager::WriteFloat32(m_currentServerTime, pDataStream);
	return size;
}

int Event_TimeSync_S_to_C::constructFromStream(char *pDataStream){
	int read = 0;
	// currently we imitate the MoveTank event
	read += PE::Components::StreamManager::ReadFloat32(&pDataStream[read], m_currentServerTime);
	return read;
}

// Client side physics process
PE_IMPLEMENT_CLASS1(Event_PhysicsProcess_C_to_S, Event);

Event_PhysicsProcess_C_to_S::Event_PhysicsProcess_C_to_S(PE::GameContext &context) : Networkable(context, this){

}

void *Event_PhysicsProcess_C_to_S::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_PhysicsProcess_C_to_S *pEvt = new (arena)Event_PhysicsProcess_C_to_S(context);
	return pEvt;
}

int Event_PhysicsProcess_C_to_S::packCreationData(char *pDataStream){
	int size = 0;
	//size += PE::Components::StreamManager::WriteFloat32(m_time, pDataStream);
	//size += PE::Components::StreamManager::WriteFloat32(m_velocity, pDataStream);
	//size += PE::Components::StreamManager::WriteFloat32(m_rotation, pDataStream);
	size += PE::Components::StreamManager::WriteMatrix4x4(m_dataMatrix, pDataStream);
	return size;
}

int Event_PhysicsProcess_C_to_S::constructFromStream(char *pDataStream){
	int read = 0;
	// currently we imitate the MoveTank event
	//read += PE::Components::StreamManager::ReadFloat32(&pDataStream[read], m_time);
	//read += PE::Components::StreamManager::ReadFloat32(&pDataStream[read], m_velocity);
	//read += PE::Components::StreamManager::ReadFloat32(&pDataStream[read], m_rotation);
	read += PE::Components::StreamManager::ReadMatrix4x4(&pDataStream[read], m_dataMatrix);
	return read;
}

PE_IMPLEMENT_CLASS1(Event_PhysicsProcess_S_to_C, Event);
Event_PhysicsProcess_S_to_C::Event_PhysicsProcess_S_to_C(PE::GameContext &context) : Event_PhysicsProcess_C_to_S(context){

}

void *Event_PhysicsProcess_S_to_C::FactoryConstruct(PE::GameContext& context, PE::MemoryArena arena)
{
	Event_PhysicsProcess_S_to_C *pEvt = new (arena)Event_PhysicsProcess_S_to_C(context);
	return pEvt;
}

int Event_PhysicsProcess_S_to_C::packCreationData(char *pDataStream){
	int size = 0;
	size += Event_PhysicsProcess_C_to_S::packCreationData(&pDataStream[size]); // first matrix and then id
	size += PE::Components::StreamManager::WriteInt32(m_clientTankId, &pDataStream[size]);
	return size;
}

int Event_PhysicsProcess_S_to_C::constructFromStream(char *pDataStream){
	int read = 0;
	read += Event_PhysicsProcess_C_to_S::constructFromStream(&pDataStream[read]); // first read matrix and then id
	read += PE::Components::StreamManager::ReadInt32(&pDataStream[read], m_clientTankId);
	return read;
}





PE_IMPLEMENT_CLASS1(Event_Tank_Throttle, Event);

PE_IMPLEMENT_CLASS1(Event_Tank_Turn, Event);

PE_IMPLEMENT_CLASS1(Event_Stop_Timer, Event);

PE_IMPLEMENT_CLASS1(Event_Reload_Car, Event);


};
};

