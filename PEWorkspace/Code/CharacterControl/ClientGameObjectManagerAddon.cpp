#include "ClientGameObjectManagerAddon.h"

#include "PrimeEngine/PrimeEngineIncludes.h"

#include "Characters/SoldierNPC.h"
#include "WayPoint.h"
#include "Tank/ClientTank.h"

// For Assignment 8 : Add including PhysicsNode in ClientGameObjectManagerAddon
#include "PrimeEngine/PhysicsNode.h"
#include "PrimeEngine/RootPhysicsNode.h"


// For Assignment 8 : Implement car movement
static float unit_accerleration = 60.0f;	// Unit accerleration per second for throttle - fps + resistence value
static float unit_rotation = 0.05f;			// Unit rotation per second for rotation
static float max_velocity = 3.0f;			// Car's maximal velocity
static float min_velocity = 0.005f;			// Car's minimal velociry
static float max_angle = 0.5f;				// Car's rotation maximal angle
static float rr_grass = 0.35f;				// Rolling resistence in grass
static float rr_road = 0.1f;				// Rolling resistence in road
static float max_depth = 3.0f;				// The maximal depth that car could reach by falling down

using namespace PE::Components;
using namespace PE::Events;
using namespace CharacterControl::Events;
using namespace CharacterControl::Components;

