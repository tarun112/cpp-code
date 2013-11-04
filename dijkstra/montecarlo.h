/*
*  class MonteCarlo declaration
*  -Simulate Monte Carlo method using dijkstra algorithm to find
*  average shortest path distance for different edge densities in graph.
*/

#ifndef _MONTECARLO__H_
#define _MONTECARLO__H_

class MonteCarlo
{
public:
    MonteCarlo();

    //run all the test configurations
    int runTest();

    //run number of tests for given edge density and finds average shortest path.
    int runTest(double edgedensity, int numtests, double& averageshortestpath);

};

#endif  //_MONTECARLO__H_
