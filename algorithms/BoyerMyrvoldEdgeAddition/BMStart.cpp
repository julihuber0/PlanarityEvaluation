#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <iostream>
#include "BoyerMyrvoldEdgeAddition.h"
#include <ogdf/planarity/BoothLueker.h>
#include <chrono>

using namespace ogdf;
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

void checkInputPlanar(const string &input) {
    Graph G;
    GraphAttributes GA(G,
                       GraphAttributes::nodeGraphics |
                       GraphAttributes::edgeGraphics |
                       GraphAttributes::nodeLabel |
                       GraphAttributes::edgeStyle |
                       GraphAttributes::nodeStyle |
                       GraphAttributes::nodeTemplate);
    GA.directed() = false;
    if (!GraphIO::read(GA, G, input, GraphIO::readGML)) {
        cout << "Read failed" << endl;
        return;
    }
    auto start_time = Clock::now();
    BoyerMyrvoldEdgeAddition b(G);
    bool isPlanar = b.embed();
    auto end_time = Clock::now();
    cout << isPlanar << endl;
    cout << chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count() << endl;
}

void checkRandomPlanarGraph(int low_n, int high_n, int iterations, bool addEdge, bool compare) {
    bool correct = false;
    for (int i = low_n; i <= high_n; ++i) {
        for (int j = 0; j < iterations; ++j) {
            Graph gd;
            randomPlanarConnectedGraph(gd, i, 2 * i);
            if (addEdge) {
                node v = gd.chooseNode([&](node x) {
                    return x->degree() < gd.numberOfNodes() - 1;
                });

                node u = gd.chooseNode([&](node x) {
                    if (x == v) return false;
                    for (adjEntry adj: v->adjEntries) {
                        if (adj->twinNode() == x) return false;
                    }
                    return true;
                });
                gd.newEdge(v, u);
            }

            auto start_time = Clock::now();
            BoyerMyrvoldEdgeAddition b(gd);
            bool isPlanar = b.embed();
            auto end_time = Clock::now();
            if (compare) {
                correct = true;
                BoothLueker BL;
                bool bl = BL.isPlanar(gd);
                if ((isPlanar && !bl) || (!isPlanar && bl)) {
                    GraphIO::write(gd, "failed/n_" + to_string(i) + "_m_" + to_string(2*i) + "_it_" + to_string(j) + ".gml", GraphIO::writeGML);
                    cout << "Error with " << i << " vertices and " << 2 * i << " edges in iteration " << j << endl;
                    correct = false;
                }
            } else {
                cout << isPlanar << endl;
                cout << chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count() << endl;
            }
        }
    }
    if (correct) {
        cout << "Success" << endl;
    }
}

int main(int argc, char *argv[]) {

    if (argc == 2) {
        checkInputPlanar(argv[1]);
    } else if (argc == 6) {
        int low_n, high_n, iterations;
        bool addEdge, compare;

        low_n = stoi(argv[1]);
        high_n = stoi(argv[2]);
        iterations = stoi(argv[3]);
        addEdge = stoi(argv[4]);
        compare = stoi(argv[5]);
        checkRandomPlanarGraph(low_n, high_n, iterations, addEdge, compare);
    } else {
        checkRandomPlanarGraph(100, 100, 10, true, true);
    }

    return 0;
}

