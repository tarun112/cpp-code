#pragma once
#include <vector>
#include <unordered_map>
using namespace std;

#include "entity.h"

namespace Estimate {

	//Part is root entity containing 
	//counter clockwise oriented list of all edges
	class Part {
		friend class EntityManager;
	public:
		const vector<EntityId>& getEdges() const {
			return m_edges;
		}
		int addEdge(EntityId edgeid) {
			m_edges.push_back(edgeid);
			return 0;
		}
	private:
		Part() {};
		~Part() {};
		Part(const Part&);
		Part& operator=(const Part&);
	private:
		vector<EntityId> m_edges;  //CounterClockwise ordered list of edges
	};


	//singleton class 
	//creates, manages and destroys all entities
	class EntityManager {
	public:
		static int initialize();
		static int terminate();
	
		static const Part& getPart();
		static Entity& getEntity(EntityId id);
		
		static Vertex& addVertex(EntityId id, double x, double y);
		static LineSegment& addLineSegment(EntityId id, EntityId start, EntityId end);
		static CircularArc& addCircularArc(EntityId id, EntityId start, EntityId end, 
			Point center, EntityId from);
		static int addEdge(EntityId id);

	private:
		EntityManager() {};
		EntityManager(const EntityManager&);
		EntityManager& operator=(const EntityManager&);

		unordered_map<EntityId, Entity*> m_entityMap;
		Part m_part;
		static EntityManager m_entityManager;
	};

};

