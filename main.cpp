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
    Graph G;
    GraphAttributes GA(G,
                       GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics);

    const int LEN = 5;
    node nodes[5];
    for (int i = 0; i < LEN; ++i)
    {
        nodes[i] = G.newNode();
    }
    for (int i = 0; i < LEN; ++i)
    {
        for (int j = 0; j < LEN; ++j)
        {
            if (j != i)
            {
                G.newEdge(nodes[i], nodes[j]);
            }
        }
    }

    SugiyamaLayout SL;
    SL.setRanking(new OptimalRanking);
    SL.setCrossMin(new MedianHeuristic);
 
    OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
    ohl->layerDistance(30.0);
    ohl->nodeDistance(25.0);
    ohl->weightBalancing(0.8);
    SL.setLayout(ohl);
 
    SL.call(GA);
    GraphIO::write(GA, "output-unix-history-hierarchical.gml", GraphIO::writeGML);
    GraphIO::write(GA, "output-unix-history-hierarchical.svg", GraphIO::drawSVG);

    BoyerMyrvold bm;

    bool isPlanar = bm.isPlanar(G);

    printf("Planar: %d\n", isPlanar);

    return 0;
}
