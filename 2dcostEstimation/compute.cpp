#include <set>
#include <algorithm>
using namespace std;

#include "defines.h"
#include "entity.h"
#include "entityManager.h"
#include "compute.h"
#include "utility.h"

namespace Estimate {


	int Calculations::convexHull(const Part& part) {
		double tcost = 0.0;
		set<EntityId> cnvxhull;
		const vector<EntityId>& edges = part.getEdges();
		for (unsigned int i = 0; i < edges.size(); ++i) {
			EntityId edgeid = edges[i];
			Edge& edge = reinterpret_cast<Edge&>(EntityManager::getEntity(edgeid));
			EntityId start = edge.getStart();
			EntityId end = edge.getEnd();
			Edge::Type type = edge.getType();
			if (type == Edge::Type::EdgeType_LineSegment) {
				cnvxhull.insert(start);
				cnvxhull.insert(end);
			}
			else if (type == Edge::Type::EdgeType_CircularArc) {
				EntityId cwfrom = reinterpret_cast<CircularArc&>(edge).getCWFrom();
				//If edgeloop is CCW & circular arc is CW from 'start' vertex then it is a 
				//cut arc and contibutes 'no' points in convec hull.
				//If edgeloop is CCW & circular arc is CW from 'end' vertex then it is an 
				//extrude arc so compute points for convec hull.
				if (cwfrom == end) {
					reinterpret_cast<CircularArc&>(edge).getCircumscribingPolygon(m_points);
				}
			}
		}

		for (auto it = cnvxhull.begin(); it != cnvxhull.end(); ++it) {
			Vertex& vrtx = reinterpret_cast<Vertex&>(EntityManager::getEntity(*it));
			m_points.push_back(vrtx.getPoint());
		}

		return convexHull();

	}
	

	int Calculations::rotatingCalipers() {
		vector<Point> out(3);
		rotatingCalipers(m_hpnts, out);
		Point pt1 = Point(out[1].first + out[0].first, out[1].second + out[0].second);
		Point pt2 = Point(out[2].first + out[0].first, out[2].second + out[0].second);
		m_bbx = getDistance(out[0], pt1);
		m_bby = getDistance(out[0], pt2);
		return 0;
	}


	double Calculations::computeCuttingCost(const Part& part) {
		double tcost = 0.0;
		const vector<EntityId>& edges = part.getEdges();
		for (unsigned int i = 0; i < edges.size(); ++i) {
			EntityId edgeid = edges[i];
			Edge& edge = reinterpret_cast<Edge&>(EntityManager::getEntity(edgeid));
			EntityId start = edge.getStart();
			EntityId end = edge.getEnd();
			Edge::Type type = edge.getType();
			if (type == Edge::Type::EdgeType_LineSegment) {
				double length = reinterpret_cast<LineSegment&>(edge).getLength();
				tcost += length *CCOST / VMAX;
			}
			else if (type == Edge::Type::EdgeType_CircularArc) {
				double length = reinterpret_cast<CircularArc&>(edge).getLength();
				double radius = reinterpret_cast<CircularArc&>(edge).getRadius();
				tcost += length*CCOST / (VMAX*pow(exp(1), -1.0 / radius));
			}
		}
		tcost += (m_bbx + PADDING)*(m_bby + PADDING)*MCOST;
		return tcost;
	}


	int Calculations::convexHull() {

		int sz = m_points.size();
		int indx = 0;

		sort(m_points.begin(), m_points.end(), 
			[](const Point& p0, const Point& p1) -> bool {
			if (p0.first < p1.first) 
				return true;
			else if (abs(p0.first-p1.first) < TOLERANCE)
				if (p0.second < p1.second)	
					return true;
			return false;	
		});

		m_hpnts.resize(2 * sz);

		for (int i = 0; i < sz; ++i) {
			while (indx >= 2 &&
				crossProduct(m_hpnts[indx-2], m_hpnts[indx-1], m_points[i]) <= 0)
				indx--;
			m_hpnts[indx++] = m_points[i];
		}

		int last = indx + 1;
		for (int i = sz-2; i >= 0; --i) {
			while (indx >= last &&
				crossProduct(m_hpnts[indx-2], m_hpnts[indx-1], m_points[i]) <= 0)
				indx--;
			m_hpnts[indx++] = m_points[i];
		}
		
		m_hpnts.resize(indx);
		
		return 0;
	}

//The following code for rotating callipers is adapted from OpenCV code
//It has been modified to remove void* usage and to substitute generic data structure to STL
//copyright note from OpenCV included as required.
/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

