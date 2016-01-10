#include <cassert>
using namespace std;

#include "defines.h"
#include "entityManager.h"
#include "utility.h"

namespace Estimate {

	double getDistance(const Vertex& vrtx0, const Vertex& vrtx1) {
		pair<double, double> pnt0 = vrtx0.getPoint();
		pair<double, double> pnt1 = vrtx1.getPoint();
		return sqrt((pnt0.first - pnt1.first)*(pnt0.first - pnt1.first) +
			(pnt0.second - pnt1.second)*(pnt0.second - pnt1.second));
	}

	double getDistance(EntityId vrtxid0, EntityId vrtxid1) {
		Vertex& vrtx0 = reinterpret_cast<Vertex&>(EntityManager::getEntity(vrtxid0));
		Vertex& vrtx1 = reinterpret_cast<Vertex&>(EntityManager::getEntity(vrtxid1));
		return getDistance(vrtx0, vrtx1);
	}

	double getDistance(Point p0, Point p1) {
		return sqrt((p0.first - p1.first)*(p0.first - p1.first) +
			(p0.second - p1.second)*(p0.second - p1.second));
	}

	double crossProduct(Point p0, Point p1, Point p2) {
		return (p1.first - p0.first)*(p2.second - p0.second) -
			(p1.second - p0.second)*(p2.first - p0.first);
	}

	double dotProduct(Point p0, Point p1, Point p2) {
		return (p1.first - p0.first)*(p2.first - p0.first) +
			(p1.second - p0.second)*(p2.second - p0.second);
	}

	double getAngle2(Point p0, Point p1, Point p2) {
		double dp = dotProduct(p0, p1, p2);
		double d01 = getDistance(p0, p1);
		double d02 = getDistance(p0, p2);
		assert(abs(d01) > TOLERANCE);
		assert(abs(d02) > TOLERANCE);
		return acos(dp / (d01*d02));
	}

	Vector getVector(Point p0, Point p1) {
		return Vector(p1.first - p0.first, p1.second - p0.second);
	}

	double dotProduct(Vector v0, Vector v1) {
		return v0.first*v1.first + v0.second*v1.second;
	}

	double crossProduct(Vector v0, Vector v1) {
		return v0.first*v1.second - v0.second*v1.first;
	}

	double getMagnitude(Vector v0) {
		return sqrt(v0.first*v0.first + v0.second*v0.second);
	}

	double getAngle2(Vector v0, Vector v1) {
		double dp = dotProduct(v0, v1);
		double d0 = getMagnitude(v0);
		double d1 = getMagnitude(v1);
		assert(abs(d0) > TOLERANCE);
		assert(abs(d1) > TOLERANCE);
		return acos(dp / (d0*d1));
	}
};
