#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "costEstimation.h"


int main(int argc, const char* argv[]) {
	if (argc < 2) {
		cout << "No input arguments" << endl;
		cout << "Usage: " << endl;
		cout << " CostEstimation.exe  filename.json" << endl;
		return 0;
	}

	2DEstimate::CostEstimation estimate;

	int stat = 0;
	stat = estimate.initialize();

	stat = estimate.readJson(string(argv[1]));
	if (stat)
		return 0;

	double cost = 0.0;
	stat = estimate.compute(cost);
	if (stat)
		return 0;
	
	cout << "Cost Estimate is : " << fixed << setprecision(2) << cost << endl;

	stat = estimate.terminate();

	return 0;
}
