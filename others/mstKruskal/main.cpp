/*
* -main function
*/
#include <chrono>
#include <iostream>
#include "graph.h"
#include "mstkruskal.h"
using namespace std;

int main()
{
    //exception handling as some stl methods (for example map.at) throw exceptions
    //also Graph throws exception in constructor if cannot read input file.
    try {
        //chrono used to get the time taken by the process
        chrono::steady_clock::time_point timestart = chrono::steady_clock::now();

        //initialize graph with data from the provided file.
        Graph graph("sampledata.txt");

        MinSpanTreeKruskal minspantree(graph);

        double mstcost = 0.0;
        vector<int> mstedges;
        minspantree.findTree(mstedges, mstcost);

        chrono::steady_clock::time_point timeend = chrono::steady_clock::now();

        //output the results
        cout << endl << "Minimum Spanning Tree total cost : " << mstcost << endl;
        cout << endl << "Minimum Spanning Tree edges (with costs):" << endl;
        for (auto it = mstedges.begin(); it != mstedges.end(); it++)
        {
            int edgeid = *it;
            int nodeid0, nodeid1;
            graph.getEdgeNodes(edgeid, nodeid0, nodeid1);

            double edgecost;
            graph.getEdgeDistance(edgeid, edgecost);

            cout << " " << nodeid0 << " - " << nodeid1 << "  (" << edgecost << ")" << endl;
        }
        cout << endl << "Time taken : " << 
            chrono::duration_cast<chrono::milliseconds>(timeend-timestart).count() << " milliseconds" << endl;

    }
    catch( string & msg)
    {
        cerr << msg.c_str() << endl;
    }
    return 0;
}
