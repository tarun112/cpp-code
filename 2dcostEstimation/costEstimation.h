#pragma once

#include <string>
using namespace std;

namespace Estimate {

	class CostEstimation {
	public:
		int initialize();
		int readJson(string& fname);
		int compute(double& cost);
		int terminate();

		~CostEstimation() { terminate(); }
	};

};

