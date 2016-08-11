#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Inter-Engine includes
#include "PrimeEngine/Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"
#include "PrimeEngine/Scene/Mesh.h"
#include "PrimeEngine/Scene/SceneNode.h"
#include "PrimeEngine/Networking/EventManager.h"
#include "PrimeEngine/Networking/Client/ClientNetworkManager.h"
#include "CharacterControl/Events/Events.h"
#include "PrimeEngine/GameObjectModel/GameObjectManager.h"
#include "PrimeEngine/Events/StandardKeyboardEvents.h"
#include "PrimeEngine/Events/StandardIOSEvents.h"
#include "PrimeEngine/Events/StandardGameEvents.h"
#include "PrimeEngine/Events/EventQueueManager.h"
#include "PrimeEngine/Events/StandardControllerEvents.h"
#include "PrimeEngine/GameObjectModel/DefaultGameControls/DefaultGameControls.h"
#include "CharacterControl/CharacterControlContext.h"

#include "ClientTank.h"

// For Assignment 8 : Add including PhysicsNode in ClientTank
#include "PrimeEngine/PhysicsNode.h"
#include "PrimeEngine/RootPhysicsNode.h"
#include "../ClientGameObjectManagerAddon.h"

using namespace PE::Components;
using namespace PE::Events;
using namespace CharacterControl::Events;

// Arkane Control Values
#define Analog_To_Digital_Trigger_Distance 0.5f
static float Debug_Fly_Speed = 8.0f; //Units per second
#define Debug_Rotate_Speed 2.0f //Radians per second
#define Player_Keyboard_Rotate_Speed 20.0f //Radians per second

// For Assignment 8 : Implement car movement
static float unit_accerleration = 60.0f;	// Unit accerleration per second for throttle - fps + resistence value
static float unit_rotation = 0.05f;			// Unit rotation per second for rotation
static float max_velocity = 3.0f;			// Car's maximal velocity
static float min_velocity = 0.005f;			// Car's minimal velociry
static float max_angle = 0.5f;				// Car's rotation maximal angle
static float rr_grass = 0.35f;				// Rolling resistence in grass
static float rr_road = 0.1f;				// Rolling resistence in road
static float max_depth = 3.0f;				// The maximal depth that car could reach by falling down

