//
// Created by huber on 01.12.23.
//
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>
#include <bits/stdc++.h>

#ifndef PLANARITYEVALUATIONFRAMEWORK_BMGRAPHATTRIBUTES_H
#define PLANARITYEVALUATIONFRAMEWORK_BMGRAPHATTRIBUTES_H

#define NIL -1
#define NOTOK -2

using namespace std;

namespace ogdf {
    class BMGraphAttributes {

    public: explicit BMGraphAttributes(Graph &g);

        BMGraphAttributes();

        typedef struct {
            int v;
            int visited;
            node link[2];
            int type;
            node DFSParent, leastAncestor, Lowpoint, adjacentTo;
            node pertinentBicompList, separatedDFSChildList, fwdArcList;
        } graphVertex;

        typedef struct {
            int visited;
            edge link[2];
            int type;
            int sign;
        } graphEdge;

        typedef struct {
            node link[2];
            int inversionFlag;
        } extFaceLinkRec;

#define FLAGS_DFSNUMBERED       1
#define FLAGS_SORTEDBYDFI       2

        typedef struct {
            int minorType;
            node v, r, x, y, w, px, py, z;
            node ux, dx, uy, dy, dw, uz, dz;
        } isolatorContext;

        isolatorContext initIC = {-1, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

#define FLAGS_MINOR_A         1
#define FLAGS_MINOR_B         2
#define FLAGS_MINOR_C         4
#define FLAGS_MINOR_D         8
#define FLAGS_MINOR_E         16
#define FLAGS_MINOR_E1        32
#define FLAGS_MINOR_E2        64
#define FLAGS_MINOR_E3        128
#define FLAGS_MINOR_E4        256

#define FLAGS_MINOR_E5        512
#define FLAGS_MINOR_E6        1024
#define FLAGS_MINOR_E7        2048

        typedef struct {
            node next, prev;
            int statusCode;
        } lcnode;

        typedef struct {
            NodeArray<lcnode> nodeList;
            node head;
        } listCollection;

        public:
        NodeArray<graphVertex> vertexData;
        NodeArray<graphVertex> rootVertexData;
        EdgeArray<graphEdge> edgeData;
        isolatorContext IC;
        NodeArray<int> buckets;
        NodeArray<extFaceLinkRec> extFace;
        stack<node> theStack;
        int N, M, internalFlags, embedFlags;
        listCollection BicompLists, DFSChildLists, bin;

        vector<node> dfi_sorted;

        static void LCInit(listCollection&, Graph&);

        node LCAppend(listCollection&, node, node);

        node LCPrepend(listCollection&, node, node);

        node LCGetNext(listCollection&, node, node);

        node LCGetPrev(listCollection&, node, node);

        void LCCopy(listCollection&, listCollection&);

        node gp_GetTwinArc(node);

    };
}

#endif //PLANARITYEVALUATIONFRAMEWORK_BMGRAPHATTRIBUTES_H
