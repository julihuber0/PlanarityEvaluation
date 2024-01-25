//
// Created by huber on 29.11.23.
//
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>
#include <bits/stdc++.h>
#include "BMGraphAttributes.h"

#ifndef PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H
#define PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H

#define EDGE_DFSCHILD           1
#define EDGE_FORWARD            2
#define EDGE_BACK               3
#define EDGE_DFSPARENT          4

#define VAS_INACTIVE    0
#define VAS_INTERNAL    1
#define VAS_EXTERNAL    2

using namespace std;

namespace ogdf {

    class BoyerMyrvoldEdgeAddition {

    public: explicit BoyerMyrvoldEdgeAddition(Graph &g);
        typedef pair<node, bool> bNode;

        Graph& sourceGraph;

        BMGraphAttributes theGraph;

        void createDfsTree();

        void lowpointAndLeastAncestor();

        void createSortedSeparatedDfsChildLists();

        void createDfsTreeEmbedding();

        void embedBackEdgeToDescendant(int, bNode, bNode, int);

        int vertexActiveStatus(bNode, bNode);

        void invertVertex(bNode);

        void setSignOfChildEdge(bNode, int);

        void mergeVertex(bNode, int, bNode);

        void mergeBicomps();

        void recordPertinentChildBicomp(bNode, bNode);

        bool pertinent(bNode);

        void walkUp(bNode, bNode);

        void walkDown(bNode, bNode);

        bool embed();

        bNode getPertinentChildBicomp(bNode);
    };


}// ogdf

#endif //PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H
