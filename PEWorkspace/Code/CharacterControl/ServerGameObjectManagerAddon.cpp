#include "ServerGameObjectManagerAddon.h"

#include "PrimeEngine/Lua/Server/ServerLuaEnvironment.h"
#include "PrimeEngine/Networking/Server/ServerNetworkManager.h"
#include "PrimeEngine/GameObjectModel/GameObjectManager.h"

#include "Characters/SoldierNPC.h"
#include "WayPoint.h"
#include "Tank/ClientTank.h"
#include "ClientGameObjectManagerAddon.h"

using namespace PE::Components;
using namespace PE::Events;
using namespace CharacterControl::Events;
using namespace CharacterControl::Components;

namespace CharacterControl{
	namespace Components
	{
		PE_IMPLEMENT_CLASS1(ServerGameObjectManagerAddon, GameObjectManagerAddon); // creates a static handle and GteInstance*() methods. still need to create construct

		void ServerGameObjectManagerAddon::addDefaultComponents()
		{
			GameObjectManagerAddon::addDefaultComponents();

			PE_REGISTER_EVENT_HANDLER(Event_MoveTank_C_to_S, ServerGameObjectManagerAddon::do_MoveTank);
			PE_REGISTER_EVENT_HANDLER(Event_MoveCar_C_to_S, ServerGameObjectManagerAddon::do_MoveCar);
			PE_REGISTER_EVENT_HANDLER(Event_PingSync_C_to_S, ServerGameObjectManagerAddon::do_PingSync);
			PE_REGISTER_EVENT_HANDLER(Event_PingAck_C_to_S, ServerGameObjectManagerAddon::do_PingAck);
			PE_REGISTER_EVENT_HANDLER(Event_PhysicsProcess_C_to_S, ServerGameObjectManagerAddon::do_PhysicsProcess);
			PE_REGISTER_EVENT_HANDLER(Event_UPDATE, ServerGameObjectManagerAddon::do_UPDATE);

			m_currentTime = 0.0f;
			m_timeSyncInterval = 0.0f;
			m_timeSyncInterval2 = 0.0f;
			m_physicsNode[0].do_INITIALIZATION();
			m_physicsNode[1].do_INITIALIZATION();
		}

		void ServerGameObjectManagerAddon::do_MoveTank(PE::Events::Event *pEvt)
		{
			assert(pEvt->isInstanceOf<Event_MoveTank_C_to_S>());

			Event_MoveTank_C_to_S *pTrueEvent = (Event_MoveTank_C_to_S*)(pEvt);

			// need to send this event to all clients except the client it came from

			Event_MoveTank_S_to_C fwdEvent(*m_pContext);
			fwdEvent.m_transform = pTrueEvent->m_transform;
			fwdEvent.m_clientTankId = pTrueEvent->m_networkClientId; // need to tell cleints which tank to move
			ServerNetworkManager *pNM = (ServerNetworkManager *)(m_pContext->getNetworkManager());
			pNM->scheduleEventToAllExcept(&fwdEvent, m_pContext->getGameObjectManager(), pTrueEvent->m_networkClientId);

		}

		void ServerGameObjectManagerAddon::do_MoveCar(PE::Events::Event *pEvt)
		{
			// retransfer the data to the other client
			// need to send this event to all clients except the client it came from
			Event_MoveCar_C_to_S *pTrueEvent = (Event_MoveCar_C_to_S *)(pEvt);

			Event_MoveCar_S_to_C fwdEvent(*m_pContext);
			fwdEvent.m_matrix = pTrueEvent->m_matrix;
			fwdEvent.m_clientTankId = pTrueEvent->m_networkClientId;
			ServerNetworkManager *pNM = (ServerNetworkManager *)(m_pContext->getNetworkManager());
			pNM->scheduleEventToAllExcept(&fwdEvent, m_pContext->getGameObjectManager(), pTrueEvent->m_networkClientId);

		}

		void ServerGameObjectManagerAddon::do_PingSync(PE::Events::Event *pEvt){
			Event_PingSync_C_to_S *pTrueEvent = (Event_PingSync_C_to_S*)(pEvt);
			Event_PingSync_S_to_C evt(*m_pContext);
			evt.m_time = pTrueEvent->m_time;
			ServerNetworkManager *pNM = (ServerNetworkManager *)(m_pContext->getNetworkManager());
			pNM->scheduleEventToTarget(&evt, m_pContext->getGameObjectManager(), pTrueEvent->m_networkClientId);
		}

		void ServerGameObjectManagerAddon::do_PingAck(PE::Events::Event *pEvt){
			Event_PingAck_C_to_S *pTrueEvent = (Event_PingAck_C_to_S*)(pEvt);
			m_pingTime[pTrueEvent->m_networkClientId] = pTrueEvent->m_pingTime;
			//PEINFO("Client num%d: %f", pTrueEvent->m_networkClientId, pTrueEvent->m_pingTime);
		}

		void ServerGameObjectManagerAddon::do_PhysicsProcess(PE::Events::Event *pEvt){
			Event_PhysicsProcess_C_to_S *pTrueEvent = (Event_PhysicsProcess_C_to_S*)(pEvt);
			//PEINFO("Velocity: %f, Rotation: %f, Time: %f", pTrueEvent->m_dataMatrix.m[0][0], pTrueEvent->m_dataMatrix.m[0][1], pTrueEvent->m_dataMatrix.m[0][2]);
			Event_PhysicsProcess_S_to_C fwdEvent(*m_pContext);
			fwdEvent.m_dataMatrix = pTrueEvent->m_dataMatrix;
			fwdEvent.m_clientTankId = pTrueEvent->m_networkClientId;
			ServerNetworkManager *pNM = (ServerNetworkManager *)(m_pContext->getNetworkManager());
			pNM->scheduleEventToAllExcept(&fwdEvent, m_pContext->getGameObjectManager(), pTrueEvent->m_networkClientId);
		}

		void ServerGameObjectManagerAddon::do_UPDATE(PE::Events::Event *pEvt){
			Event_UPDATE *pTrueEvent = (Event_UPDATE*)(pEvt);
			ServerNetworkManager *pNM = (ServerNetworkManager *)(m_pContext->getNetworkManager());
			m_currentTime += pTrueEvent->m_frameTime;

			// during a certain time interval, the server should sync with the client timer
			m_timeSyncInterval -= pTrueEvent->m_frameTime;
			if (m_timeSyncInterval >= 0.0f){
				// not the time to sync with the client
				return;
			}
			else{
				// send out the sync package to all the client
				m_timeSyncInterval = 5.0f;
				Event_TimeSync_S_to_C fwdEvent(*m_pContext);
				fwdEvent.m_currentServerTime = m_currentTime;
				pNM->scheduleEventToAll(&fwdEvent, m_pContext->getGameObjectManager());
			}
		}


}
}
