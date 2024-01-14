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
        stack<pair<node, adjEntry>> dfsStack;

        for (node n: sourceGraph.nodes) {
            theGraph.vertexData[n].visited = 0;
        }

        for (node n: sourceGraph.nodes) {
            if (DFI >= sourceGraph.numberOfEdges()) break;
            if (theGraph.vertexData[n].DFSParent != nullptr) continue;
            theGraph.vertexData[n].dfi = DFI;
            theGraph.vertexData[n].leastAncestor = DFI;
            theGraph.dfi_sorted[DFI] = n;
            DFI++;
            theGraph.vertexData[n].visited = 1;
            theGraph.vertexData[n].DFSParent = nullptr;
            for (adjEntry ad: n->adjEntries) {
                if (theGraph.vertexData[ad->twinNode()].visited != 0) {
                    if (theGraph.vertexData[n].dfi > theGraph.vertexData[ad->twinNode()].dfi && theGraph.edgeData[ad->twin()].type != EDGE_DFSCHILD) {
                        if (theGraph.vertexData[n].leastAncestor > theGraph.vertexData[ad->twinNode()].dfi) {
                            theGraph.vertexData[n].leastAncestor = theGraph.vertexData[ad->twinNode()].dfi;
                        }
                    }
                }
                dfsStack.emplace(ad->twinNode(), ad);
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
                    theGraph.vertexData[u].link[0] = uparent;
                    theGraph.vertexData[u].link[1] = uparent;

                    theGraph.edgeData[e].type = EDGE_DFSCHILD;

                    for (adjEntry a: u->adjEntries) {
                        dfsStack.emplace(a->twinNode(), a);
                    }
                } else {
                    if (theGraph.vertexData[uparent].dfi < theGraph.vertexData[u].dfi) {
                        theGraph.edgeData[e].type = EDGE_FORWARD;
                        theGraph.vertexData[u].fwdArcList.pushBack(e);
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

    void BoyerMyrvoldEdgeAddition::gp_LowpointAndLeastAncestor() {
        adjEntry adj, lastAdj;
        node w;
        for (int i = sourceGraph.numberOfNodes() - 1; i>= 0; --i) {
            node v = theGraph.dfi_sorted[i];
            theGraph.vertexData[v].Lowpoint = theGraph.vertexData[v].leastAncestor;

            adj = v->firstAdj();
            while (adj) {
                lastAdj = adj;
                adj = adj->succ();
                if (theGraph.edgeData[lastAdj].type != EDGE_DFSCHILD && theGraph.edgeData[lastAdj].type != EDGE_DFSPARENT) {
                    continue;
                }
                w = lastAdj->twinNode();

                if (theGraph.vertexData[w].dfi <= i) {
                    continue;
                }
                if (theGraph.vertexData[w].Lowpoint < theGraph.vertexData[v].Lowpoint) {
                    theGraph.vertexData[v].Lowpoint = theGraph.vertexData[w].Lowpoint;
                }
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_CreateSortedSeparatedDFSChildLists() {
        for (node n: sourceGraph.nodes) {
            theGraph.buckets[theGraph.vertexData[n].Lowpoint].push_back(n);
        }
        vector<node> sortedNodes;
        for (auto & bucket : theGraph.buckets) {
            for (node n: bucket) {
                sortedNodes.push_back(n);
            }
        }

        for (node n: sortedNodes) {
            node parent;
            if ((parent = theGraph.vertexData[n].DFSParent) != nullptr) {
                theGraph.vertexData[parent].separatedDFSChildList.push_back(n);
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_CreateDFSTreeEmbedding() {
        for (node n: sourceGraph.nodes) {
            if (theGraph.vertexData[n].DFSParent != nullptr) {
                for (adjEntry adj: n->adjEntries) {
                    if (adj->twinNode() == theGraph.vertexData[n].DFSParent) {
                        theGraph.vertexData[n].adjList.pushBack(adj);
                        theGraph.vertexData[n].adjList;
                        theGraph.vertexData[theGraph.vertexData[n].DFSParent].rootAdjList.pushBack(adj->twin());
                        break;
                    }
                }
            }
            theGraph.extFace[n].link[0] = theGraph.vertexData[n].link[1] = n;
        }
    }

    void BoyerMyrvoldEdgeAddition::_EmbedBackEdgeToDescendant(int RootSide, node RootVertex, node W,
                                                              int WPrevLink) {
        adjEntry fwdArc = theGraph.vertexData[W].adjacentTo;
        adjEntry backArc = fwdArc->twin();

        node parentCopy = theGraph.vertexData[RootVertex].DFSParent;

        theGraph.extFace[RootVertex].link[RootSide] = W;
        theGraph.extFace[W].link[WPrevLink] = RootVertex;
    }

    int BoyerMyrvoldEdgeAddition::_VertexActiveStatus(node theVertex, node I) {
        int leastLowpoint;
        node DFSCHILD;

        if ((DFSCHILD = theGraph.vertexData[theVertex].separatedDFSChildList.front()) == nullptr) {
            leastLowpoint = theGraph.vertexData[theVertex].Lowpoint;
        } else {
            leastLowpoint = theGraph.vertexData[DFSCHILD].Lowpoint;
        }

        if (theGraph.vertexData[theGraph.dfi_sorted[leastLowpoint]].dfi > theGraph.vertexData[theGraph.dfi_sorted[theGraph.vertexData[theVertex].leastAncestor]].dfi) {
            leastLowpoint = theGraph.vertexData[theVertex].leastAncestor;
        }

        if (theGraph.vertexData[theGraph.dfi_sorted[leastLowpoint]].dfi < theGraph.vertexData[I].dfi) {
            return VAS_EXTERNAL;
        }

        if (theGraph.vertexData[theVertex].adjacentTo != nullptr || theGraph.vertexData[theVertex].pertinentBicompList.front() !=
                                                                            nullptr) {
            return VAS_INTERNAL;
        }
        return VAS_INACTIVE;
    }

    int BoyerMyrvoldEdgeAddition::_GetNextVertexOnExternalFace(node curVertex, int& pPrevLink) {
        node e = theGraph.vertexData[curVertex].link[pPrevLink];

    }

    void BoyerMyrvoldEdgeAddition::_InvertVertex(int V) {}

    void BoyerMyrvoldEdgeAddition::_SetSignOfChildEdge(node V, int sign) {
        for (adjEntry a: V->adjEntries) {
            if (theGraph.edgeData[a].type == EDGE_DFSCHILD) {
                theGraph.edgeData[a].sign = sign;
                break;
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_MergeVertex(int W, int WPrevLink, int R) {}

    void BoyerMyrvoldEdgeAddition::_MergeBicomps() {}

    void BoyerMyrvoldEdgeAddition::_RecordPertinentChildBicomp(node I, node RootVertex) {
        node parent = theGraph.vertexData[RootVertex].DFSParent;

        if(theGraph.vertexData[RootVertex].Lowpoint < theGraph.vertexData[I].dfi) {
            theGraph.vertexData[parent].pertinentBicompList.push_back(RootVertex);
        } else {
            theGraph.vertexData[parent].pertinentBicompList.push_front(RootVertex);
        }
    }

    node BoyerMyrvoldEdgeAddition::_GetPertinentChildBicomp(node W) {
        node RootId;

        /* If the bicomp list is empty, then we just return NIL */

        if ((RootId = theGraph.vertexData[W].pertinentBicompList.front()) == nullptr)
            return nullptr;

        /* Return the RootVertex, which is computed by adding N because we
            subtracted N before storing it in the bicomp list */

        return RootId + theGraph.N;
    }

    void BoyerMyrvoldEdgeAddition::_WalkUp(node I, node W) {
        node Zig, Zag, R, nextVertex, ParentCopy;
        int ZigPrevLink, ZagPrevLink;

        Zig = Zag = W;
        ZigPrevLink = 1;
        ZagPrevLink = 0;

        while (Zig != I) {
            if (theGraph.vertexData[Zig].visited == 1) break;
            if (theGraph.vertexData[Zag].visited == 1) break;

            theGraph.vertexData[Zig].visited = 1;
            theGraph.vertexData[Zag].visited = 1;

            if (theGraph.vertexData[Zig].isRoot) {
                R = Zig;
            } else if (theGraph.vertexData[Zag].isRoot) {
                R = Zag;
            } else {
                R = nullptr;
            }

            if (R != nullptr) {
                ParentCopy = theGraph.vertexData[R].DFSParent;
                if (ParentCopy != I) {
                    _RecordPertinentChildBicomp(I, R);
                }
                Zig = Zag = ParentCopy;
                ZigPrevLink = 1;
                ZagPrevLink = 0;
            } else {
                nextVertex = theGraph.extFace[Zig].link[1^ZigPrevLink];
                ZigPrevLink = theGraph.extFace[nextVertex].link[0] == Zig ? 0 : 1;
                Zig = nextVertex;

                nextVertex = theGraph.extFace[Zag].link[1^ZagPrevLink];
                ZagPrevLink = theGraph.extFace[nextVertex].link[0] == Zag ? 0 : 1;
                Zag = nextVertex;
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_WalkDown(node I, node RootVertex) {}

    int BoyerMyrvoldEdgeAddition::gp_Embed(int embedFlags) {}

    void BoyerMyrvoldEdgeAddition::_OrientVerticesInEmbedding() {}

    void BoyerMyrvoldEdgeAddition::_OrientVerticesInBicomp(int BicompRoot, int PreserveSigns) {}

    int BoyerMyrvoldEdgeAddition::_JoinBicomps() {}

    void _initGraphAttributes(Graph &G) {
    }
} // ogdf