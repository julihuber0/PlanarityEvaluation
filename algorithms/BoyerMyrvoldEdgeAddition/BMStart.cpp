#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>
#include <bits/stdc++.h>
#include "BoyerMyrvoldEdgeAddition.h"
#include <ogdf/planarity/BoothLueker.h>

#define NIL		(-1)
#define TYPE_UNKNOWN 8

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
        printf("Read failed");
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
    cout << BL.isPlanar(gd) << endl;
    gd.newEdge(v, u);
    cout << BL.isPlanar(gd) << endl;

    BoyerMyrvoldEdgeAddition b(gd);
    b.gp_CreateDFSTree();


    return 0;
}

