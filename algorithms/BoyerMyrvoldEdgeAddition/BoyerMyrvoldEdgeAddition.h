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

        Graph& sourceGraph;

        BMGraphAttributes theGraph;

        edge getTwinArc(edge&);

        int gp_CreateDFSTree();

        void gp_SortVertices();

        void gp_LowpointAndLeastAncestor();

        void _CreateSortedSeparatedDFSChildLists();

        void _CreateFwdArcLists();

        void _CreateDFSTreeEmbedding();

        void _EmbedBackEdgeToDescendant(int, node, node, int);

        int _VertexActiveStatus(node, node);

        int _GetNextVertexOnExternalFace(node, int &);

        void _InvertVertex(node);

        void _SetSignOfChildEdge(node, int);

        void _MergeVertex(node, int, node);

        void _MergeBicomps();

        void _RecordPertinentChildBicomp(node, node);

        bool pertinent(node);

        void _WalkUp(node, node);

        void _WalkDown(node, node);

        int gp_Embed(int);

        void _OrientVerticesInEmbedding();

        void _OrientVerticesInBicomp(int, int);

        int _JoinBicomps();

        node _GetPertinentChildBicomp(node);
    };


}// ogdf

#endif //PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H
