//
// Created by huber on 01.12.23.
//

#include "BMGraphAttributes.h"

namespace ogdf {

    BMGraphAttributes::BMGraphAttributes(ogdf::Graph &g) :
            vertexData(g),
            rootVertexData(g),
            edgeData(g),
            extFace(g),
            rootExtFace(g),
            fwdListIters(g),
            bicompListIters(g),
            sepDfsChildIters(g),
            N(g.numberOfNodes()),
            dfi_sorted(g.numberOfNodes()),
            buckets(g.numberOfNodes()),
            internalFlags(-1) {
        initGraph(g);
    }

    void BMGraphAttributes::_fillVisitedFlags(bool b, Graph & g) {
        for (node n: g.nodes) {
            vertexData[n].visited = b;
        }
    }

    void BMGraphAttributes::initGraph(Graph & g) {
        int nc = 0;
        for(node n: g.nodes) {
            vertexData[n].Lowpoint = vertexData[n].leastAncestor = nc++;
            vertexData[n].visited = false;
            vertexData[n].adjacentTo = nullptr;
            vertexData[n].DFSParent = nullptr;
            for (adjEntry a: n->adjEntries) {
                edgeData[a].type = 0;
                edgeData[a].sign = 1;
            }
        }
    }

}
