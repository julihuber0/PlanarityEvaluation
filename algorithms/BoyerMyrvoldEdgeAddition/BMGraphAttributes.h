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
            int dfi;
            int visited;
            node DFSParent;
            bool isRoot;
            int leastAncestor, Lowpoint;
            vector<adjEntry> dfsChildArcs;
            list<node> pertinentBicompList, separatedDFSChildList;
            vector<int> nodeInParent;
            list<adjEntry> fwdArcList;
            ListPure<adjEntry> adjList, rootAdjList;
            adjEntry parentArc, childArc, adjacentTo;
        } graphVertex;

        typedef struct {
            int type;
            int sign;
        } graphEdge;

        typedef struct {
            node link[2];
            int inversionFlag;
        } extFaceLinkRec;

#define FLAGS_DFSNUMBERED       1
#define FLAGS_SORTEDBYDFI       2

        public:
        NodeArray<graphVertex> vertexData;
        NodeArray<graphVertex> rootVertexData;
        AdjEntryArray<graphEdge> edgeData;
        NodeArray<extFaceLinkRec> extFace, rootExtFace;
        vector<vector<node>> buckets;
        vector<pair<node, int>> theStack;
        int N, internalFlags;
        vector<node> dfi_sorted;
        AdjEntryArray<list<adjEntry>::iterator> fwdListIters;
        NodeArray<list<node>::iterator> bicompListIters, sepDfsChildIters;

        void _fillVisitedFlags(bool, Graph &);

        void initGraph(Graph&);
    };
}

#endif //PLANARITYEVALUATIONFRAMEWORK_BMGRAPHATTRIBUTES_H
