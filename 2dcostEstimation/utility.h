#pragma once

#include "entity.h"

namespace Estimate {

	double getDistance(const Vertex& vrtx0, const Vertex& vrtx1);
	double getDistance(EntityId vrtxid0, EntityId vrtxid1);

	double getDistance(Point p0, Point p1);
	double crossProduct(Point p0, Point p1, Point p2);
	double dotProduct(Point p0, Point p1, Point p2);
	double getAngle2(Point p0, Point p1, Point p2);
	
	Vector getVector(Point p0, Point p1);
	double dotProduct(Vector v0, Vector v1);
	double crossProduct(Vector v0, Vector v1);
	double getMagnitude(Vector v0);
	double getAngle2(Vector v0, Vector v1);
};