namespace CharacterControl{
namespace Components
{
PE_IMPLEMENT_CLASS1(ClientGameObjectManagerAddon, Component); // creates a static handle and GteInstance*() methods. still need to create construct

void ClientGameObjectManagerAddon::addDefaultComponents()
{
	GameObjectManagerAddon::addDefaultComponents();

	PE_REGISTER_EVENT_HANDLER(Event_CreateSoldierNPC, ClientGameObjectManagerAddon::do_CreateSoldierNPC);
	PE_REGISTER_EVENT_HANDLER(Event_CREATE_WAYPOINT, ClientGameObjectManagerAddon::do_CREATE_WAYPOINT);

	// note this component (game obj addon) is added to game object manager after network manager, so network manager will process this event first
	PE_REGISTER_EVENT_HANDLER(PE::Events::Event_SERVER_CLIENT_CONNECTION_ACK, ClientGameObjectManagerAddon::do_SERVER_CLIENT_CONNECTION_ACK);

	PE_REGISTER_EVENT_HANDLER(Event_MoveTank_S_to_C, ClientGameObjectManagerAddon::do_MoveTank);
	PE_REGISTER_EVENT_HANDLER(Event_MoveCar_S_to_C, ClientGameObjectManagerAddon::do_MoveCar);
	PE_REGISTER_EVENT_HANDLER(Event_PingSync_S_to_C, ClientGameObjectManagerAddon::do_PingSync);
	PE_REGISTER_EVENT_HANDLER(Event_TimeSync_S_to_C, ClientGameObjectManagerAddon::do_TimeSync);
	PE_REGISTER_EVENT_HANDLER(Event_PhysicsProcess_S_to_C, ClientGameObjectManagerAddon::do_PhysicsProcess);
	PE_REGISTER_EVENT_HANDLER(Event_UPDATE, ClientGameObjectManagerAddon::do_UPDATE);

	m_currentTime = 0;
	m_networkPing = 0;
	m_pingRefreshInterval = 0.0f;
	m_u = 0.5f;
	m_v = 0.5f;
}

void ClientGameObjectManagerAddon::do_UPDATE(PE::Events::Event *pEvt)
{
	Event_UPDATE *pRealEvt = (Event_UPDATE*)(pEvt);
	// ready to refresh all the record data here
	for (int i = 0; i < 2; i++){
		if (m_records[i].m_isCounting){
			m_records[i].m_time += pRealEvt->m_frameTime;
		}
		else{
			// is not counting anymore
			if (m_records[i].m_time != 0.0f){
				// Keep the record into the array (Currently just replace the first array)
				m_records[i].m_lastRecord = m_records[i].m_time;
				m_records[i].m_time = 0.0f;
				PEINFO("Your last record is: %f", m_records[i].m_lastRecord);
				// Write this record into the passed record
				for (int j = 0; j < 3; j++){
					if (m_records[i].m_passedRecord[j] == 0.0f){
						// this is an empty slot, directly write into this slot
						m_records[i].m_passedRecord[j] = m_records[i].m_lastRecord;
						break;
					}
					else{
						if (m_records[i].m_passedRecord[j] > m_records[i].m_lastRecord){
							float originalRecord = m_records[i].m_passedRecord[j];
							float tempRecord;
							m_records[i].m_passedRecord[j] = m_records[i].m_lastRecord;
							for (int k = j; k < 3; k++){
								if (k == j){
									continue;
								}
								else{
									if (k == 2){
										// the last one
										m_records[i].m_passedRecord[k] = originalRecord;
									}
									else{
										tempRecord = m_records[i].m_passedRecord[k]; // copy the current record in this slot into the temp buffer
										m_records[i].m_passedRecord[k] = originalRecord; // assgin the origianl record into this slot
										originalRecord = tempRecord; // assign the current record in the temp buffer to the original record
									}
								}
							}
							break; // jump out of the loop
						}
					}
				}
			}
		}
	}

	// update the current time
	//now should be synced by the server
	m_currentTime += pRealEvt->m_frameTime;
	// PEINFO("Current game time is: %f", m_currentTime);
	 m_pingRefreshInterval -= pRealEvt->m_frameTime;
	 if (m_pingRefreshInterval >= 0.0f){
		 // not the time to refresh the ping
		 return;
	 }
	 else{
		 // refresh
		 m_pingRefreshInterval = 5.0f;
	 }

	// send out the network ping package
	if (!m_waitingForPing){
		ClientNetworkManager *pNetworkManager = (ClientNetworkManager *)(m_pContext->getNetworkManager());
		if (pNetworkManager->m_state != ClientNetworkManager::ClientState_Disconnected){
			CharacterControl::Events::Event_PingSync_C_to_S evt(*m_pContext);
			evt.m_time = m_currentTime;
			pNetworkManager->getNetworkContext().getEventManager()->scheduleEvent(&evt, m_pContext->getGameObjectManager(), true);
			m_waitingForPing = true;
		}
	}
}

void ClientGameObjectManagerAddon::do_CreateSoldierNPC(PE::Events::Event *pEvt)
{
	assert(pEvt->isInstanceOf<Event_CreateSoldierNPC>());

	Event_CreateSoldierNPC *pTrueEvent = (Event_CreateSoldierNPC*)(pEvt);

	createSoldierNPC(pTrueEvent);
}

void ClientGameObjectManagerAddon::createSoldierNPC(Vector3 pos, int &threadOwnershipMask)
{
	Event_CreateSoldierNPC evt(threadOwnershipMask);
	evt.m_pos = pos;
	evt.m_u = Vector3(1.0f, 0, 0);
	evt.m_v = Vector3(0, 1.0f, 0);
	evt.m_n = Vector3(0, 0, 1.0f);
	
	StringOps::writeToString( "SoldierTransform.mesha", evt.m_meshFilename, 255);
	StringOps::writeToString( "Soldier", evt.m_package, 255);
	StringOps::writeToString( "mg34.x_mg34main_mesh.mesha", evt.m_gunMeshName, 64);
	StringOps::writeToString( "CharacterControl", evt.m_gunMeshPackage, 64);
	StringOps::writeToString( "", evt.m_patrolWayPoint, 32);
	createSoldierNPC(&evt);
}

void ClientGameObjectManagerAddon::createSoldierNPC(Event_CreateSoldierNPC *pTrueEvent)
{
	PEINFO("CharacterControl: GameObjectManagerAddon: Creating CreateSoldierNPC\n");

	PE::Handle hSoldierNPC("SoldierNPC", sizeof(SoldierNPC));
	SoldierNPC *pSoldierNPC = new(hSoldierNPC) SoldierNPC(*m_pContext, m_arena, hSoldierNPC, pTrueEvent);
	pSoldierNPC->addDefaultComponents();

	// add the soldier as component to the ObjecManagerComponentAddon
	// all objects of this demo live in the ObjecManagerComponentAddon
	addComponent(hSoldierNPC);
}

void ClientGameObjectManagerAddon::do_CREATE_WAYPOINT(PE::Events::Event *pEvt)
{
	PEINFO("GameObjectManagerAddon::do_CREATE_WAYPOINT()\n");

	assert(pEvt->isInstanceOf<Event_CREATE_WAYPOINT>());

	Event_CREATE_WAYPOINT *pTrueEvent = (Event_CREATE_WAYPOINT*)(pEvt);

	PE::Handle hWayPoint("WayPoint", sizeof(WayPoint));
	WayPoint *pWayPoint = new(hWayPoint) WayPoint(*m_pContext, m_arena, hWayPoint, pTrueEvent);
	pWayPoint->addDefaultComponents();

	addComponent(hWayPoint);
}

WayPoint *ClientGameObjectManagerAddon::getWayPoint(const char *name)
{
	PE::Handle *pHC = m_components.getFirstPtr();

	for (PrimitiveTypes::UInt32 i = 0; i < m_components.m_size; i++, pHC++) // fast array traversal (increasing ptr)
	{
		Component *pC = (*pHC).getObject<Component>();

		if (pC->isInstanceOf<WayPoint>())
		{
			WayPoint *pWP = (WayPoint *)(pC);
			if (StringOps::strcmp(pWP->m_name, name) == 0)
			{
				// equal strings, found our waypoint
				return pWP;
			}
		}
	}
	return NULL;
}

TankController *ClientGameObjectManagerAddon::getTankController(const int index)
{
	PE::Handle *pHC = m_components.getFirstPtr();

	int j = 0;

	for (PrimitiveTypes::UInt32 i = 0; i < m_components.m_size; i++, pHC++) // fast array traversal (increasing ptr)
	{
		Component *pC = (*pHC).getObject<Component>();

		if (pC->isInstanceOf<TankController>())
		{
			TankController *pTC = (TankController *)(pC);
			if (j == index){
				return pTC;
			}
			else{
				j++;
			}
		}
	}
	return NULL;
}


void ClientGameObjectManagerAddon::createTank(int index, int &threadOwnershipMask)
{

	//create hierarchy:
	//scene root
	//  scene node // tracks position/orientation
	//    Tank

	//game object manager
	//  TankController
	//    scene node
	
	PE::Handle hMeshInstance("MeshInstance", sizeof(MeshInstance));
	MeshInstance *pMeshInstance = new(hMeshInstance) MeshInstance(*m_pContext, m_arena, hMeshInstance);

	pMeshInstance->addDefaultComponents();
	pMeshInstance->initFromFile("car.mesha", "CarRacingDemo", threadOwnershipMask);

	// need to create a scene node for this mesh
	PE::Handle hSN("SCENE_NODE", sizeof(SceneNode));
	SceneNode *pSN = new(hSN) SceneNode(*m_pContext, m_arena, hSN);
	pSN->addDefaultComponents();

	Vector3 spawnPos(12 + 0.5 * index, 0.069f , 0.0f);
	pSN->m_base.setPos(spawnPos);
	
	pSN->addComponent(hMeshInstance);

	// For Assignment 8 : Add physics node into tank's scene node
	PE::Handle hPN("PHYSICS_NODE", sizeof(PhysicsNode));
	PhysicsNode *pMainPN = new(hPN) PhysicsNode(*m_pContext, m_arena, hPN);

	pMainPN->do_INITIALIZATION();
	pMainPN->m_type = PhysicsNode::CAR;
	pMainPN->m_state = PhysicsNode::STOP;
	pMainPN->m_id = index;

	// For Assignment 8 : Get MeshCPU
	Mesh *pMesh = pMeshInstance->getFirstParentByType<Mesh>().getObject<Mesh>();

	pMainPN->AABB_min = pMesh->AABB_min * 0.01f;
	pMainPN->AABB_max = pMesh->AABB_max * 0.01f;

	// For Assignment 9 : Collision Detection
	pMainPN->length = pMainPN->AABB_max.getZ() - pMainPN->AABB_min.getZ();
	pMainPN->width = pMainPN->AABB_max.getX() - pMainPN->AABB_min.getX();

	pMainPN->m_base = pSN->m_base;
	pMainPN->m_pos = pSN->m_base.getPos();

	pSN->pPhysicsNode = pMainPN;

	// add physics node as the child component
	pSN->addComponent(hPN);

	RootPhysicsNode::Instance()->addComponent(hPN);

	RootSceneNode::Instance()->addComponent(hSN);

	// now add game objects

	PE::Handle hSquareController("DbgSquareController0", sizeof(TankController));
	TankController *pSquareController = new(hSquareController) TankController(*m_pContext, m_arena, hSquareController, 0.05f, spawnPos,  0.05f);
	pSquareController->addDefaultComponents();

	// For Assignment 8 : recording tank ID
	pSquareController->m_id = index;

	addComponent(hSquareController);

	// add the same scene node to square controller
	static int alllowedEventsToPropagate[] = {0}; // we will pass empty array as allowed events to propagate so that when we add
	// scene node to the square controller, the square controller doesnt try to handle scene node's events
	// because scene node handles events trhough scene graph, and is child of sqaure controller just for referencing purposes
	pSquareController->addComponent(hSN, &alllowedEventsToPropagate[0]);
}


void ClientGameObjectManagerAddon::do_SERVER_CLIENT_CONNECTION_ACK(PE::Events::Event *pEvt)
{
	Event_SERVER_CLIENT_CONNECTION_ACK *pRealEvt = (Event_SERVER_CLIENT_CONNECTION_ACK *)(pEvt);
	PE::Handle *pHC = m_components.getFirstPtr();

	int itc = 0;
	for (PrimitiveTypes::UInt32 i = 0; i < m_components.m_size; i++, pHC++) // fast array traversal (increasing ptr)
	{
		Component *pC = (*pHC).getObject<Component>();

		if (pC->isInstanceOf<TankController>())
		{
			if (itc == pRealEvt->m_clientId) //activate tank controller for local client based on local clients id
			{
				TankController *pTK = (TankController *)(pC);
				pTK->activate();
				break;
			}
			++itc;
		}
	}
}

void ClientGameObjectManagerAddon::do_MoveTank(PE::Events::Event *pEvt)
{
	assert(pEvt->isInstanceOf<Event_MoveTank_S_to_C>());

	Event_MoveTank_S_to_C *pTrueEvent = (Event_MoveTank_S_to_C*)(pEvt);

	PE::Handle *pHC = m_components.getFirstPtr();

	//PEINFO("Received Position change");

	int itc = 0;
	for (PrimitiveTypes::UInt32 i = 0; i < m_components.m_size; i++, pHC++) // fast array traversal (increasing ptr)
	{
		Component *pC = (*pHC).getObject<Component>();

		if (pC->isInstanceOf<TankController>())
		{
			if (itc == pTrueEvent->m_clientTankId) //activate tank controller for local client based on local clients id
			{
				//PEINFO("Received Position Sync Signal");
				TankController *pTK = (TankController *)(pC);
				SceneNode *pFirstSN = pTK->getFirstComponent<SceneNode>();

				pFirstSN->m_base.setPos(pTrueEvent->m_transform.getPos());
				pFirstSN->m_base.setU(pTrueEvent->m_transform.getU());
				pFirstSN->m_base.setV(pTrueEvent->m_transform.getV());
				pFirstSN->m_base.setN(pTrueEvent->m_transform.getN());
				break;
			}
			++itc;
		}
	}
}

void ClientGameObjectManagerAddon::do_MoveCar(PE::Events::Event *pEvt)
{

	Event_MoveCar_S_to_C *pTrueEvent = (Event_MoveCar_S_to_C*)(pEvt);

	PE::Handle *pHC = m_components.getFirstPtr();

	int itc = 0;
	for (PrimitiveTypes::UInt32 i = 0; i < m_components.m_size; i++, pHC++) // fast array traversal (increasing ptr)
	{
		Component *pC = (*pHC).getObject<Component>();

		if (pC->isInstanceOf<TankController>())
		{
			if (itc == pTrueEvent->m_clientTankId) //activate tank controller for local client based on local clients id
			{
				TankController *pTK = (TankController *)(pC);
				SceneNode *pFirstSN = pTK->getFirstComponent<SceneNode>();
				PhysicsNode *pMainPN = pFirstSN->pPhysicsNode;
				break;
			}
			++itc;
		}
	}
}

void ClientGameObjectManagerAddon::do_PingSync(PE::Events::Event *pEvt){
	Event_PingSync_S_to_C *pTrueEvent = (Event_PingSync_S_to_C*)(pEvt);
	m_networkPing = (m_currentTime - pTrueEvent->m_time) / 2;
	// send out the ping ack package
	ClientNetworkManager *pNetworkManager = (ClientNetworkManager *)(m_pContext->getNetworkManager());
	if (pNetworkManager->m_state != ClientNetworkManager::ClientState_Disconnected){
		CharacterControl::Events::Event_PingAck_C_to_S evt(*m_pContext);
		evt.m_pingTime = m_networkPing;
		pNetworkManager->getNetworkContext().getEventManager()->scheduleEvent(&evt, m_pContext->getGameObjectManager(), true);
	}
	m_waitingForPing = false;
}

void ClientGameObjectManagerAddon::do_TimeSync(PE::Events::Event *pEvt){
	Event_TimeSync_S_to_C *pTrueEvent = (Event_TimeSync_S_to_C*)(pEvt);
	//PEINFO("Received time: %f", pTrueEvent->m_currentServerTime);
	m_currentTime = pTrueEvent->m_currentServerTime;
}

void ClientGameObjectManagerAddon::do_PhysicsProcess(PE::Events::Event *pEvt){
	Event_PhysicsProcess_S_to_C *pTrueEvent = (Event_PhysicsProcess_S_to_C*)(pEvt);

	PE::Handle *pHC = m_components.getFirstPtr();

	int itc = 0;
	for (PrimitiveTypes::UInt32 i = 0; i < m_components.m_size; i++, pHC++) // fast array traversal (increasing ptr)
	{
		Component *pC = (*pHC).getObject<Component>();

		if (pC->isInstanceOf<TankController>())
		{
			if (itc == pTrueEvent->m_clientTankId) //activate tank controller for local client based on local clients id
			{
				TankController *pTK = (TankController *)(pC);
				SceneNode *pFirstSN = pTK->getFirstComponent<SceneNode>();
				PhysicsNode *pMainPN = pFirstSN->pPhysicsNode;
				pMainPN->m_state = PhysicsNode::MOVING;
				pMainPN->velocity = pTrueEvent->m_dataMatrix.m[0][0]; // copy the velocity into the physics node
				pMainPN->rotation = pTrueEvent->m_dataMatrix.m[0][1]; // copy the rotation into the physics node
				//NavMeshManager *pNavMeshManager = m_pContext->getGameObjectManager()->getFirstComponent<NavMeshManager>();
				//float m_frameTime = m_currentTime - pTrueEvent->m_dataMatrix.m[0][2];
				//// Simulate the other car's movement here
				//if (pMainPN->m_state == PhysicsNode::MOVING)
				//{
				//	// Calculate pMainPN->u by checking four corners whehter are in NavMesh
				//	{
				//		pMainPN->rr = 0.0f;

				//		bool inRoad = true;

				//		Matrix4x4 rotation = pFirstSN->m_base;
				//		// For rotation
				//		Vector3 r_x = Vector3(rotation.getU().getX(), rotation.getV().getX(), rotation.getN().getX());
				//		Vector3 r_y = Vector3(rotation.getU().getY(), rotation.getV().getY(), rotation.getN().getY());
				//		Vector3 r_z = Vector3(rotation.getU().getZ(), rotation.getV().getZ(), rotation.getN().getZ());
				//		// For translation
				//		Vector3 origin = pFirstSN->m_base.getPos();

				//		Vector3 p[2], temp_p;
				//		p[0] = pMainPN->AABB_min;
				//		p[1] = pMainPN->AABB_max;
				//		Vector3 point[2][2][2];
				//		for (int i = 0; i < 2; i++)
				//			for (int j = 0; j < 2; j++)
				//				for (int k = 0; k < 2; k++)
				//				{
				//			temp_p.m_x = p[i].getX();
				//			temp_p.m_y = p[j].getY();
				//			temp_p.m_z = p[k].getZ();

				//			// Rotation
				//			point[i][j][k].m_x = r_x.dotProduct(temp_p);
				//			point[i][j][k].m_y = r_y.dotProduct(temp_p);
				//			point[i][j][k].m_z = r_z.dotProduct(temp_p);
				//			// Translation
				//			point[i][j][k] += origin;

				//			inRoad = pNavMeshManager->isOnNavMesh(point[i][j][k]);
				//			if (inRoad) {
				//				pMainPN->rr += rr_road;
				//				//m_isOnTrack = true;
				//			}
				//			else {
				//				pMainPN->rr += rr_grass;
				//				//m_isOnTrack = false;
				//			}
				//				}

				//		pMainPN->rr /= 2.0;
				//	}

				//	float speed = (float)abs(pMainPN->velocity);

				//	// Calculate rolling resistence and air resistence
				//	float resistence = pMainPN->u * speed * speed + pMainPN->rr * speed;

				//	pMainPN->resistence = resistence;

				//	pMainPN->velocity = pMainPN->velocity + pMainPN->accerleration * m_frameTime;
				//	float curSpeed = (float)abs(pMainPN->velocity) - resistence * m_frameTime;
				//	if (curSpeed < min_velocity) curSpeed = 0;
				//	else if (curSpeed > max_velocity) curSpeed = max_velocity;
				//	pMainPN->velocity = pMainPN->velocity > 0 ? curSpeed : -curSpeed;
				//	// Car stop
				//	if (curSpeed <= min_velocity)
				//	{
				//		pMainPN->m_state = PhysicsNode::STOP;
				//		pMainPN->velocity = 0.0f;
				//		pMainPN->accerleration = 0.0f;
				//	}
				//	// Moving Car
				//	else
				//	{
				//		float dis = pMainPN->velocity * m_frameTime;

				//		if (pMainPN->decreaseRotation)
				//		{
				//			float rotation_offset = 2 * m_frameTime * unit_rotation;

				//			if (pMainPN->rotation <= rotation_offset && pMainPN->rotation >= -rotation_offset)
				//			{
				//				pMainPN->rotation = 0.0f;
				//				pMainPN->decreaseRotation = false;
				//			}
				//			else
				//			{
				//				float r = pMainPN->rotation;
				//				pMainPN->rotation = r > 0 ? r - rotation_offset : r + rotation_offset;
				//			}
				//		}

				//		pFirstSN->m_base.turnLeft(pMainPN->rotation * (curSpeed / max_velocity));
				//		pFirstSN->m_base.moveForward(dis);

				//		pMainPN->m_base = pFirstSN->m_base;
				//		pMainPN->m_pos = pFirstSN->m_base.getPos();
				//		//pMainPN->setTransformMatrix(pFirstSN->m_base);
				//		pMainPN->accerleration = 0.0f;
				//	}
				//}

				break;
			}
			++itc;
		}
	}
}

}
}
