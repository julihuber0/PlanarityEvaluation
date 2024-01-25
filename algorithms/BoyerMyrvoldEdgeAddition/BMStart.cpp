#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <iostream>
#include "BoyerMyrvoldEdgeAddition.h"
#include <ogdf/planarity/BoothLueker.h>

using namespace ogdf;
using namespace std;

int main(int argc, char* argv[])
{
    Graph G;
    GraphAttributes GA(G,
                       GraphAttributes::nodeGraphics |
                       GraphAttributes::edgeGraphics |
                       GraphAttributes::nodeLabel |
                       GraphAttributes::edgeStyle |
                       GraphAttributes::nodeStyle |
                       GraphAttributes::nodeTemplate);
    GA.directed() = false;
    if(!GraphIO::read(GA, G, "graph_generation/K_4.gml", GraphIO::readGML)) {
        cout << "Read failed" << endl;
    }

    bool correct = true;
    for (int i = 0; i < 1000; ++i) {
        Graph gd;
        randomPlanarConnectedGraph(gd, 1000, 2000);
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

        BoothLueker BL;
        //cout << BL.isPlanar(gd) << endl;
        gd.newEdge(v, u);
        //cout << BL.isPlanar(gd) << endl;

        BoyerMyrvoldEdgeAddition b(gd);
        int r = b.gp_Embed();
        bool bl = BL.isPlanar(gd);
        /*if (r == 0) {
            cout << "Graph is planar" << endl;
        } else if (r == -3) {
            cout << "Graph is not planar" << endl;
        } else {
            cout << "Something went wrong" << endl;
        }*/
        if ((r == -3 && bl) || (r == 0 && !bl)) {
            cout << "Error with graph " << i << endl;
            correct = false;
        }
    }
    if (correct) {
        cout << "Success" << endl;
    }


    return 0;
}