namespace CharacterControl {
namespace Components {

	


PE_IMPLEMENT_CLASS1(TankGameControls, PE::Components::Component);

void TankGameControls::addDefaultComponents()
{
	Component::addDefaultComponents();

	PE_REGISTER_EVENT_HANDLER(Event_UPDATE, TankGameControls::do_UPDATE);
}

void TankGameControls::do_UPDATE(PE::Events::Event *pEvt)
{
	// Process input events (XBOX360 buttons, triggers...)
	PE::Handle iqh = PE::Events::EventQueueManager::Instance()->getEventQueueHandle("input");
	
	// Process input event -> game event conversion
	while (!iqh.getObject<PE::Events::EventQueue>()->empty())
	{
		PE::Events::Event *pInputEvt = iqh.getObject<PE::Events::EventQueue>()->getFront();
		m_frameTime = ((Event_UPDATE*)(pEvt))->m_frameTime;
		// Have DefaultGameControls translate the input event to GameEvents
		handleKeyboardDebugInputEvents(pInputEvt);
		handleControllerDebugInputEvents(pInputEvt);
        handleIOSDebugInputEvents(pInputEvt);
		
		iqh.getObject<PE::Events::EventQueue>()->destroyFront();
	}

	// Events are destoryed by destroyFront() but this is called every frame just in case
	iqh.getObject<PE::Events::EventQueue>()->destroy();
}
    
void TankGameControls::handleIOSDebugInputEvents(Event *pEvt)
{
    #if APIABSTRACTION_IOS
    m_pQueueManager = PE::Events::EventQueueManager::Instance();
    if (Event_IOS_TOUCH_MOVED::GetClassId() == pEvt->getClassId())
    {
        Event_IOS_TOUCH_MOVED *pRealEvent = (Event_IOS_TOUCH_MOVED *)(pEvt);
        
        if(pRealEvent->touchesCount > 1)
        {
            PE::Handle h("EVENT", sizeof(Events::Event_Tank_Throttle));
            Events::Event_Tank_Throttle *flyCameraEvt = new(h) Events::Event_Tank_Throttle ;
                
            Vector3 relativeMovement(0.0f,0.0f,-30.0f * pRealEvent->m_normalized_dy);
            flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
            m_pQueueManager->add(h, QT_GENERAL);
        }
        else
        {
            PE::Handle h("EVENT", sizeof(Event_Tank_Turn));
            Event_Tank_Turn *rotateCameraEvt = new(h) Event_Tank_Turn ;
            
            Vector3 relativeRotate(pRealEvent->m_normalized_dx * 10,0.0f,0.0f);
            rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
            m_pQueueManager->add(h, QT_GENERAL);
        }
    }
    #endif
}

void TankGameControls::handleKeyboardDebugInputEvents(Event *pEvt)
{
	m_pQueueManager = PE::Events::EventQueueManager::Instance();
	/*
	if (PE::Events::Event_KEY_A_HELD::GetClassId() == pEvt->getClassId())
	{
	PE::Handle h("EVENT", sizeof(Event_FLY_CAMERA));
	PE::Events::Event_FLY_CAMERA *flyCameraEvt = new(h) PE::Events::Event_FLY_CAMERA ;

	Vector3 relativeMovement(-1.0f,0.0f,0.0f);
	flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
	m_pQueueManager->add(h, QT_GENERAL);
	}
	else
	*/
	if (Event_KEY_S_HELD::GetClassId() == pEvt->getClassId())
	{
		PE::Handle h("EVENT", sizeof(Events::Event_Tank_Throttle));
		Events::Event_Tank_Throttle *flyCameraEvt = new(h)Events::Event_Tank_Throttle;

		Vector3 relativeMovement(0.0f, 0.0f, -1.0f);
		// flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;

		// For Assignment 8 : Movement Implementation
		flyCameraEvt->m_relativeMove = relativeMovement * unit_accerleration * m_frameTime;
		m_pQueueManager->add(h, QT_GENERAL);
	}
	/*
	else if (Event_KEY_D_HELD::GetClassId() == pEvt->getClassId())
	{
	PE::Handle h("EVENT", sizeof(Event_FLY_CAMERA));
	Event_FLY_CAMERA *flyCameraEvt = new(h) Event_FLY_CAMERA ;

	Vector3 relativeMovement(1.0f,0.0f,0.0f);
	flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
	m_pQueueManager->add(h, QT_GENERAL);
	}*/
	else if (Event_KEY_W_HELD::GetClassId() == pEvt->getClassId())
	{
		PE::Handle h("EVENT", sizeof(Events::Event_Tank_Throttle));
		Events::Event_Tank_Throttle *flyCameraEvt = new(h)Events::Event_Tank_Throttle;

		Vector3 relativeMovement(0.0f, 0.0f, 1.0f);
		// flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;

		// For Assignment 8 : Movement Implementation
		flyCameraEvt->m_relativeMove = relativeMovement * unit_accerleration * m_frameTime;

		m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_KEY_LEFT_DOWN::GetClassId() == pEvt->getClassId())
	{
		// Event Handle
	}
	else if (Event_KEY_LEFT_HELD::GetClassId() == pEvt->getClassId())
	{
		PE::Handle h("EVENT", sizeof(Event_Tank_Turn));
		Event_Tank_Turn *rotateCameraEvt = new(h)Event_Tank_Turn;

		Vector3 relativeRotate(1.0f, 0.0f, 0.0f);
		// rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;

		// For Assignment 8 : Movement Implementation
		rotateCameraEvt->m_relativeRotate = relativeRotate * unit_rotation * m_frameTime;

		m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_KEY_LEFT_UP::GetClassId() == pEvt->getClassId())
	{
		// Event Handle
		// For Assignment 8 : Add game event to handle decreasing rotation events

		PE::Handle h("EVENT", sizeof(Event_Tank_Turn));
		Event_Tank_Turn *rotateCameraEvt = new(h)Event_Tank_Turn;

		Vector3 relativeRotate(0.0f, 0.0f, 1.0f);

		rotateCameraEvt->m_relativeRotate = relativeRotate;

		m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_KEY_RIGHT_DOWN::GetClassId() == pEvt->getClassId())
	{
		// Event Handle
	}
	else if (Event_KEY_RIGHT_HELD::GetClassId() == pEvt->getClassId())
	{
		PE::Handle h("EVENT", sizeof(Event_Tank_Turn));
		Event_Tank_Turn *rotateCameraEvt = new(h)Event_Tank_Turn;

		Vector3 relativeRotate(-1.0f, 0.0f, 0.0f);
		//rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;

		// For Assignment 8 : Movement Implementation
		rotateCameraEvt->m_relativeRotate = relativeRotate * unit_rotation * m_frameTime;
		m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_KEY_RIGHT_UP::GetClassId() == pEvt->getClassId())
	{
		// Event Handle
		// For Assignment 8 : Add game event to handle decreasing rotation events

		PE::Handle h("EVENT", sizeof(Event_Tank_Turn));
		Event_Tank_Turn *rotateCameraEvt = new(h)Event_Tank_Turn;

		Vector3 relativeRotate(0.0f, 0.0f, 1.0f);

		rotateCameraEvt->m_relativeRotate = relativeRotate;

		m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_KEY_K_UP::GetClassId() == pEvt->getClassId())
	{
		// Reload the car position on the finish line
		PEINFO("Reload");
		PE::Handle h("EVENT", sizeof(Event_Reload_Car));
		Event_Reload_Car *rotateCameraEvt = new(h)Event_Reload_Car;

		m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_KEY_L_UP::GetClassId() == pEvt->getClassId())
	{
		// Stop the timer
		PEINFO("Stop the timer");
		PE::Handle h("EVENT", sizeof(Event_Stop_Timer));
		Event_Stop_Timer *rotateCameraEvt = new(h)Event_Stop_Timer;

		m_pQueueManager->add(h, QT_GENERAL);
	}
	/*
	else if (Event_KEY_DOWN_HELD::GetClassId() == pEvt->getClassId())
	{
	PE::Handle h("EVENT", sizeof(Event_ROTATE_CAMERA));
	Event_ROTATE_CAMERA *rotateCameraEvt = new(h) Event_ROTATE_CAMERA ;

	Vector3 relativeRotate(0.0f,-1.0f,0.0f);
	rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
	m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_KEY_UP_HELD::GetClassId() == pEvt->getClassId())
	{
	PE::Handle h("EVENT", sizeof(Event_ROTATE_CAMERA));
	Event_ROTATE_CAMERA *rotateCameraEvt = new(h) Event_ROTATE_CAMERA ;

	Vector3 relativeRotate(0.0f,1.0f,0.0f);
	rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
	m_pQueueManager->add(h, QT_GENERAL);
	}
	*/

	else
	{
		Component::handleEvent(pEvt);
	}
}

void TankGameControls::handleControllerDebugInputEvents(Event *pEvt)
{
	
	if (Event_ANALOG_L_THUMB_MOVE::GetClassId() == pEvt->getClassId())
	{
		Event_ANALOG_L_THUMB_MOVE *pRealEvent = (Event_ANALOG_L_THUMB_MOVE*)(pEvt);
		
		//throttle
		{
			PE::Handle h("EVENT", sizeof(Events::Event_Tank_Throttle));
			Events::Event_Tank_Throttle *flyCameraEvt = new(h) Events::Event_Tank_Throttle ;

			Vector3 relativeMovement(0.0f,0.0f, pRealEvent->m_absPosition.getY());
			// flyCameraEvt->m_relativeMove = relativeMovement * Debug_Fly_Speed * m_frameTime;
			
			// For Assignment 8 : Movement Implementation
			flyCameraEvt->m_relativeMove = relativeMovement * unit_accerleration * m_frameTime;
			
			m_pQueueManager->add(h, QT_GENERAL);
		}

		//turn
		{
			PE::Handle h("EVENT", sizeof(Event_Tank_Turn));
			Event_Tank_Turn *rotateCameraEvt = new(h) Event_Tank_Turn ;

			Vector3 relativeRotate(pRealEvent->m_absPosition.getX(), 0.0f, 0.0f);
			// rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
			
			// For Assignment 8 : Movement Implementation
			rotateCameraEvt->m_relativeRotate = relativeRotate * unit_rotation * m_frameTime;

			m_pQueueManager->add(h, QT_GENERAL);
		}
	}
	else if (Event_BUTTON_A_UP::GetClassId() == pEvt->getClassId())
	{
		// Stop the timer
		PEINFO("Stop the timer");
		PE::Handle h("EVENT", sizeof(Event_Stop_Timer));
		Event_Stop_Timer *rotateCameraEvt = new(h)Event_Stop_Timer;

		m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_BUTTON_B_UP::GetClassId() == pEvt->getClassId())
	{
		// Reload the car position on the finish line
		PEINFO("Reload");
		PE::Handle h("EVENT", sizeof(Event_Reload_Car));
		Event_Reload_Car *rotateCameraEvt = new(h)Event_Reload_Car;

		m_pQueueManager->add(h, QT_GENERAL);
	}
	/*
	else if (Event_ANALOG_R_THUMB_MOVE::GetClassId() == pEvt->getClassId())
	{
		Event_ANALOG_R_THUMB_MOVE *pRealEvent = (Event_ANALOG_R_THUMB_MOVE *)(pEvt);
		
		PE::Handle h("EVENT", sizeof(Event_ROTATE_CAMERA));
		Event_ROTATE_CAMERA *rotateCameraEvt = new(h) Event_ROTATE_CAMERA ;
		
		Vector3 relativeRotate(pRealEvent->m_absPosition.getX(), pRealEvent->m_absPosition.getY(), 0.0f);
		rotateCameraEvt->m_relativeRotate = relativeRotate * Debug_Rotate_Speed * m_frameTime;
		m_pQueueManager->add(h, QT_GENERAL);
	}
	else if (Event_PAD_N_DOWN::GetClassId() == pEvt->getClassId())
	{
	}
	else if (Event_PAD_N_HELD::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_N_UP::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_S_DOWN::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_S_HELD::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_S_UP::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_W_DOWN::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_W_HELD::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_W_UP::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_E_DOWN::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_E_HELD::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_PAD_E_UP::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_BUTTON_A_HELD::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_BUTTON_Y_DOWN::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_BUTTON_A_UP::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_BUTTON_B_UP::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_BUTTON_X_UP::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_BUTTON_Y_UP::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_ANALOG_L_TRIGGER_MOVE::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_ANALOG_R_TRIGGER_MOVE::GetClassId() == pEvt->getClassId())
	{
		
	}
	else if (Event_BUTTON_L_SHOULDER_DOWN::GetClassId() == pEvt->getClassId())
	{
		
	}
	else
	*/
	{
		Component::handleEvent(pEvt);
	}
}

PE_IMPLEMENT_CLASS1(TankController, Component);
    
TankController::TankController(PE::GameContext &context, PE::MemoryArena arena,
	PE::Handle myHandle, float speed, Vector3 spawnPos,
	float networkPingInterval)
: Component(context, arena, myHandle)
, m_timeSpeed(speed)
, m_time(0)
, m_counter(0)
, m_active(0)
, m_networkPingTimer(0)
, m_networkPingInterval(networkPingInterval)
, m_overriden(false)
{
	m_spawnPos = spawnPos;
	m_checkedIn = false;
	m_isCounting = false;
}
    
void TankController::addDefaultComponents()
{
    Component::addDefaultComponents();
        
    PE_REGISTER_EVENT_HANDLER(PE::Events::Event_UPDATE, TankController::do_UPDATE);

	// note: these event handlers will be registered only when one tank is activated as client tank (i.e. driven by client input on this machine)
// 	PE_REGISTER_EVENT_HANDLER(Event_Tank_Throttle, TankController::do_Tank_Throttle);
// 	PE_REGISTER_EVENT_HANDLER(Event_Tank_Turn, TankController::do_Tank_Turn);
	m_positionSyncTimer = 0.2f;

}

void TankController::do_Tank_Throttle(PE::Events::Event *pEvt)
{
	Event_Tank_Throttle *pRealEvent = (Event_Tank_Throttle *)(pEvt);

	PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
	if (!hFisrtSN.isValid())
	{
		assert(!"wrong setup. must have scene node referenced");
		return;
	}

	SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();

	// For Assignment 8 : Movement Implementation
	PhysicsNode *pPN = pFirstSN->pPhysicsNode;
	
	// When car is falling down, do not handle input events
	if ( pPN->m_state == PhysicsNode::FALLING ) return;

	pPN->m_state = PhysicsNode::MOVING;
	pPN->accerleration = pRealEvent->m_relativeMove.getZ();

	//ClientNetworkManager *pNetworkManager = (ClientNetworkManager *)(m_pContext->getNetworkManager());
	//if (pNetworkManager->m_state != ClientNetworkManager::ClientState_Disconnected && m_active){	// only active car should send out event
	//	// we use Event_MoveCar_C_to_S as the throttle event for the network
	//	CharacterControl::Events::Event_MoveTank_C_to_S evt(*m_pContext);
	//	evt.m_transform.setU(pRealEvent->m_relativeMove);
	//	//PEINFO("Send: %f, %f, %f", pRealEvent->m_relativeMove.m_x, pRealEvent->m_relativeMove.m_y, pRealEvent->m_relativeMove.m_z);
	//	pNetworkManager->getNetworkContext().getEventManager()->scheduleEvent(&evt, m_pContext->getGameObjectManager(), true);
	//}

	//pFirstSN->m_base.moveForward(pRealEvent->m_relativeMove.getZ());
	//pFirstSN->m_base.moveRight(pRealEvent->m_relativeMove.getX());
	//pFirstSN->m_base.moveUp(pRealEvent->m_relativeMove.getY());

}

void TankController::do_Tank_Turn(PE::Events::Event *pEvt)
{
	Event_Tank_Turn *pRealEvent = (Event_Tank_Turn *)(pEvt);

	PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
	if (!hFisrtSN.isValid())
	{
		assert(!"wrong setup. must have scene node referenced");
		return;
	}
	
	SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();

	PhysicsNode *pPN = pFirstSN->pPhysicsNode;

	// When car is falling down, do not handle input events
	if ( pPN->m_state == PhysicsNode::FALLING ) return;

	// For Assignment 8 : Movement Implementation
	if ( pRealEvent->m_relativeRotate.getX() == 0 && pRealEvent->m_relativeRotate.getZ() == 1 )
	{
		pPN->decreaseRotation = true;
	}
	else
	{
		pPN->decreaseRotation = false;
		pPN->rotation += pRealEvent->m_relativeRotate.getX();

		if ( pPN->rotation > max_angle ) pPN->rotation = max_angle;
		if ( pPN->rotation < - max_angle ) pPN->rotation = - max_angle;
	}

	//ClientNetworkManager *pNetworkManager = (ClientNetworkManager *)(m_pContext->getNetworkManager());
	//if (pNetworkManager->m_state != ClientNetworkManager::ClientState_Disconnected && m_active){ // only active car should send out the event
	//	// we use Event_MoveCar_C_to_S as the rotation event for the network
	//	CharacterControl::Events::Event_MoveCar_C_to_S evt(*m_pContext);
	//	evt.m_matrix.setU(pRealEvent->m_relativeRotate); 
	//	pNetworkManager->getNetworkContext().getEventManager()->scheduleEvent(&evt, m_pContext->getGameObjectManager(), true);
	//}

	//pcam->m_base.turnUp(pRealEvent->m_relativeRotate.getY());
	// pFirstSN->m_base.turnLeft(pRealEvent->m_relativeRotate.getX());

}

void TankController::do_Stop_Timer(PE::Events::Event *pEvt)
{
	// stop the timer
	ClientGameObjectManagerAddon *pClientGameOMA = getFirstParentByTypePtr<ClientGameObjectManagerAddon>();
	pClientGameOMA->m_records[m_id].m_time = 0.0f;
	pClientGameOMA->m_records[m_id].m_isCounting = false;
	// change the status
	m_isCounting = false;

}

void TankController::do_Reload_Car(PE::Events::Event *pEvt)
{
	// first stop the timer
	// stop the timer
	ClientGameObjectManagerAddon *pClientGameOMA = getFirstParentByTypePtr<ClientGameObjectManagerAddon>();
	pClientGameOMA->m_records[m_id].m_time = 0.0f;
	pClientGameOMA->m_records[m_id].m_isCounting = false;
	// change the status
	m_isCounting = false;

	PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
	SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();
	PhysicsNode *pMainPN = pFirstSN->pPhysicsNode;

	// Determine the reloading postion
	float reload_x = 12.0f;
	float reload_y = 0.069f;
	float reload_z = 0.0f;

	Vector3 reload_pos = Vector3(reload_x, reload_y, reload_z);
	pMainPN->do_RELOAD();

	pFirstSN->m_base.setPos(reload_pos);
	pFirstSN->m_base.setU(Vector3(1.0f, 0.0f, 0.0f));
	pFirstSN->m_base.setV(Vector3(0.0f, 1.0f, 0.0f));
	pFirstSN->m_base.setN(Vector3(0.0f, 0.0f, 1.0f));
	pMainPN->m_base = pFirstSN->m_base;
	pMainPN->m_pos = pFirstSN->m_base.getPos();

	m_isOnTrack = true;
}


void TankController::do_UPDATE(PE::Events::Event *pEvt)
{
	PE::Events::Event_UPDATE *pRealEvt = (PE::Events::Event_UPDATE *)(pEvt);
	NavMeshManager *pNavMeshManager = m_pContext->getGameObjectManager()->getFirstComponent<NavMeshManager>();
	m_frameTime = pRealEvt->m_frameTime;

	PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
	SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();
	PhysicsNode *pMainPN = pFirstSN->pPhysicsNode;

		float pos_y = (pFirstSN->m_base * pMainPN->AABB_min).getY();

		if (!hFisrtSN.isValid())
		{
			assert(!"wrong setup. must have scene node referenced");
			return;
		}

		if (true){
			// Setting reload function
			// ----------------------------------------------------------- //
			if (pFirstSN->m_base.getPos().getY() <= -max_depth)
			{
				// Reload Car

				//disable tank controls

				m_pContext->get<CharacterControlContext>()->getTankGameControls()->setEnabled(false);

				// Determine the reloading postion
				float reload_x = 9.0f;
				float reload_y = 0.069f;
				float reload_z = 0.0f;

				Vector3 reload_pos = Vector3(reload_x, reload_y, reload_z);
				pMainPN->do_RELOAD();

				pFirstSN->m_base.setPos(reload_pos);
				pMainPN->m_base = pFirstSN->m_base;
				pMainPN->m_pos = pFirstSN->m_base.getPos();
				//pMainPN->setTransformMatrix(pFirstSN->m_base); // use the new method 
			}

			else if (pMainPN->m_state == PhysicsNode::FALLING)
			{
				m_pContext->get<CharacterControlContext>()->getTankGameControls()->setEnabled(true);
				// Do Falling
				if (pFirstSN->m_base.getPos().getY() > -max_depth)
				{
					Vector3 unit_g = Vector3(0.0f, -pMainPN->g, 0.0f);
					pMainPN->fallingVelocity = pMainPN->fallingVelocity + 10 * unit_g * m_frameTime;

					float dis = pMainPN->fallingVelocity.length() * m_frameTime;
					Vector3 dir = pMainPN->fallingVelocity;
					dir.normalize();

					pFirstSN->m_base.setPos(pFirstSN->m_base.getPos() + dis * dir);

					pMainPN->m_base = pFirstSN->m_base;
					pMainPN->m_pos = pFirstSN->m_base.getPos();
					//pMainPN->setTransformMatrix(pFirstSN->m_base); // use the new method
					pMainPN->velocity = pMainPN->fallingVelocity.length();
				}
			}
			else
			{
				m_pContext->get<CharacterControlContext>()->getTankGameControls()->setEnabled(true);
				bool isFalling = false;
				bool isCollision = false;

				int iPhysicsNode = RootPhysicsNode::Instance()->getComponentCount();

				for (int i = 0; i < iPhysicsNode; i++)
				{
					PE::Handle hPN = RootPhysicsNode::Instance()->getComponentByIndex(i);
					PhysicsNode *pPN = hPN.getObject<PE::Components::PhysicsNode>();

					if (pPN->m_type == PhysicsNode::GRASS)
					{
						// Check Falling here!
						isFalling = pMainPN->do_PHYSICS_GRAVITY(pPN);

						// For debug
						// pMainPN->m_state = PhysicsNode::MOVING;
					}
					else if (pPN->m_type == PhysicsNode::CAR && pPN->m_id != pMainPN->m_id)
					{
						// Calculate collision here!

						// For Assignment 9 : Collision Detection
						isCollision = pMainPN->do_PHYSICS_COLLISION(pPN);
						// isCollision = isCollision || pPN->do_PHYSICS_COLLISION(pMainPN);

						// Offset of current car
						if ( isCollision )
						{
							pFirstSN->m_base.setPos( pMainPN->m_pos );
							pMainPN->m_base = pFirstSN->m_base;

							//pSceneNodeTest = pPN->getFirstParentByTypePtr<SceneNode>();
							//int test = 0;
							//if (pSN){
							//	int test = 0;
							//}
							//pSN->m_base.setPos(pPN->m_pos);
							//pPN->m_base = pSN->m_base;
						}
					}
				}

				if (pMainPN->m_state == PhysicsNode::MOVING)
				{
					// Calculate pMainPN->u by checking four corners whehter are in NavMesh
					{
						pMainPN->rr = 0.0f;

						bool inRoad = true;

						Matrix4x4 rotation = pFirstSN->m_base;
						// For rotation
						Vector3 r_x = Vector3(rotation.getU().getX(), rotation.getV().getX(), rotation.getN().getX());
						Vector3 r_y = Vector3(rotation.getU().getY(), rotation.getV().getY(), rotation.getN().getY());
						Vector3 r_z = Vector3(rotation.getU().getZ(), rotation.getV().getZ(), rotation.getN().getZ());
						// For translation
						Vector3 origin = pFirstSN->m_base.getPos();

						Vector3 p[2], temp_p;
						p[0] = pMainPN->AABB_min;
						p[1] = pMainPN->AABB_max;
						Vector3 point[2][2][2];
						for (int i = 0; i < 2; i++)
							for (int j = 0; j < 2; j++)
								for (int k = 0; k < 2; k++)
								{
							temp_p.m_x = p[i].getX();
							temp_p.m_y = p[j].getY();
							temp_p.m_z = p[k].getZ();

							// Rotation
							point[i][j][k].m_x = r_x.dotProduct(temp_p);
							point[i][j][k].m_y = r_y.dotProduct(temp_p);
							point[i][j][k].m_z = r_z.dotProduct(temp_p);
							// Translation
							point[i][j][k] += origin;

							inRoad = pNavMeshManager->isOnNavMesh(point[i][j][k]);
							if (inRoad) {
								pMainPN->rr += rr_road;
								m_isOnTrack = true;
							}
							else {
								pMainPN->rr += rr_grass;
								m_isOnTrack = false;
							}
								}

						pMainPN->rr /= 2.0;
					}

					float speed = (float)abs(pMainPN->velocity);

					// Calculate rolling resistence and air resistence
					float resistence = pMainPN->u * speed * speed + pMainPN->rr * speed;

					pMainPN->resistence = resistence;

					pMainPN->velocity = pMainPN->velocity + pMainPN->accerleration * m_frameTime;
					float curSpeed = (float)abs(pMainPN->velocity) - resistence * m_frameTime;
					if (curSpeed < min_velocity) curSpeed = 0;
					else if (curSpeed > max_velocity) curSpeed = max_velocity;
					pMainPN->velocity = pMainPN->velocity > 0 ? curSpeed : -curSpeed;
					// Car stop
					if (curSpeed <= min_velocity)
					{
						pMainPN->m_state = PhysicsNode::STOP;
						pMainPN->velocity = 0.0f;
						pMainPN->accerleration = 0.0f;
					}
					// Moving Car
					else
					{
						float dis = pMainPN->velocity * m_frameTime;

						if (pMainPN->decreaseRotation)
						{
							float rotation_offset = 2 * m_frameTime * unit_rotation;

							if (pMainPN->rotation <= rotation_offset && pMainPN->rotation >= -rotation_offset)
							{
								pMainPN->rotation = 0.0f;
								pMainPN->decreaseRotation = false;
							}
							else
							{
								float r = pMainPN->rotation;
								pMainPN->rotation = r > 0 ? r - rotation_offset : r + rotation_offset;
							}
						}

						pFirstSN->m_base.turnLeft(pMainPN->rotation * (curSpeed / max_velocity));
						pFirstSN->m_base.moveForward(dis);

						pMainPN->m_base = pFirstSN->m_base;
						pMainPN->m_pos = pFirstSN->m_base.getPos();
						//pMainPN->setTransformMatrix(pFirstSN->m_base);
						pMainPN->accerleration = 0.0f;

					}
				}
			}
		}

	if (m_active)
	{
		m_time += pRealEvt->m_frameTime;
		m_networkPingTimer += pRealEvt->m_frameTime;
	}

	//PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
	if (!hFisrtSN.isValid())
	{
		assert(!"wrong setup. must have scene node referenced");
		return;
	}

	//SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();

	static float x = 0.0f;
	static float y = 0.3f;
	static float z = -0.6f;

	// For Assignment 8 : Adjust camera position
	//static float x = 0.0f;
	//static float y = 8.0f;
	//static float z = -13.0f;

	// note we could have stored the camera reference in this object instead of searching for camera scene node
	if (CameraSceneNode *pCamSN = pFirstSN->getFirstComponent<CameraSceneNode>())
	{
		pCamSN->m_base.setPos(Vector3(x,y,z));
	}

	// Currently we don't send out anything here in the do_UPDATE function

	ClientGameObjectManagerAddon *pClientGameObjectMA = getFirstParentByTypePtr<ClientGameObjectManagerAddon>();
	m_networkPingTimer += pRealEvt->m_frameTime;
	if (m_networkPingTimer > 0.1f && m_active){
		PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
		SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();
		PhysicsNode *pMainPN = pFirstSN->pPhysicsNode;
		// send client authoritative position event
		CharacterControl::Events::Event_PhysicsProcess_C_to_S evt(*m_pContext);
		evt.m_dataMatrix.m[0][0] = pMainPN->velocity; // copy the velocity data into the network package
		evt.m_dataMatrix.m[0][1] = pMainPN->rotation;
		evt.m_dataMatrix.m[0][2] = pClientGameObjectMA->m_currentTime;
		ClientNetworkManager *pNetworkManager = (ClientNetworkManager *)(m_pContext->getNetworkManager());
		pNetworkManager->getNetworkContext().getEventManager()->scheduleEvent(&evt, m_pContext->getGameObjectManager(), true);
		m_networkPingTimer = 0.0f;
	}
	//// send out the position sync timer if a certain interval ends
	m_positionSyncTimer -= pRealEvt->m_frameTime;
	if (m_positionSyncTimer < 0.0f && m_active){
		m_positionSyncTimer = 0.2f;
		PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
		SceneNode *pFirstSN = hFisrtSN.getObject<SceneNode>();
		PhysicsNode *pMainPN = pFirstSN->pPhysicsNode;
		CharacterControl::Events::Event_MoveTank_C_to_S evt(*m_pContext);
		evt.m_transform = pFirstSN->m_base;
		ClientNetworkManager *pNetworkManager = (ClientNetworkManager *)(m_pContext->getNetworkManager());
		pNetworkManager->getNetworkContext().getEventManager()->scheduleEvent(&evt, m_pContext->getGameObjectManager(), true);
	}
	// here we add the timer for the record and do other stuff
	if (m_active){
		// we only count the player's car
		if (pNavMeshManager->m_startPolygon.isOnPolygon(pFirstSN->m_base.getPos())){
			// is on the start polygon
			m_checkedIn = true; // check in the car
			if (m_isCounting){
				// The car is already on counting; need to stop the counting
				if (pClientGameObjectMA){
					pClientGameObjectMA->m_records[m_id].m_isCounting = false;
				}
				m_isCounting = false;
			}
		}
		else{
			// the car is not in the start polygon region
			if (m_checkedIn){
				// the car just left the counting should start
				ClientGameObjectManagerAddon *pClientGameObjectMA = getFirstParentByTypePtr<ClientGameObjectManagerAddon>();
				if (pClientGameObjectMA){
					pClientGameObjectMA->m_records[m_id].m_isCounting = true;
				}
				m_isCounting = true;
				m_checkedIn = false;
			}
		}

		// we also store the UV value here for the map mesh
		float xPos = pFirstSN->m_base.getPos().m_x;
		float zPos = - pFirstSN->m_base.getPos().m_z;
		xPos += 23.0f;
		zPos += 23.0f;
		pClientGameObjectMA->m_u = xPos / 48.0f;
		pClientGameObjectMA->m_v = zPos / 48.0f;
	}
}

void TankController::overrideTransform(Matrix4x4 &t)
{
	m_overriden = true;
	m_transformOverride = t;
}

void TankController::activate()
{
	m_active = true;

	// this function is called on client tank. since this is client tank and we have lcient authoritative movement
	// we need to register event handling for movement here
	PE_REGISTER_EVENT_HANDLER(Event_Tank_Throttle, TankController::do_Tank_Throttle);
	PE_REGISTER_EVENT_HANDLER(Event_Tank_Turn, TankController::do_Tank_Turn);
	PE_REGISTER_EVENT_HANDLER(Event_Stop_Timer, TankController::do_Stop_Timer);
	PE_REGISTER_EVENT_HANDLER(Event_Reload_Car, TankController::do_Reload_Car);

	PE::Handle hFisrtSN = getFirstComponentHandle<SceneNode>();
	if (!hFisrtSN.isValid())
	{
		assert(!"wrong setup. must have scene node referenced");
		return;
	}

	//create camera
	PE::Handle hCamera("Camera", sizeof(Camera));
	Camera *pCamera = new(hCamera) Camera(*m_pContext, m_arena, hCamera, hFisrtSN);
	pCamera->addDefaultComponents();
	CameraManager::Instance()->setCamera(CameraManager::VEHICLE, hCamera);

	CameraManager::Instance()->selectActiveCamera(CameraManager::VEHICLE);

	//disable default camera controls

	m_pContext->getDefaultGameControls()->setEnabled(false);

	//enable tank controls

	m_pContext->get<CharacterControlContext>()->getTankGameControls()->setEnabled(true);
	
	// assign this car id in the ClientGameObjectManagerAddon

	getFirstParentByTypePtr<ClientGameObjectManagerAddon>()->m_activatedCarID = m_id;
}


}
}
