/*
* -MonteCarlo methods definition & implementations.
* -Number of Tests using various configurations.
*/
#include <algorithm>
#include <chrono>
#include <iostream>
#include "montecarlo.h"
#include "graph.h"
#include "shortestpath.h"
using namespace std;

const int NUM_NODES = 50;
const pair<double, double> DIST_RANGE(1.0, 10.0);
const int NUM_TESTS = 10000;

//constructor
MonteCarlo::MonteCarlo()
{}

//run number of tests for given edge density and finds average shortest path.
int MonteCarlo::runTest(double edgedensity, int numtests, double& averageshortestpath)
{
    int nodeid0 = 0;
    double totalsp = 0.0;
    for (int i=0; i < numtests; i++)
    {
        Graph graph(NUM_NODES);
        graph.initialize(edgedensity, DIST_RANGE);

        ShortestPath shortestpath(graph);
        double testavgsp = 0.0;
        int pathsfound = 0;
        for (int j=1; j<NUM_NODES; j++)
        {
            vector<int> path;
            double spdist = 0.0;
            int res = shortestpath.findPath(nodeid0, j, path, spdist);
            if (!res) 
            {
                testavgsp += spdist;
                pathsfound++;
            }
        }

        if (pathsfound != 0 )
        {
            testavgsp = testavgsp/pathsfound;
            totalsp += testavgsp;
        }
    }
    averageshortestpath = totalsp/numtests;

    return 0;
}

//run all the test configurations
int MonteCarlo::runTest()
{
    chrono::steady_clock::time_point timestart = chrono::steady_clock::now();
    double averagesp20 = 0.0;
    int res = runTest(0.2, NUM_TESTS, averagesp20);
    chrono::steady_clock::time_point timeend = chrono::steady_clock::now();
    cout << endl << "Graph with 50 nodes" << endl;
    cout << endl << "Average Shortest Path for edge density 20% :" << averagesp20 << endl;
    cout << "Time taken for " << NUM_TESTS << " tests: " << 
        chrono::duration_cast<chrono::seconds>(timeend-timestart).count() << " seconds" << endl;

    timestart = chrono::steady_clock::now();
    double averagesp40 = 0.0;
    res = runTest(0.4, NUM_TESTS, averagesp40);
    timeend = chrono::steady_clock::now();
    cout << endl << "Average Shortest Path for edge density 40% :" << averagesp40 << endl;
    cout << "Time taken for " << NUM_TESTS << " tests: " << 
        chrono::duration_cast<chrono::seconds>(timeend-timestart).count() << " seconds" << endl;

    return 0;
}
