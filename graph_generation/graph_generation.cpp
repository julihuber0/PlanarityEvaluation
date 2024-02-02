#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

using namespace ogdf;
using namespace std;

void generateRandomPlanarConnectedGraph(int n, int m, int id) {
    Graph G;
    randomPlanarConnectedGraph(G, n, m);
    GraphAttributes GA(G);
    GA.label(G.nodes.head()) = "random_planar_connected";
    GraphIO::write(GA, "randomPlanarConnected_" + toString(id) + ".gml", GraphIO::writeGML);
}

int main(int argc, char *argv[]) {

    if (argc == 4) {
        int n = stoi(argv[1]);
        int m = stoi(argv[2]);
        int id = stoi(argv[3]);
        generateRandomPlanarConnectedGraph(n, m, id);
    } else {
        cout << "Please provide valid arguments." << endl;
    }

    return 0;
}