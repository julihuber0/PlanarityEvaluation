#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>

using namespace ogdf;

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
        GraphIO::read(GA,G, argv[1], GraphIO::readGML);
        BoyerMyrvold b;
        bool planar = b.isPlanar(G);
        if(planar) {
            printf("Graph is planar\n");
        } else {
            printf("Graph is not planar\n");
        }

    return 0;
}
