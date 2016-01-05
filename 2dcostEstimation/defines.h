#pragma once

namespace 2DEstimate {
	//given values for cost estimation
	const double VMAX = 0.5;
	const double MCOST = 0.75; //material cost
	const double CCOST = 0.07; //cutting cost
	const double PADDING = 0.1; 

	//distance tolerance 
	const double TOLERANCE = 0.00000001; //10^-8

	//number of sides for circumscribing polygon used for
	//circular arc hull calculations.
	//larger value give hull closer to circle but increases 
	//number of points for convex hull calculation
	const int NSIDES = 8;	

};
