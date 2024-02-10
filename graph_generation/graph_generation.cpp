#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

using namespace ogdf;
using namespace std;

void generateRandomPlanarConnectedGraphs(int n, int m, int number) {
    for (int i = 1; i <= number; ++i) {
        Graph G;
        randomPlanarConnectedGraph(G, n, m);
        GraphAttributes GA(G, GraphAttributes::nodeLabel);
        GA.label(G.nodes.head()) = "random_planar_connected";
        GraphIO::write(GA, "randomPlanarConnected_" + to_string(n) + "_" + to_string(m) + "_" + to_string(i) + ".gml", GraphIO::writeGML);
    }
}

int main(int argc, char *argv[]) {

    if (argc == 4) {
        int n = stoi(argv[1]);
        int m = stoi(argv[2]);
        int number = stoi(argv[3]);
        generateRandomPlanarConnectedGraphs(n, m, number);
    } else {
        cout << "Please provide valid arguments." << endl;
    }

    return 0;
}