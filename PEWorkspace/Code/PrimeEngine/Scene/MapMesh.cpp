#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <math.h>
// Inter-Engine includes
#include "PrimeEngine/FileSystem/FileReader.h"
#include "PrimeEngine/APIAbstraction/GPUMaterial/GPUMaterialSet.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "PrimeEngine/APIAbstraction/Texture/Texture.h"
#include "PrimeEngine/APIAbstraction/Effect/EffectManager.h"
#include "PrimeEngine/APIAbstraction/GPUBuffers/VertexBufferGPUManager.h"
#include "Light.h"
#include "PrimeEngine/GameObjectModel/Camera.h"

// Sibling/Children includes
#include "MapMesh.h"
#include "SceneNode.h"
#include "CameraManager.h"
#include "../Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

namespace PE {
	namespace Components{

		PE_IMPLEMENT_CLASS1(MapMesh, Mesh);

		void MapMesh::addDefaultComponents()
		{
			//add this handler before Mesh's handlers so we can intercept draw and modify transform
			PE_REGISTER_EVENT_HANDLER(Events::Event_GATHER_DRAWCALLS, MapMesh::do_GATHER_DRAWCALLS);
			Mesh::addDefaultComponents();
		}

		void MapMesh::loadFromPos_needsRC(float x, float y, const char *techName, int &threadOwnershipMask){
			const char *str = { "Hello" };
			int len = StringOps::length(str);

			float offset = 0.2f;
			float uMin = x - offset;
			float uMax = x + offset;
			float vMin = y - offset;
			float vMax = y + offset;
			uMin = MAX(uMin, 0);
			uMax = MIN(uMax, 1.0f);
			vMin = MAX(vMin, 0);
			vMax = MIN(vMax, 1.0f);

			if (uMin == 0.0f){
				uMax = 0.4f;
			}

			if (uMax == 1.0f){
				uMin = 0.6f;
			}

			if (vMin == 0.0f){
				vMax = 0.4f;
			}

			if (vMax == 1.0f){
				vMin = 0.6f;
			}

			if (!m_meshCPU.isValid()){
				m_meshCPU = Handle("MeshCPU MapMesh", sizeof(MeshCPU));
				new (m_meshCPU)MeshCPU(*m_pContext, m_arena);
			}

			MeshCPU &mcpu = *m_meshCPU.getObject<MeshCPU>();

			if (!m_loaded)
				mcpu.createBillboardMeshWithColorTexture("Map.dds", "CarRacingDemo");
			
			// this will cause not using the vertex buffer manager
			// so that engine always creates a new vertex buffer gpu and doesn't try to find and
			// existing one
			mcpu.m_manualBufferManagement = true;

			PositionBufferCPU *pVB = mcpu.m_hPositionBufferCPU.getObject<PositionBufferCPU>();
			TexCoordBufferCPU *pTCB = mcpu.m_hTexCoordBufferCPU.getObject<TexCoordBufferCPU>();
			NormalBufferCPU *pNB = mcpu.m_hNormalBufferCPU.getObject<NormalBufferCPU>();
			IndexBufferCPU *pIB = mcpu.m_hIndexBufferCPU.getObject<IndexBufferCPU>();

			pVB->m_values.reset(4 * 3);
			pTCB->m_values.reset(4 * 2);
			pNB->m_values.reset(4 * 3);
			pIB->m_values.reset(6); // 2 tris

			pIB->m_indexRanges[0].m_start = 0;
			pIB->m_indexRanges[0].m_end = 5;
			pIB->m_indexRanges[0].m_minVertIndex = 0;
			pIB->m_indexRanges[0].m_maxVertIndex = 3;

			pIB->m_minVertexIndex = pIB->m_indexRanges[0].m_minVertIndex;
			pIB->m_maxVertexIndex = pIB->m_indexRanges[0].m_maxVertIndex;

			// we need all the buffer data here
			// TODO
			pVB->m_values.add(0, 0, 0);
			pVB->m_values.add(12, 0, 0);
			pVB->m_values.add(12, -12, 0);
			pVB->m_values.add(0, -12, 0);

			pIB->m_values.add(0, 1, 2);
			pIB->m_values.add(2, 3, 0);

			pTCB->m_values.add(uMin, vMin);
			pTCB->m_values.add(uMax, vMin);
			pTCB->m_values.add(uMax, vMax);
			pTCB->m_values.add(uMin, vMax);

			pNB->m_values.add(0, 0, 0);
			pNB->m_values.add(0, 0, 0);
			pNB->m_values.add(0, 0, 0);
			pNB->m_values.add(0, 0, 0);


			if (!m_loaded)
			{
				// first time creating gpu mesh
				loadFromMeshCPU_needsRC(mcpu, threadOwnershipMask);

				if (techName)
				{
					Handle hEffect = EffectManager::Instance()->getEffectHandle(techName);

					for (unsigned int imat = 0; imat < m_effects.m_size; imat++)
					{
						if (m_effects[imat].m_size)
							m_effects[imat][0] = hEffect;
					}
				}
				m_loaded = true;
			}
			else
			{
				//just need to update vertex buffers gpu
				updateGeoFromMeshCPU_needsRC(mcpu, threadOwnershipMask);
			}
		}

		void MapMesh::do_GATHER_DRAWCALLS(Events::Event *pEvt)
		{

		}


	}; // namespace Components
}; // namespace PE
