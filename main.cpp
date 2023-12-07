#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>

using namespace ogdf;
using namespace std;

int main()
{
        Graph G;
        GraphAttributes GA(G,
                           GraphAttributes::nodeGraphics |
                           GraphAttributes::edgeGraphics |
                           GraphAttributes::nodeLabel |
                           GraphAttributes::edgeStyle |
                           GraphAttributes::nodeStyle |
                           GraphAttributes::nodeTemplate);
        GraphIO::read(GA,G, "graph_generation/K_4.gml", GraphIO::readGML);
        //NodeArray<int> n(G, 1);
        //Array<node> a(G.numberOfNodes());
        //printf("Index: %d\n", a[2]->degree());
        for(node n:G.nodes) {
            printf("Node %d\n", n->index());
            for(adjEntry a : n->adjEntries) {
                printf("Test: %d\n", a->twinNode()->index());
            }
        }

        BoyerMyrvold b;
        bool planar = b.isPlanar(G);
        printf("Graph: %d\n", planar);




    return 0;
}
