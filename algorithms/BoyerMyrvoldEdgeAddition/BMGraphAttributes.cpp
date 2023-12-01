//
// Created by huber on 01.12.23.
//

#include "BMGraphAttributes.h"

namespace ogdf {

    BMGraphAttributes::BMGraphAttributes(ogdf::Graph &g):
        vertexData(g),
        rootVertexData(g),
        edgeData(g),
        buckets(g),
        N(g.numberOfNodes()),
        M(g.numberOfEdges()),
        embedFlags(-1),
        internalFlags(-1),
        IC(initIC)
        {
            LCInit(DFSChildLists, g);
            LCInit(BicompLists, g);
            LCInit(bin, g);
        }

    void BMGraphAttributes::LCInit(listCollection& l, Graph& g) {
        l.nodeList.init(g);
        l.head = nullptr;
    };

}
