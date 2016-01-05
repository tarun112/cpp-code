#pragma once
namespace 2DEstimate {

	class Calculations {
	public:
		int convexHull(const Part& part);
		int rotatingCalipers();
		double computeCuttingCost(const Part& part);

	private:
		int convexHull();
		int rotatingCalipers(vector<Point>& chull, vector<Point>& out);

	private:
		vector<Point> m_points;  //all points
		vector<Point> m_hpnts;   //convex hull points

		double m_cost = 0.0;
		double m_bbx = 0.0;
		double m_bby = 0.0;
	};

};
