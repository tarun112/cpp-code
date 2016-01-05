#pragma once

#include <string>
using namespace std;

namespace 2DEstimate {

	class CostEstimation {
	public:
		int initialize();
		int readJson(string& fname);
		int compute(double& cost);
		int terminate();

		~CostEstimation() { terminate(); }
	};

};

