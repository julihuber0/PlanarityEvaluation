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

#define NIL		(-1)
#define TYPE_UNKNOWN 8
#define OK 0
#define NONPLANAR -3

#define TYPE_UNKNOWN            0

#define EDGE_DFSCHILD           1
#define EDGE_FORWARD            2
#define EDGE_BACK               3
#define EDGE_DFSPARENT          4

#define VAS_INACTIVE    0
#define VAS_INTERNAL    1
#define VAS_EXTERNAL    2

#define VERTEX_HIGH_RXW         6
#define VERTEX_LOW_RXW          7
#define VERTEX_HIGH_RYW         8
#define VERTEX_LOW_RYW          9
using namespace std;

namespace ogdf {

    class BoyerMyrvoldEdgeAddition {

    public: explicit BoyerMyrvoldEdgeAddition(Graph &g);
        typedef pair<node, bool> bNode;

        Graph& sourceGraph;

        BMGraphAttributes theGraph;

        int gp_CreateDFSTree();

        void gp_LowpointAndLeastAncestor();

        void _CreateSortedSeparatedDFSChildLists();

        void _CreateDFSTreeEmbedding();

        void _EmbedBackEdgeToDescendant(int, bNode, bNode, int);

        int _VertexActiveStatus(bNode, bNode);

        void _InvertVertex(bNode);

        void _SetSignOfChildEdge(bNode, int);

        void _MergeVertex(bNode, int, bNode);

        void _MergeBicomps();

        void _RecordPertinentChildBicomp(bNode, bNode);

        bool pertinent(bNode);

        void _WalkUp(bNode, bNode);

        void _WalkDown(bNode, bNode);

        int gp_Embed();

        bNode _GetPertinentChildBicomp(bNode);
    };


}// ogdf

#endif //PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H
