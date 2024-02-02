#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <iostream>
#include <ogdf/planarity/BoothLueker.h>
#include <chrono>

using namespace ogdf;
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

void checkInputPlanar(const string &input, const string &id) {
    Graph G;
    GraphAttributes GA(G, GraphAttributes::nodeLabel);
    if (!GraphIO::read(GA, G, input, GraphIO::readGML)) {
        cout << "Read failed" << endl;
        return;
    }
    string graphClass = GA.label(G.nodes.head());
    int n, m;
    n = G.numberOfNodes();
    m = G.numberOfEdges();
    BoothLueker BL;
    auto start_time = Clock::now();
    bool isPlanar = BL.isPlanar(G);
    auto end_time = Clock::now();
    cout << "BoothLueker," << isPlanar << "," << id << "," << graphClass << "," << n << "," << m << "," << n + m << "," << chrono::duration_cast<chrono::microseconds>(end_time - start_time).count() << endl;
}

int main(int argc, char *argv[]) {
    if (argc == 3) {
        checkInputPlanar(argv[1], argv[2]);
    } else {
        cout << "Please provide valid arguments." << endl;
    }
    return 0;
}
