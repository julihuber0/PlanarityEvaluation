#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/graph_generators.h>

using namespace ogdf;

int randomGraphs() {
    for (int i = 1; i <= 20; i++)
    {
        Graph G;
        randomGraph(G, 3 * i, (i+1)^2);
        GraphIO::write(G, "random" + std::to_string(i) + ".gml", GraphIO::writeGML);
    }

    return 0;
}

int main() {

    Graph K_4;
    completeGraph(K_4, 4);
    GraphIO::write(K_4, "K_4.gml", GraphIO::writeGML);

    Graph K_15;
    completeGraph(K_15, 15);
    GraphIO::write(K_15, "K_15.gml", GraphIO::writeGML);

    randomGraphs();

    return 0;
}