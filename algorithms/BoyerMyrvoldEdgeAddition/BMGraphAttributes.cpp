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

    void BMGraphAttributes::LCInit(listCollection &l, Graph &g) {
        l.nodeList.init(g);
        l.N = g.numberOfEdges();
    }

    void BMGraphAttributes::LCReset(listCollection &l) {
        for (auto & iterator : l.nodeList) {
            iterator.prev = iterator.next = nullptr;
            iterator.statusCode = 0;
        }
    }

    node BMGraphAttributes::LCGetNext(listCollection &listColl, node theList, node theNode) {
        node next = listColl.nodeList[theNode].next;
        return next == theList ? nullptr : next;
    }

    node BMGraphAttributes::LCGetPrev(listCollection &listColl, node theList, node theNode) {
        if (theList == nullptr) return nullptr;
        if (theNode == nullptr) return listColl.nodeList[theList].prev;
        if (theNode == theList) return nullptr;
        return listColl.nodeList[theNode].prev;
    }

    node BMGraphAttributes::LCAppend(listCollection &listColl, node theList, node theNode) {
        if (theList == nullptr) {
            listColl.nodeList[theNode].next = listColl.nodeList[theNode].prev = theNode;
            theList = theNode;
        } else {
            node pred = listColl.nodeList[theList].prev;

            listColl.nodeList[theList].prev = theNode;
            listColl.nodeList[theNode].next = theList;
            listColl.nodeList[theNode].prev = pred;
            listColl.nodeList[pred].next = theNode;
        }
        return theList;
    }

    node BMGraphAttributes::LCPrepend(listCollection &listColl, node theList, node theNode) {
        node newList = LCAppend(listColl, theList, theNode);
        newList = listColl.nodeList[newList].prev;
        return newList;
    }

    /*void BMGraphAttributes::LCCopy(listCollection & dst, listCollection & src) {
        for(node n: src.nodeList.graphOf()->nodes) {
            dst.nodeList[n] = src.nodeList[n];
        }
    }*/

    void BMGraphAttributes::initFwArcList(ogdf::Graph & g) {
        for (node n: g.nodes) {
            vertexData[n].fwdArcList.reserve(n->degree());
        }
    }

}
