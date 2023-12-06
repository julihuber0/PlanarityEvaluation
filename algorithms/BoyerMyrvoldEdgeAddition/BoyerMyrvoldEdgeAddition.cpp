//
// Created by huber on 29.11.23.
//

#include "BoyerMyrvoldEdgeAddition.h"

namespace ogdf {

    BoyerMyrvoldEdgeAddition::BoyerMyrvoldEdgeAddition(Graph &g): theGraph(g), sourceGraph(g) {
    }

    int BoyerMyrvoldEdgeAddition::gp_CreateDFSTree() {
        int DFI = 0;
        node I, uparent, u, e, J;

        if(theGraph.internalFlags & FLAGS_DFSNUMBERED) return OK;
        //TODO Clear stack

        for(node n : sourceGraph.nodes) {
            theGraph.vertexData[n].visited = 0;
        }

        for(node n : sourceGraph.nodes) {
            if (DFI >= sourceGraph.numberOfEdges()) break;
            if (theGraph.vertexData[n].DFSParent != nullptr) continue;

            theGraph.theStack.push(nullptr);
            theGraph.theStack.push(nullptr);
            while(theGraph.theStack.top() != nullptr) {
                e = theGraph.theStack.top();
                uparent = theGraph.theStack.top();
                u = uparent == nullptr ? n : e;

                if(!theGraph.vertexData[u].visited) {
                    theGraph.vertexData[u].visited = 1;
                    theGraph.vertexData[u].v = DFI++;
                    theGraph.vertexData[u].DFSParent = uparent;
                    if(e != nullptr) {
                        theGraph.vertexData[e].type = EDGE_DFSCHILD;

                        theGraph.vertexData[theGraph.vertexData[e].link[0]].link[1] = theGraph.vertexData[e].link[1];
                        theGraph.vertexData[theGraph.vertexData[e].link[1]].link[0] = theGraph.vertexData[e].link[0];

                        theGraph.vertexData[e].link[0] =theGraph.vertexData[uparent].link[0];
                        theGraph.vertexData[e].link[1] = uparent;

                        theGraph.vertexData[uparent].link[0] = e;
                        theGraph.vertexData[theGraph.vertexData[e].link[0]].link[1] = e;
                    }

                    J = theGraph.vertexData[u].link[0];
                    while (true) {
                        //TODO
                    }
                } else {
                    if(theGraph.vertexData[uparent].v < theGraph.vertexData[u].v) {
                        theGraph.vertexData[e].type = EDGE_FORWARD;

                        theGraph.vertexData[theGraph.vertexData[e].link[0]].link[1] = theGraph.vertexData[e].link[1];
                        theGraph.vertexData[theGraph.vertexData[e].link[1]].link[0] = theGraph.vertexData[e].link[0];

                        theGraph.vertexData[e].link[0] = uparent;
                        theGraph.vertexData[e].link[1] = theGraph.vertexData[uparent].link[1];

                        theGraph.vertexData[uparent].link[1] = e;
                        theGraph.vertexData[theGraph.vertexData[e].link[1]].link[0] = e;
                    } else if (theGraph.vertexData[theGraph.gp_GetTwinArc(e)].type == EDGE_FORWARD) {
                        theGraph.vertexData[e].type = EDGE_DFSPARENT;
                    } else {
                        theGraph.vertexData[e].type = EDGE_BACK;
                    }
                }
            }
        }

        theGraph.internalFlags |= FLAGS_DFSNUMBERED;
        return OK;
    }

    void BoyerMyrvoldEdgeAddition::_CreateSortedSeparatedDFSChildLists() {


    }

    void BoyerMyrvoldEdgeAddition::_CreateFwdArcLists() {

    }

    void BoyerMyrvoldEdgeAddition::_CreateDFSTreeEmbedding() {

    }

    void BoyerMyrvoldEdgeAddition::_EmbedBackEdgeToDescendant(int RootSide, int RootVertex, int W,
                                                              int WPrevLink) {

    }

    int BoyerMyrvoldEdgeAddition::_GetNextVertexOnExternalFace(int curVertex, int *pPrevLink) {

    }

    void BoyerMyrvoldEdgeAddition::_InvertVertex(int V) {}

    void BoyerMyrvoldEdgeAddition::_SetSignOfChildEdge(int V, int sign) {}

    void BoyerMyrvoldEdgeAddition::_MergeVertex(int W, int WPrevLink, int R) {}

    void BoyerMyrvoldEdgeAddition::_MergeBicomps() {}

    void BoyerMyrvoldEdgeAddition::_RecordPertinentChildBicomp(int I, int RootVertex) {}

    node BoyerMyrvoldEdgeAddition::_GetPertinentChildBicomp(node W) {
        node RootId;

        /* If the bicomp list is empty, then we just return NIL */

        if ((RootId = theGraph.vertexData[W].pertinentBicompList) == nullptr)
            return nullptr;

        /* Return the RootVertex, which is computed by adding N because we
            subtracted N before storing it in the bicomp list */

        return RootId + theGraph.N;
    }

    void BoyerMyrvoldEdgeAddition::_WalkUp(int I, int W) {}

    void BoyerMyrvoldEdgeAddition::_WalkDown(int I, int RootVertex) {}

    int BoyerMyrvoldEdgeAddition::gp_Embed(int embedFlags) {}

    void BoyerMyrvoldEdgeAddition::_OrientVerticesInEmbedding() {}

    void BoyerMyrvoldEdgeAddition::_OrientVerticesInBicomp(int BicompRoot, int PreserveSigns) {}

    int BoyerMyrvoldEdgeAddition::_JoinBicomps() {}

    void _initGraphAttributes(Graph &G) {
    }
} // ogdf