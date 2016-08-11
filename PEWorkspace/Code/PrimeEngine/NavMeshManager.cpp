#include "NavMeshManager.h"

using namespace std;

namespace PE {
	namespace Components{

		PE_IMPLEMENT_CLASS1(NavMeshManager, Component);

		NavMeshManager::NavMeshManager(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself) : Component(context, arena, hMyself)
		{

		}

		NavMeshManager::~NavMeshManager(){
		}

		void NavMeshManager::generatePolygon(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4){
			// create the polygon object and add the four vertices into the polygon
			NavPolygon polygon;
			polygon.m_vertices.push_back(v1);
			polygon.m_vertices.push_back(v2);
			polygon.m_vertices.push_back(v3);
			polygon.m_vertices.push_back(v4);
			// push this polygon into the vector
			m_polygons.push_back(polygon); 
		}

		void NavMeshManager::createStartPolygon(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4)
		{
			m_startPolygon.m_vertices.push_back(v1);
			m_startPolygon.m_vertices.push_back(v2);
			m_startPolygon.m_vertices.push_back(v3);
			m_startPolygon.m_vertices.push_back(v4);
		}

		/* Detect if the current pos is on the Nav mesh */
		bool NavMeshManager::isOnNavMesh(Vector3 pos){
			for (int i = 0; i < m_polygons.size(); i++){
				if (m_polygons[i].isOnPolygon(pos)){
					return true;
				}
			}
			return false;
		}

		bool isPointInTriangle(Vector3 pos, Vector3 vec0, Vector3 vec1, Vector3 vec2){
			if (pos == vec0 || pos == vec1 || pos == vec2){
				return true;
			}
			
			// should be reviewed (Barycentric method)
			float area = 0.5f * (-vec1.getZ() * vec2.getX() + vec0.getZ() * (-vec1.getX() + vec2.getX()) +
				vec0.getX() * (vec1.getZ() - vec2.getZ()) + vec1.getX() * vec2.getZ());
			int sign = area < 0 ? -1 : 1;
			float s = (vec0.getZ() * vec2.getX() - vec0.getX() * vec2.getZ() + (vec2.getZ() - vec0.getZ()) * pos.getX() +
				(vec0.getX() - vec2.getX()) * pos.getZ()) * sign;
			float t = (vec0.getX() * vec1.getZ() - vec0.getZ() * vec1.getX() + (vec0.getZ() - vec1.getZ()) * pos.getX() +
				(vec1.getX() - vec0.getX()) * pos.getZ()) * sign;
			return s > 0 && t > 0 && (s + t) < 2 * area * sign;

		}

	}; // namespace Components
}; // namespace PE