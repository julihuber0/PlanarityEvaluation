#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>

using namespace ogdf;

int main()
{
    for (int i = 1; i < 21; ++i) {
        Graph G;
        GraphAttributes GA(G,
                           GraphAttributes::nodeGraphics |
                           GraphAttributes::edgeGraphics |
                           GraphAttributes::nodeLabel |
                           GraphAttributes::edgeStyle |
                           GraphAttributes::nodeStyle |
                           GraphAttributes::nodeTemplate);
        GraphIO::read(GA,G, "graph_generation/random" + std::to_string(i) + ".gml", GraphIO::readGML);
        BoyerMyrvold b;
        bool planar = b.isPlanar(G);
        printf("Graph %d: %d\n", i, planar);
    }



    return 0;
}
