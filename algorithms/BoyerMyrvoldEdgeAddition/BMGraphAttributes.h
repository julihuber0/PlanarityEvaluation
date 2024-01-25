//
// Created by huber on 01.12.23.
//
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

namespace ogdf {
    class BMGraphAttributes {

    public: explicit BMGraphAttributes(Graph &g);

        typedef struct {
            int dfi;
            int visited;
            pair<node, bool> visitor;
            node DFSParent;
            int leastAncestor, Lowpoint;
            vector<adjEntry> dfsChildArcs;
            list<node> pertinentBicompList, separatedDFSChildList;
            vector<int> nodeInParent;
            list<adjEntry> fwdArcList;
            ListPure<adjEntry> adjList;
            adjEntry parentArc, childArc, adjacentTo;
        } graphVertex;

        typedef struct {
            int type;
            bool isTargetRoot;
            int sign;
        } graphEdge;

        typedef struct {
            pair<node, bool> link[2];
            int inversionFlag;
        } extFaceLinkRec;

        public:
        typedef pair<node, bool> bNode;
        NodeArray<graphVertex> vertexData;
        NodeArray<graphVertex> rootVertexData;
        AdjEntryArray<graphEdge> edgeData;
        NodeArray<extFaceLinkRec> extFace, rootExtFace;
        vector<vector<node>> buckets;
        vector<pair<bNode, int>> theStack;
        vector<node> dfi_sorted;
        AdjEntryArray<list<adjEntry>::iterator> fwdListIters;
        NodeArray<list<node>::iterator> bicompListIters, sepDfsChildIters;

        void initGraph(Graph&);

        extFaceLinkRec& getExtFace(bNode);

        graphVertex& getVertexData(bNode);

        bNode getTarget(adjEntry);
    };
}
