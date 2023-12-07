//
// Created by huber on 29.11.23.
//

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/basic/Graph_d.h>
#include "BoyerMyrvoldEdgeAddition.h"

namespace ogdf {

    BoyerMyrvoldEdgeAddition::BoyerMyrvoldEdgeAddition(Graph &g): theGraph(g), sourceGraph(g) {
    }

    int BoyerMyrvoldEdgeAddition::gp_CreateDFSTree() {
        int DFI = 0;
        node I, uparent, u, J;
        edge e;

        //if (theGraph==nullptr) return NOTOK;
        //if(theGraph.internalFlags & FLAGS_DFSNUMBERED) return OK;
        stack<node> dfsNodes;
        stack<edge> dfsEdges;

        for(node n : sourceGraph.nodes) {
            theGraph.vertexData[n].visited = 0;
        }

        for(node n : sourceGraph.nodes) {
            if (DFI >= sourceGraph.numberOfEdges()) break;
            if (theGraph.vertexData[n].DFSParent != nullptr) continue;

            dfsNodes.push(nullptr);
            dfsEdges.push(nullptr);
            while(!dfsNodes.empty()) {
                e = dfsEdges.top();
                dfsEdges.pop();
                uparent = dfsNodes.top();
                dfsNodes.pop();
                u = uparent == nullptr ? n : e->opposite();

                if(!theGraph.vertexData[u].visited) {
                    theGraph.vertexData[u].visited = 1;
                    theGraph.vertexData[u].v = DFI++;
                    theGraph.vertexData[u].DFSParent = uparent;

                    if(e != nullptr) {
                        theGraph.edgeData[e].type = EDGE_DFSCHILD;

                        /*theGraph.vertexData[theGraph.vertexData[e].link[0]].link[1] = theGraph.vertexData[e].link[1];
                        theGraph.vertexData[theGraph.vertexData[e].link[1]].link[0] = theGraph.vertexData[e].link[0];

                        theGraph.vertexData[e].link[0] =theGraph.vertexData[uparent].link[0];
                        theGraph.vertexData[e].link[1] = uparent;

                        theGraph.vertexData[uparent].link[0] = e;
                        theGraph.vertexData[theGraph.vertexData[e].link[0]].link[1] = e;*/
                    }

                    for (adjEntry a : u->adjEntries) {
                        dfsNodes.push(a->twinNode());
                        dfsEdges.push(a->theEdge());
                    }
                } else {
                    edge twinEdge = e->adjSource()->theEdge();
                    if(theGraph.vertexData[uparent].v < theGraph.vertexData[u].v) {
                        theGraph.edgeData[e].type = EDGE_FORWARD;

                        /*theGraph.vertexData[theGraph.vertexData[e].link[0]].link[1] = theGraph.vertexData[e].link[1];
                        theGraph.vertexData[theGraph.vertexData[e].link[1]].link[0] = theGraph.vertexData[e].link[0];

                        theGraph.vertexData[e].link[0] = uparent;
                        theGraph.vertexData[e].link[1] = theGraph.vertexData[uparent].link[1];

                        theGraph.vertexData[uparent].link[1] = e;
                        theGraph.vertexData[theGraph.vertexData[e].link[1]].link[0] = e;*/
                    } else {
                        theGraph.edgeData[e].type = EDGE_BACK;
                    }
                }
            }
        }

        Graph::HiddenEdgeSet hiddenEdges(sourceGraph);
        for(edge ed : sourceGraph.edges) {
            if (theGraph.edgeData[ed].type != EDGE_DFSPARENT && theGraph.edgeData[ed].type != EDGE_DFSCHILD) {
                hiddenEdges.hide(ed);
            }
        }

        GraphAttributes GA(sourceGraph,
                           GraphAttributes::nodeGraphics |
                           GraphAttributes::edgeGraphics |
                           GraphAttributes::nodeLabel |
                           GraphAttributes::edgeStyle |
                           GraphAttributes::nodeStyle |
                           GraphAttributes::nodeTemplate);

        SugiyamaLayout SL;
        SL.setRanking(new OptimalRanking);
        SL.setCrossMin(new MedianHeuristic);

        OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
        ohl->layerDistance(30.0);
        ohl->nodeDistance(25.0);
        ohl->weightBalancing(0.8);
        SL.setLayout(ohl);

        SL.call(GA);
        //GraphIO::write(GA, "test.gml", GraphIO::writeGML);
        GraphIO::write(GA, "test.svg", GraphIO::drawSVG);

        theGraph.internalFlags |= FLAGS_DFSNUMBERED;
        return OK;
    }

    edge getTwinArc(edge & e) {
        adjEntry a = e->adjSource();
        return a->theEdge();
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