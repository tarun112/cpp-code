#include "entity.h"
#include "entityManager.h"

namespace 2DEstimate {

	EntityManager EntityManager::m_entityManager;

	int EntityManager::initialize() {
		return 0; 
	}
	
	int EntityManager::terminate() {
		for (auto it = m_entityManager.m_entityMap.begin(); it != m_entityManager.m_entityMap.end(); ++it) {
			delete it->second;
			it->second = nullptr;
		}
		m_entityManager.m_entityMap.clear();
		return 0;
	}
	
	Entity& EntityManager::getEntity(EntityId id) {
		return *m_entityManager.m_entityMap[id];
	}
	
	Vertex& EntityManager::addVertex(EntityId id, double x, double y) {
		Vertex* entity = new Vertex(id, Point(x, y));
		m_entityManager.m_entityMap[id] = entity;
		return *entity;
	}
	
	LineSegment& EntityManager::addLineSegment(EntityId id, EntityId start, EntityId end) {
		LineSegment* entity = new LineSegment(id, start, end);
		m_entityManager.m_entityMap[id] = entity;
		return *entity;
	}
	
	CircularArc& EntityManager::addCircularArc(EntityId id, EntityId start, EntityId end, 
										Point center, EntityId from) {
		CircularArc* entity = new CircularArc(id, start, end, center, from);
		m_entityManager.m_entityMap[id] = entity;
		return *entity;
	}
	
	int EntityManager::addEdge(EntityId id) {
		m_entityManager.m_part.addEdge(id);
		return 0;
	}
	
	const Part& EntityManager::getPart() {
		return m_entityManager.m_part; 
	}
};
