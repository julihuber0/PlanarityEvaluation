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

    Graph gd;
    randomPlanarConnectedGraph(gd, 100, 200);
    node v = gd.chooseNode([&](node x) {
        return x->degree() < gd.numberOfNodes() -1;
    });

    node u = gd.chooseNode([&](node x) {
        if (x == v) return false;
        for (adjEntry adj : v->adjEntries) {
            if (adj->twinNode() == x) return false;
        }
        return true;
    });

    BoothLueker BL;
    //cout << BL.isPlanar(gd) << endl;
    //gd.newEdge(v, u);
    //cout << BL.isPlanar(gd) << endl;

    BoyerMyrvoldEdgeAddition b(gd);
    int r = b.gp_Embed();
    if (r == 0) {
        cout << "Graph is planar" << endl;
    } else if (r == -3) {
        cout << "Graph is not planar" << endl;
    } else {
        cout << "Something went wrong" << endl;
    }


    return 0;
}