	int Calculations::rotatingCalipers(vector<Point>& points, vector<Point>& out)
	{
		int n = points.size();
		double minarea = FLT_MAX;
		double max_dist = 0;
		int buf0, buf5;
		double buf1, buf2, buf3, buf4, buf6;
		int i, k;
		vector<double> inv_vect_length(n);
		vector<Point> vect(n);
		int left = 0, bottom = 0, right = 0, top = 0;
		int seq[4] = { -1, -1, -1, -1 };

		// rotating calipers sides will always have coordinates
		//(a,b) (-b,a) (-a,-b) (b, -a)

		// this is a first base bector (a,b) initialized by (1,0) 
		double orientation = 0;
		double base_a;
		double base_b = 0;

		double left_x, right_x, top_y, bottom_y;
		Point pt0 = points[0];

		left_x = right_x = pt0.first;
		top_y = bottom_y = pt0.second;

		for (i = 0; i < n; i++)
		{
			double dx, dy;

			if (pt0.first < left_x)
				left_x = pt0.first, left = i;

			if (pt0.first > right_x)
				right_x = pt0.first, right = i;

			if (pt0.second > top_y)
				top_y = pt0.second, top = i;

			if (pt0.second < bottom_y)
				bottom_y = pt0.second, bottom = i;

			Point pt = points[(i + 1) & (i + 1 < n ? -1 : 0)];

			dx = pt.first - pt0.first;
			dy = pt.second - pt0.second;

			vect[i].first = (double)dx;
			vect[i].second = (double)dy;
			inv_vect_length[i] = (double)(1. / std::sqrt(dx*dx + dy*dy));

			pt0 = pt;
		}

		// find convex hull orientation
		{
			double ax = vect[n - 1].first;
			double ay = vect[n - 1].second;

			for (i = 0; i < n; i++)
			{
				double bx = vect[i].first;
				double by = vect[i].second;

				double convexity = ax * by - ay * bx;

				if (convexity != 0)
				{
					orientation = (convexity > 0) ? 1.f : (-1.f);
					break;
				}
				ax = bx;
				ay = by;
			}
			assert(orientation != 0);
		}
		base_a = orientation;

		//init calipers position                                        
		seq[0] = bottom;
		seq[1] = right;
		seq[2] = top;
		seq[3] = left;

		//Main loop - evaluate angles and rotate calipers               

		// all of edges will be checked while rotating calipers by 90 degrees 
		for (k = 0; k < n; k++)
		{
			// sinus of minimal angle 
			//double sinus;

			// compute cosine of angle between calipers side and polygon edge 
			// dp - dot product 
			double dp0 = base_a * vect[seq[0]].first + base_b * vect[seq[0]].second;
			double dp1 = -base_b * vect[seq[1]].first + base_a * vect[seq[1]].second;
			double dp2 = -base_a * vect[seq[2]].first - base_b * vect[seq[2]].second;
			double dp3 = base_b * vect[seq[3]].first - base_a * vect[seq[3]].second;

			double cosalpha = dp0 * inv_vect_length[seq[0]];
			double maxcos = cosalpha;

			// number of calipers edges, that has minimal angle with edge 
			int main_element = 0;

			// choose minimal angle 
			cosalpha = dp1 * inv_vect_length[seq[1]];
			maxcos = (cosalpha > maxcos) ? (main_element = 1, cosalpha) : maxcos;
			cosalpha = dp2 * inv_vect_length[seq[2]];
			maxcos = (cosalpha > maxcos) ? (main_element = 2, cosalpha) : maxcos;
			cosalpha = dp3 * inv_vect_length[seq[3]];
			maxcos = (cosalpha > maxcos) ? (main_element = 3, cosalpha) : maxcos;

			//rotate calipers
			{
				//get next base
				int pindex = seq[main_element];
				double lead_x = vect[pindex].first*inv_vect_length[pindex];
				double lead_y = vect[pindex].second*inv_vect_length[pindex];
				switch (main_element)
				{
				case 0:
					base_a = lead_x;
					base_b = lead_y;
					break;
				case 1:
					base_a = lead_y;
					base_b = -lead_x;
					break;
				case 2:
					base_a = -lead_x;
					base_b = -lead_y;
					break;
				case 3:
					base_a = -lead_y;
					base_b = lead_x;
					break;
				default:
					assert(false);
				}
			}
			// change base point of main edge 
			seq[main_element] += 1;
			seq[main_element] = (seq[main_element] == n) ? 0 : seq[main_element];

			// find area of rectangle 
			{
				double height;
				double area;

				// find vector left-right 
				double dx = points[seq[1]].first - points[seq[3]].first;
				double dy = points[seq[1]].second - points[seq[3]].second;

				// dotproduct 
				double width = dx * base_a + dy * base_b;

				// find vector left-right 
				dx = points[seq[2]].first - points[seq[0]].first;
				dy = points[seq[2]].second - points[seq[0]].second;

				// dotproduct 
				height = -dx * base_b + dy * base_a;

				area = width * height;
				if (area <= minarea)
				{
					minarea = area;
					// leftist point 
					buf0 = seq[3];
					buf1 = base_a;
					buf2 = width;
					buf3 = base_b;
					buf4 = height;
					// bottom point 
					buf5 = seq[0];
					buf6 = area;
				}
			}
		}                           // for 

		{
			double A1 = buf1;
			double B1 = buf3;

			double A2 = -buf3;
			double B2 = buf1;

			double C1 = A1 * points[buf0].first + points[buf0].second * B1;
			double C2 = A2 * points[buf5].first + points[buf5].second * B2;

			double idet = 1.f / (A1 * B2 - A2 * B1);

			double px = (C1 * B2 - C2 * B1) * idet;
			double py = (A1 * C2 - A2 * C1) * idet;

			out[0].first = px;
			out[0].second = py;

			out[1].first = A1 * buf2;
			out[1].second = B1 * buf2;

			out[2].first = A2 * buf4;
			out[2].second = B2 * buf4;
		}

		return 0;
	}

};
