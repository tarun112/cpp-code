#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "json/json.h"
#include "entity.h"
#include "entityManager.h"
#include "compute.h"
#include "costEstimation.h"

namespace Estimate {

	int CostEstimation::initialize() {	
		EntityManager::initialize();
		return 0;
	}


	int CostEstimation::terminate() {
		EntityManager::terminate();
		return 0;
	}

	//uses jsoncpp to parse JSON file
	int CostEstimation::readJson(string& fname) {

		Json::Value root;   // will contains the root value after parsing.
		Json::Reader reader;
		ifstream ifstrm;
		ifstrm.open(fname, ifstream::binary);
		if (!ifstrm.is_open()) {
			return 0;
		}

		bool parsingSuccessful = reader.parse(ifstrm, root);
		if (!parsingSuccessful)
		{
			cout << "Failed to parse JSON file\n"
				<< reader.getFormattedErrorMessages();
			return 0;
		}


		{
			const Json::Value vertices = root["Vertices"];
			const Json::Value::Members members = vertices.getMemberNames();
			for (unsigned int i = 0; i < members.size(); ++i)  // Iterates over the sequence elements.
			{
				EntityId id = stol(members[i]);
				Json::Value vertex = vertices[members[i]];
				double x = vertex["Position"]["X"].asDouble();
				double y = vertex["Position"]["Y"].asDouble();
				EntityManager::addVertex(id, x, y);
			}
		}

		{
			const Json::Value edges = root["Edges"];
			const Json::Value::Members members = edges.getMemberNames();
			for (unsigned int i = 0; i < members.size(); ++i)  // Iterates over the sequence elements.
			{
				EntityId id = stol(members[i]);
				Json::Value edge = edges[members[i]];
				Json::Value vertices = edge["Vertices"];
				EntityId start = static_cast<EntityId>(vertices[0].asLargestInt());
				EntityId end = static_cast<EntityId>(vertices[1].asLargestInt());
				string type = edge["Type"].asString();
				if (type == "LineSegment") {
					EntityManager::addLineSegment(id, start, end);
				}
				else if (type == "CircularArc") {
					double x = edge["Center"]["X"].asDouble();
					double y = edge["Center"]["Y"].asDouble();
					EntityId from = static_cast<EntityId>(edge["ClockwiseFrom"].asLargestInt());
					CircularArc& carc = EntityManager::addCircularArc(id, start, end, Point(x, y), from);
				}
				else
					assert(false);

				EntityManager::addEdge(id);
			}
		}

		ifstrm.close();

		return 0;
	}

	//For material cost minimum area bounding box (MABB) is used to get minimum area
	//to get MABB  convex hull & rotating calipers method is used
	int CostEstimation::compute(double& cost) {
		Calculations calc;
		calc.convexHull(EntityManager::getPart());
		calc.rotatingCalipers();
		cost = calc.computeCuttingCost(EntityManager::getPart());
		return 0;
	}
};
