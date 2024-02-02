#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <iostream>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <chrono>

using namespace ogdf;
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

void checkInputPlanar(const string &input) {
    Graph G;
    GraphAttributes GA;
    if (!GraphIO::read(GA, G, input, GraphIO::readGML)) {
        cout << "Read failed" << endl;
        return;
    }
    string graphClass = GA.label(G.nodes.head());
    int n, m;
    n = G.numberOfNodes();
    m = G.numberOfEdges();
    BoyerMyrvold BM;
    auto start_time = Clock::now();
    bool isPlanar = BM.isPlanar(G);
    auto end_time = Clock::now();
    cout << "BoyerMyrvold;" << isPlanar << ";" << graphClass << ";" << n << ";" << m << ";" << n + m << ";" << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        checkInputPlanar(argv[1]);
    } else {
        cout << "Please provide valid arguments." << endl;
    }
    return 0;
}