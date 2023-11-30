#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/planarity/BoyerMyrvold.h>
#include <iostream>
#include <bits/stdc++.h>

#define NIL		(-1)
#define TYPE_UNKNOWN 8

using namespace ogdf;
using namespace std;

typedef struct
{
    int  v;
    int  visited;
    int  link[2];
    int  type;
    int  sign;
} graphNode;

graphNode initGraphNode = {NIL, 0, {NIL, NIL}, TYPE_UNKNOWN, 1};

typedef struct
{
    int DFSParent, leastAncestor, Lowpoint, adjacentTo;
    int pertinentBicompList, separatedDFSChildList, fwdArcList;
} vertexRec;

typedef vertexRec;

typedef struct
{
    int link[2];
    int inversionFlag;
} extFaceLinkRec;

#define FLAGS_DFSNUMBERED       1
#define FLAGS_SORTEDBYDFI       2

typedef struct
{
    int minorType;
    int v, r, x, y, w, px, py, z;
    int ux, dx, uy, dy, dw, uz, dz;
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

NodeArray<graphNode> graphNodeArray;

NodeArray<graphNode> graphRootNodeArray;

EdgeArray<graphNode> graphEdgeArray;

NodeArray<vertexRec> vertexRecNodeArray;

typedef struct {
    int prev, next;
} lcnode;

NodeArray<lcnode> BicompLists, DFSChildLists, bin;

NodeArray<int> buckets;

NodeArray<extFaceLinkRec> extFace;

stack<int> theStack;

int N, M, internalFlags, embedFlags;

void  _CreateSortedSeparatedDFSChildLists(Graph&);

void _CreateFwdArcLists(Graph &);

void _CreateDFSTreeEmbedding(Graph &);

void _EmbedBackEdgeToDescendant(Graph &, int, int, int, int);

int  _GetNextVertexOnExternalFace(Graph &, int, int *);

void _InvertVertex(Graph &, int);

void _SetSignOfChildEdge(Graph &, int, int);

void _MergeVertex(Graph &, int, int, int);

void _MergeBicomps(Graph &);

void _RecordPertinentChildBicomp(Graph &, int, int);

void _WalkUp(Graph &, int, int);

void _WalkDown(Graph &, int, int);

int gp_Embed(Graph &, int);

void _OrientVerticesInEmbedding(Graph &);

void _OrientVerticesInBicomp(Graph &, int, int);

int  _JoinBicomps(Graph &);




void  _CreateSortedSeparatedDFSChildLists(Graph &theEmbedding) {}

void _CreateFwdArcLists(Graph &theGraph) {}

void _CreateDFSTreeEmbedding(Graph &theGraph) {}

void _EmbedBackEdgeToDescendant(Graph &theGraph, int RootSide, int RootVertex, int W, int WPrevLink) {}

int  _GetNextVertexOnExternalFace(Graph &theEmbedding, int curVertex, int *pPrevLink) {}

void _InvertVertex(Graph &theEmbedding, int V) {}

void _SetSignOfChildEdge(Graph &theEmbedding, int V, int sign) {}

void _MergeVertex(Graph &theEmbedding, int W, int WPrevLink, int R) {}

void _MergeBicomps(Graph &theEmbedding) {}

void _RecordPertinentChildBicomp(Graph &theEmbedding, int I, int RootVertex) {}

#ifndef SPEED_MACROS

int _GetPertinentChildBicomp(int W)
{
    int  RootId;

    /* If the bicomp list is empty, then we just return NIL */

    if ((RootId=vertexRecNodeArray[W].pertinentBicompList) == NIL)
        return NIL;

    /* Return the RootVertex, which is computed by adding N because we
        subtracted N before storing it in the bicomp list */

    return RootId + N;
}

#else

#define _GetPertinentChildBicomp(W) \
        (vertexRecNodeArray[W].pertinentBicompList==NIL ? NIL : vertexRecNodeArray[W].pertinentBicompList + N)

#endif

void _WalkUp(Graph &theEmbedding, int I, int W) {}

void _WalkDown(Graph &theEmbedding, int I, int RootVertex) {}

int gp_Embed(Graph &theGraph, int embedFlags) {}

void _OrientVerticesInEmbedding(Graph &theEmbedding) {}

void _OrientVerticesInBicomp(Graph &theEmbedding, int BicompRoot, int PreserveSigns) {}

int  _JoinBicomps(Graph &theEmbedding) {}

void _initGraphAttributes(Graph &G) {
}

int main(int argc, char* argv[])
{
    Graph g;
    return 0;
}

