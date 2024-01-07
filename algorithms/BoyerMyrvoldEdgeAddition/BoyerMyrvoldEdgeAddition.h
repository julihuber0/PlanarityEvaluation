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

#define EDGE_DFSCHILD           1
#define EDGE_FORWARD            2
#define EDGE_BACK               3
#define EDGE_DFSPARENT          4

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

        void _EmbedBackEdgeToDescendant(int, int, int, int);

        int _GetNextVertexOnExternalFace(int, int *);

        void _InvertVertex(int);

        void _SetSignOfChildEdge(int, int);

        void _MergeVertex(int, int, int);

        void _MergeBicomps();

        void _RecordPertinentChildBicomp(int, int);

        void _WalkUp(int, int);

        void _WalkDown(int, int);

        int gp_Embed(int);

        void _OrientVerticesInEmbedding();

        void _OrientVerticesInBicomp(int, int);

        int _JoinBicomps();

        node _GetPertinentChildBicomp(node);
    };


}// ogdf

#endif //PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H
