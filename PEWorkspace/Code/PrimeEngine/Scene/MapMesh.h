#ifndef __PYENGINE_2_0_MAPMESH_H__
#define __PYENGINE_2_0_MAPMESH_H__

#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "PrimeEngine/APIAbstraction/Effect/Effect.h"

// Sibling/Children includes
#include "Mesh.h"

namespace PE {
	namespace Components {

		struct MapMesh : public Mesh
		{
			PE_DECLARE_CLASS(MapMesh);

			// Constructor -------------------------------------------------------------
			MapMesh(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself) : Mesh(context, arena, hMyself)
			{
				m_loaded = false;
			}

			virtual ~MapMesh(){}

			virtual void addDefaultComponents();

			PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_GATHER_DRAWCALLS);
			virtual void do_GATHER_DRAWCALLS(Events::Event *pEvt);

			//void loadFromString_needsRC(const char *str, const char *techName, int &threadOwnershipMask);
			void loadFromPos_needsRC(float x, float y, const char *techName, int &threadOwnershipMask);
			PrimitiveTypes::Float32 m_charW, m_charWAbs, m_charH, m_textLength;
			PrimitiveTypes::Bool m_loaded;
			Handle m_meshCPU;
		};

	}; // namespace Components
}; // namespace PE
#endif
