#ifndef __NAV_MESH_MANAGER_H
#define __NAV_MESH_MANAGER_H
#include "PrimeEngine/PrimeEngineIncludes.h"
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"
#include "PrimeEngine/Events/Component.h"

#include <vector>
#include <deque>
#include <algorithm>

namespace PE {
	namespace Components {

		// helper function
		bool isPointInTriangle(Vector3 pos, Vector3 vec0, Vector3 vec1, Vector3 vec2);

		/* Deprecated class as the new isReachable() method will be implemented*/
		struct NavPolygon{

			NavPolygon(){
				//m_vertices.resize(4);
			}
			~NavPolygon(){}
			
			/* The test would be done in 2D */
			bool isOnPolygon(Vector3 pos){
				return isPointInTriangle(pos, m_vertices[0], m_vertices[1], m_vertices[2]) || isPointInTriangle(pos, m_vertices[0], m_vertices[2], m_vertices[3]);
			}

			std::vector<Vector3> m_vertices;
		};
		

		struct NavMeshManager : public Component
		{
			PE_DECLARE_CLASS(NavMeshManager);
			// Constructor -------------------------------------------------------------
			// when created in a scene node
			NavMeshManager(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself);
			virtual ~NavMeshManager();

			void generatePolygon(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);
			void createStartPolygon(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);
			bool isOnNavMesh(Vector3 pos);

			std::vector<NavPolygon> m_polygons; // store all the polygons
			NavPolygon m_startPolygon;
		};

		
	}; // namespace Components
}; // namespace PE
#endif