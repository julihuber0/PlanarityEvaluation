//
// Created by huber on 01.12.23.
//

#include "BMGraphAttributes.h"

namespace ogdf {

    BMGraphAttributes::BMGraphAttributes(ogdf::Graph &g) :
            vertexData(g),
            rootVertexData(g),
            edgeData(g)
    /*N(g.numberOfNodes()),
    M(g.numberOfEdges()),
    embedFlags(-1),
    internalFlags(-1),
    IC(initIC)*/
    {
        dfi_sorted.reserve(g.numberOfEdges());
        buckets.reserve(g.numberOfNodes());
    }

    void BMGraphAttributes::_fillVisitedFlags(bool b, Graph & g) {
        for (node n: g.nodes) {
            vertexData[n].visited = b;
        }
    }

}
