#include "PrimeEngine/PrimeEngineIncludes.h"

#include "CharacterControlIncludes.h"
#include "GlobalRegistry.h"

using namespace PE::Components;
using namespace CharacterControl::Components;
using namespace CharacterControl::Events;
namespace CharacterControl
{
	bool setLuaMetaDataOnly = 0;
}

void CharacterControl::Register(PE::Components::LuaEnvironment *pLuaEnv, PE::GlobalRegistry *pRegistry)
{
	// register classes defined in CharacterControl

	pLuaEnv->StartRootRegistrationTable();
	// start root
	{
		pLuaEnv->StartRegistrationTable("CharacterControl");
		// start root.CharacterControl
		{
			pLuaEnv->StartRegistrationTable("Components");
			// start root.CharacterControl.Components
			{
				WayPoint::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPC::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPCAnimationSM::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPCMovementSM::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPCBehaviorSM::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Flyer::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				TankController::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				TankGameControls::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				GameObjectManagerAddon::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
					ClientGameObjectManagerAddon::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
					ServerGameObjectManagerAddon::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
			}
			// end root.CharacterControl.Components
			pLuaEnv->EndRegistrationTable();


			pLuaEnv->StartRegistrationTable("Events");
			// start root.CharacterControl.Events
			{
				Event_CreateSoldierNPC::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_CREATE_WAYPOINT::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPCAnimSM_Event_STOP::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPCAnimSM_Event_WALK::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPCMovementSM_Event_MOVE_TO::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPCMovementSM_Event_TARGET_REACHED::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				SoldierNPCMovementSM_Event_STOP::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_MoveTank_C_to_S::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_MoveCar_C_to_S::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_MoveTank_S_to_C::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_MoveCar_S_to_C::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_Tank_Throttle::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_Tank_Turn::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_Stop_Timer::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_Reload_Car::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_PingSync_C_to_S::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_PingSync_S_to_C::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_PingAck_C_to_S::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_TimeSync_S_to_C::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_PhysicsProcess_C_to_S::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
				Event_PhysicsProcess_S_to_C::InitializeAndRegister(pLuaEnv, pRegistry, setLuaMetaDataOnly);
			}
			// end root.CharacterControl.Events
			pLuaEnv->EndRegistrationTable();
		}
		// end root.CharacterControl
		pLuaEnv->EndRegistrationTable();
	}
	// end root
	pLuaEnv->EndRootRegistrationTable();

	setLuaMetaDataOnly = true; // make sure on next pass we dont reset class id, we just set registration values in lua
}

