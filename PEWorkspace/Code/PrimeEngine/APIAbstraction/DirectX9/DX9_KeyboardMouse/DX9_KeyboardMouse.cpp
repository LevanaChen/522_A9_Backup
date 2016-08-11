// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"
#if APIABSTRACTION_D3D9 | APIABSTRACTION_D3D11 | (APIABSTRACTION_OGL && !defined(SN_TARGET_PS3))

// Outer-Engine includes

// Inter-Engine includes
#include "PrimeEngine/Events/StandardKeyboardEvents.h"
#include "PrimeEngine/Render/D3D11Renderer.h"
#include "PrimeEngine/Render/D3D9Renderer.h"
#include "../../../Lua/LuaEnvironment.h"
// Sibling/Children includes
#include "DX9_KeyboardMouse.h"
#include "PrimeEngine/Application/WinApplication.h"

namespace PE {
using namespace Events;
namespace Components {

PE_IMPLEMENT_CLASS1(DX9_KeyboardMouse, Component);

void DX9_KeyboardMouse::addDefaultComponents()
{
	Component::addDefaultComponents();
	PE_REGISTER_EVENT_HANDLER(Events::Event_UPDATE, DX9_KeyboardMouse::do_UPDATE);

	oldKeyState_P = false;
	oldKeyState_A = false;
	oldKeyState_D = false;
	oldKeyState_LEFT = false;
	oldKeyState_RIGHT = false;
	oldKeyState_K = false;
	oldKeyState_L = false;
}

void DX9_KeyboardMouse::do_UPDATE(Events::Event *pEvt)
{
	
	m_pQueueManager = Events::EventQueueManager::Instance();

	generateButtonEvents();
}

void DX9_KeyboardMouse::generateButtonEvents()
{
#ifndef _XBOX
#if PE_PLAT_IS_WIN32
	WinApplication *pWinApp = static_cast<WinApplication*>(m_pContext->getApplication());
	if(GetFocus() == pWinApp->getWindowHandle())
#endif
	{
		//Check for Button Down events
		if ((GetAsyncKeyState('A') & 0x8000) && !oldKeyState_A){
			// Key down event
			oldKeyState_A = true;
			Handle h("Event", sizeof(Event_KEY_A_DOWN));
			new (h)Event_KEY_A_DOWN;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if ((GetAsyncKeyState('D') & 0x8000) && !oldKeyState_D){
			// Key down event
			oldKeyState_D = true;
			Handle h("Event", sizeof(Event_KEY_D_DOWN));
			new (h)Event_KEY_D_DOWN;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && !oldKeyState_LEFT){
			// Key down event
			oldKeyState_LEFT = true;
			Handle h("Event", sizeof(Event_KEY_LEFT_DOWN));
			new (h)Event_KEY_LEFT_DOWN;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && !oldKeyState_RIGHT){
			// Key down event
			oldKeyState_RIGHT = true;
			Handle h("Event", sizeof(Event_KEY_RIGHT_DOWN));
			new (h)Event_KEY_RIGHT_DOWN;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if ((GetAsyncKeyState('P') & 0x8000) && !oldKeyState_P){
			// Key down event
			oldKeyState_P = true;
			Handle h("Event", sizeof(Event_KEY_P_DOWN));
			new (h)Event_KEY_P_DOWN;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if ((GetAsyncKeyState('K') & 0x8000) && !oldKeyState_K){
			// Key down event
			oldKeyState_K = true;
			Handle h("Event", sizeof(Event_KEY_K_DOWN));
			new (h)Event_KEY_K_DOWN;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if ((GetAsyncKeyState('L') & 0x8000) && !oldKeyState_L){
			// Key down event
			oldKeyState_L = true;
			Handle h("Event", sizeof(Event_KEY_L_DOWN));
			new (h)Event_KEY_L_DOWN;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		//Check for Button Up events
		if (!(GetAsyncKeyState('A') & 0x8000) && oldKeyState_A){
			// key up event
			oldKeyState_A = false;
			Handle h("EVENT", sizeof(Event_KEY_A_UP));
			new (h)Event_KEY_A_UP;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if (!(GetAsyncKeyState('D') & 0x8000) && oldKeyState_D){
			// key up event
			oldKeyState_D = false;
			Handle h("EVENT", sizeof(Event_KEY_D_UP));
			new (h)Event_KEY_D_UP;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if (!(GetAsyncKeyState(VK_LEFT) & 0x8000) && oldKeyState_LEFT){
			// key up event
			oldKeyState_LEFT = false;
			Handle h("EVENT", sizeof(Event_KEY_LEFT_UP));
			new (h)Event_KEY_LEFT_UP;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if (!(GetAsyncKeyState(VK_RIGHT) & 0x8000) && oldKeyState_RIGHT){
			// key up event
			oldKeyState_RIGHT = false;
			Handle h("EVENT", sizeof(Event_KEY_RIGHT_UP));
			new (h)Event_KEY_RIGHT_UP;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if (!(GetAsyncKeyState('P') & 0x8000) && oldKeyState_P){
			// key up event
			oldKeyState_P = false;
			Handle h("EVENT", sizeof(Event_KEY_P_UP));
			new (h)Event_KEY_P_UP;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if (!(GetAsyncKeyState('K') & 0x8000) && oldKeyState_K){
			// key up event
			oldKeyState_K = false;
			Handle h("EVENT", sizeof(Event_KEY_K_UP));
			new (h)Event_KEY_K_UP;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if (!(GetAsyncKeyState('L') & 0x8000) && oldKeyState_L){
			// key up event
			oldKeyState_L = false;
			Handle h("EVENT", sizeof(Event_KEY_L_UP));
			new (h)Event_KEY_L_UP;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		
		//Check for Button Held events
		if (GetAsyncKeyState('P') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_P_HELD));
			new (h)Event_KEY_P_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}

		if(GetAsyncKeyState('A') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_A_HELD));
			new (h) Event_KEY_A_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState('S') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_S_HELD));
			new (h) Event_KEY_S_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState('D') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_D_HELD));
			new (h) Event_KEY_D_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState('W') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_W_HELD));
			new (h) Event_KEY_W_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_LEFT_HELD));
			new (h) Event_KEY_LEFT_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_DOWN_HELD));
			new (h) Event_KEY_DOWN_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_RIGHT_HELD));
			new (h) Event_KEY_RIGHT_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState(VK_UP) & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_UP_HELD));
			new (h) Event_KEY_UP_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState(',') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_COMMA_HELD));
			new (h) Event_KEY_COMMA_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState('.') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_PERIOD_HELD));
			new (h) Event_KEY_PERIOD_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState('K') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_K_HELD));
			new (h) Event_KEY_K_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
		if(GetAsyncKeyState('L') & 0x8000)
		{
			Handle h("EVENT", sizeof(Event_KEY_L_HELD));
			new (h) Event_KEY_L_HELD;
			m_pQueueManager->add(h, Events::QT_INPUT);
		}
	}
#endif
}

}; // namespace Components
}; // namespace PE

#endif // API Abstraction
