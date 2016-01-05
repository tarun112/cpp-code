#pragma once
#include <cassert>
#include <utility>
#include <vector>
using namespace std;

//all entities are derived from Entity
//entities are created and managed by EntityManager
//entities are identified by EntityId and can be obtained from EntityManager using it
namespace 2DEstimate {
	
	typedef long EntityId;

	typedef pair<double, double> Point;

	typedef pair<double, double> Vector;


	class Entity {
	protected:
		Entity(EntityId id) : m_id(id) {}
	private:
		Entity(const Entity&);
		Entity& operator=(const Entity&);

		EntityId m_id;
	};


	class Vertex : public Entity {
		friend class EntityManager;
	public:
		Point getPoint() const {
			return m_point;
		}
	
	protected:
		Vertex(EntityId id, Point point) : Entity(id), m_point(point) {}
	
	private:
		Vertex(const Vertex&);
		Vertex& operator=(const Vertex&);

		Point m_point;
	};


	class Edge : public Entity {
		friend class EntityManager;
	public:
		enum class Type {
			EdgeType_LineSegment = 0,
			EdgeType_CircularArc = 1
		};

		Type getType() { return m_type; }
		EntityId getStart() { return m_start; }
		EntityId getEnd() { return m_end; }
	
	protected:
		Edge(EntityId id, EntityId start, EntityId end, Type type) :
			Entity(id), m_start(start), m_end(end), m_type(type) {}
	private:
		Edge(const Edge&);
		Edge& operator=(const Edge&);

		EntityId m_start;
		EntityId m_end;
		Type m_type;
	};


	class LineSegment : public Edge {
		friend class EntityManager;
	public:
		double getLength();

	protected:
		LineSegment(EntityId id, EntityId start, EntityId end) :
			Edge(id, start, end, Edge::Type::EdgeType_LineSegment) {}
	
	private:
		LineSegment(const LineSegment&);
		LineSegment& operator=(const LineSegment&);
	};


	class CircularArc : public Edge {
		friend class EntityManager;
	public:
		EntityId getCWFrom() { return m_from; }
		Point getCenter() { return m_center; }
		double getLength();
		double getRadius();
		double getAngle();
		int getCircumscribingPolygon(vector<Point>& points);

	protected:
		CircularArc(EntityId id, EntityId start, EntityId end, Point center, EntityId from) :
			Edge(id, start, end, Edge::Type::EdgeType_CircularArc), m_center(center), m_from(from) {}
	private:
		CircularArc(const CircularArc&);
		CircularArc& operator=(const CircularArc&);

		Point m_center;
		EntityId m_from; //clocwise from 
	};

};
