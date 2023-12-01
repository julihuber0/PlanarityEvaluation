//
// Created by huber on 29.11.23.
//
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>
#include <bits/stdc++.h>

#ifndef PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H
#define PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H

#define NIL		(-1)
#define TYPE_UNKNOWN 8

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

        explicit BoyerMyrvoldEdgeAddition(Graph &g);

        typedef struct {
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
            node prev, next;
        } lcnode;

        typedef struct {
            NodeArray<lcnode> nodeList;
            node head;
        } listCollection;

        void LCInit(listCollection&);

        int LCAppend(listCollection&, node, node);

        int LCPrepend(listCollection&, node, node);

        int LCGetNext(listCollection&, node, node);

        int gp_GetTwinArc(node);

        typedef struct {
            NodeArray<graphVertex> G;
            NodeArray<graphVertex> graphRootNodeArray;
            EdgeArray<graphEdge> graphEdgeArray;
            isolatorContext IC;
            NodeArray<int> buckets;
            NodeArray<extFaceLinkRec> extFace;
            stack<node> theStack;
            int N, M, internalFlags, embedFlags;
            listCollection BicompLists, DFSChildLists, bin;
        }graphData;

        graphData *theGraph, *theEmbedding;

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

        int _GetPertinentChildBicomp(int);
    };


}// ogdf

#endif //PLANARITYEVALUATIONFRAMEWORK_BOYERMYRVOLDEDGEADDITION_H
