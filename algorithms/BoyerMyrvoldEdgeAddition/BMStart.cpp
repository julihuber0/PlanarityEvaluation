#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>
#include <bits/stdc++.h>
#include "BoyerMyrvoldEdgeAddition.h"

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
    if(!GraphIO::read(GA, G, "graph_generation/K_4.gml", GraphIO::readGML)) {
        printf("Read failed");
    };
    cout << G.numberOfEdges();
    BoyerMyrvoldEdgeAddition b(G);
    b.gp_CreateDFSTree();
    return 0;
}

