//
// Created by huber on 29.11.23.
//

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/basic/Graph_d.h>
#include "BoyerMyrvoldEdgeAddition.h"

namespace ogdf {

    BoyerMyrvoldEdgeAddition::BoyerMyrvoldEdgeAddition(Graph &g) : theGraph(g), sourceGraph(g) {
    }

    int BoyerMyrvoldEdgeAddition::gp_CreateDFSTree() {
        int DFI = 0;
        node uparent, u;
        adjEntry e;

        //if (theGraph==nullptr) return NOTOK;
        //if(theGraph.internalFlags & FLAGS_DFSNUMBERED) return OK;
        stack<node> dfsNodes;
        stack<edge> dfsEdges;
        stack<pair<node, adjEntry>> dfsStack;

        for (node n: sourceGraph.nodes) {
            theGraph.vertexData[n].visited = 0;
        }

        for (node n: sourceGraph.nodes) {
            if (DFI >= sourceGraph.numberOfEdges()) break;
            if (theGraph.vertexData[n].DFSParent != nullptr) continue;
            theGraph.vertexData[n].dfi = DFI++;
            theGraph.vertexData[n].visited = 1;
            theGraph.vertexData[n].DFSParent = nullptr;
            for (adjEntry a: n->adjEntries) {
                dfsStack.emplace(a->twinNode(), a);
            }

            while (!dfsStack.empty()) {
                u = dfsStack.top().first;
                e = dfsStack.top().second;
                dfsStack.pop();
                uparent = e->theNode();

                if (!theGraph.vertexData[u].visited) {
                    theGraph.vertexData[u].visited = 1;
                    theGraph.vertexData[u].dfi = DFI++;
                    theGraph.vertexData[u].DFSParent = uparent;

                    theGraph.edgeData[e].type = EDGE_DFSCHILD;

                    for (adjEntry a: u->adjEntries) {
                        dfsStack.emplace(a->twinNode(), a);
                    }
                } else {
                    if (theGraph.vertexData[uparent].dfi < theGraph.vertexData[u].dfi) {
                        theGraph.edgeData[e].type = EDGE_FORWARD;
                        theGraph.vertexData[u].fwdArcList.push_back(e);
                    } else if (theGraph.edgeData[e->twin()].type == EDGE_DFSCHILD) {
                        theGraph.edgeData[e].type = EDGE_DFSPARENT;
                    } else {
                        theGraph.edgeData[e].type = EDGE_BACK;
                    }
                }
            }
        }

        Graph::HiddenEdgeSet hiddenEdges(sourceGraph);
        vector<edge> edgesV;
        for (edge ed: sourceGraph.edges) {
            edgesV.push_back(ed);
        }
        for (edge ed: edgesV) {
            if (theGraph.edgeData[ed->adjSource()].type != EDGE_DFSCHILD && theGraph.edgeData[ed->adjSource()].type != EDGE_DFSPARENT) {
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

        auto *ohl = new OptimalHierarchyLayout;
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

    void BoyerMyrvoldEdgeAddition::gp_SortVertices() {
        for (node n: sourceGraph.nodes) {
            theGraph.dfi_sorted[theGraph.vertexData[n].dfi] = n;
        }
    }

    void BoyerMyrvoldEdgeAddition::gp_LowpointAndLeastAncestor() {
        for (node n: sourceGraph.nodes) {
            theGraph.vertexData[n].visited = 0;
        }

        stack<node> stack;
        node u, L, leastAncestor, uneighbour;

        for (node n: theGraph.dfi_sorted) {
            if (theGraph.vertexData[n].visited) continue;
            stack.push(n);
            while (!stack.empty()) {
                u = stack.top();
                stack.pop();
                if (!theGraph.vertexData[u].visited) {
                    theGraph.vertexData[u].visited = 1;
                    stack.push(u);

                    for (adjEntry a: u->adjEntries) {
                        if (theGraph.vertexData[u].dfi - theGraph.vertexData[a->twinNode()].dfi == -1) {
                            stack.push(a->twinNode());
                        }
                    }
                } else {
                    L = leastAncestor = u;
                    for (adjEntry a: u->adjEntries) {
                        uneighbour = a->twinNode();
                        if (theGraph.vertexData[u].dfi - theGraph.vertexData[a->twinNode()].dfi == -1) {
                            if (L > theGraph.vertexData[uneighbour].Lowpoint) {
                                L = theGraph.vertexData[uneighbour].Lowpoint;
                            }
                        } else if (theGraph.vertexData[u].dfi > theGraph.vertexData[a->twinNode()].dfi) {
                            if (theGraph.vertexData[leastAncestor].dfi > theGraph.vertexData[uneighbour].dfi) {
                                leastAncestor = uneighbour;
                            }
                        } else if (theGraph.vertexData[u].dfi < theGraph.vertexData[a->twinNode()].dfi) {
                            break;
                        }
                    }
                    theGraph.vertexData[u].leastAncestor = leastAncestor;
                    theGraph.vertexData[u].Lowpoint = theGraph.vertexData[leastAncestor].dfi < theGraph.vertexData[L].dfi ? leastAncestor : L;
                }
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_CreateSortedSeparatedDFSChildLists() {
        for (node n: sourceGraph.nodes) {
            node lp = theGraph.vertexData[n].Lowpoint;
            BMGraphAttributes::LCAppend(theGraph.bin, theGraph.buckets[theGraph.vertexData[lp].dfi], n);
        }

        for (int i = 0; i < sourceGraph.numberOfNodes(); ++i) {
            node J;
            if ((J = theGraph.buckets[i]) != nullptr) {
                while (J != nullptr) {
                    if (theGraph.vertexData[J].DFSParent != nullptr && theGraph.vertexData[J].DFSParent != J) {
                        node& theList = theGraph.vertexData[theGraph.vertexData[J].DFSParent].separatedDFSChildList;
                        theList = BMGraphAttributes::LCAppend(theGraph.DFSChildLists, theList, J);
                    }
                    J = BMGraphAttributes::LCGetNext(theGraph.bin, theGraph.buckets[i], J);
                }
            }
        }

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