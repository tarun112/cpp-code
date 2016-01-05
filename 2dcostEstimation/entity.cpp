#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

#include "defines.h"
#include "entity.h"
#include "entityManager.h"
#include "utility.h"

namespace 2DEstimate {

	double LineSegment::getLength() {
		return getDistance(getStart(), getEnd());
	}

	
	double CircularArc::getRadius() {
		Point p0 = getCenter();
		Point p1 = reinterpret_cast<Vertex&>(EntityManager::getEntity(getCWFrom())).getPoint();
		return getDistance(p0, p1);
	}

	double CircularArc::getAngle() {
		Point p0 = getCenter();
		Point p1 = reinterpret_cast<Vertex&>(EntityManager::getEntity(getStart())).getPoint();
		Point p2 = reinterpret_cast<Vertex&>(EntityManager::getEntity(getEnd())).getPoint();
		double angle = getAngle2(p0, p1, p2);
		double cp = crossProduct(p0, p1, p2);

		double pi = 4 * atan(1.0);
		if (cp > 0 && getStart() == getCWFrom()) {
			angle = 2 * pi - angle;
		}
		if (cp < 0 && getEnd() == getCWFrom()) {
			angle = 2 * pi - angle;
		}
		return angle;
	}

	double CircularArc::getLength() {

		double angle = getAngle();
		double radius = getRadius();
		return radius*angle;
	}

	//to get convex hull points for circular arc
	//these points make a cicumscribed polygon with sides tangent to circular arc
	int CircularArc::getCircumscribingPolygon(vector<Point>& points) {
		double angle = getAngle();
		double radius = getRadius();

		double pi = 4.0 * atan(1.0);

		double theta = 2.0*pi / NSIDES;   //angle of each side at the center 
		double rad2 = radius / cos(theta/2.0); //radius of larger outer circle over and including polygon 

		Point p0 = getCenter();
		Point p1 = reinterpret_cast<Vertex&>(EntityManager::getEntity(getStart())).getPoint();
		Point p2 = reinterpret_cast<Vertex&>(EntityManager::getEntity(getEnd())).getPoint();
		Vector sv = getVector(p0, p1);
		Vector unitx = Vector(1, 0);
		double ang0 = getAngle2(unitx, sv);		//angle between x-axis and arc start point
		if (crossProduct(unitx, sv) < 0.0)
			ang0 = -ang0;

		points.push_back(p1);

		//get points on the outer circle at regular angles which will
		//make circumscribing polygon for initial circle
		double tang = ang0 + theta / 2.0;
		while (tang < ang0 + angle) {
			Point pnt;
			pnt.first = p0.first + rad2 *cos(tang);
			pnt.second = p0.second+ rad2 *sin(tang);
			points.push_back(pnt);
			tang += theta;
		}

		points.push_back(p2);

		return 0;
	}
};
