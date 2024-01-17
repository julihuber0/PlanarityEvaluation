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
                    if (theGraph.vertexData[n].dfi > theGraph.vertexData[ad->twinNode()].dfi &&
                        theGraph.edgeData[ad->twin()].type != EDGE_DFSCHILD) {
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
                    theGraph.vertexData[u].dfi = DFI;
                    theGraph.dfi_sorted[DFI] = u;
                    ++DFI;
                    theGraph.vertexData[u].DFSParent = uparent;

                    theGraph.edgeData[e].type = EDGE_DFSCHILD;
                    theGraph.vertexData[uparent].dfsChildArcs.push_back(e);

                    for (adjEntry a: u->adjEntries) {
                        dfsStack.emplace(a->twinNode(), a);
                    }
                } else {
                    if (theGraph.vertexData[uparent].dfi < theGraph.vertexData[u].dfi) {
                        theGraph.edgeData[e].type = EDGE_FORWARD;
                        theGraph.vertexData[u].fwdArcList.push_back(e);
                        auto it = theGraph.vertexData[u].fwdArcList.end();
                        it--;
                        theGraph.fwdListIters[e] = it;
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
            if (theGraph.edgeData[ed->adjSource()].type != EDGE_DFSCHILD &&
                theGraph.edgeData[ed->adjSource()].type != EDGE_DFSPARENT) {
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
        /*adjEntry adj, lastAdj;
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
        }*/
        vector<node> st;
        node u, uneighbor;
        int L, leastAncestor;
        for (node n: sourceGraph.nodes) {
            theGraph.vertexData[n].visited = false;
        }

        for (node I: sourceGraph.nodes) {
            if (theGraph.vertexData[I].visited) {
                continue;
            }
            st.push_back(I);
            while (!st.empty()) {
                u = st.back();
                st.pop_back();
                if (!theGraph.vertexData[u].visited) {
                    theGraph.vertexData[u].visited = 1;
                    st.push_back(u);

                    for (adjEntry a: theGraph.vertexData[u].dfsChildArcs) {
                        st.push_back(a->twinNode());
                    }
                } else {
                    L = leastAncestor = theGraph.vertexData[u].dfi;
                    for (adjEntry a: u->adjEntries) {
                        uneighbor = a->twinNode();
                        if (theGraph.edgeData[a].type == EDGE_DFSCHILD) {
                            if (L > theGraph.vertexData[uneighbor].Lowpoint) {
                                L = theGraph.vertexData[uneighbor].Lowpoint;
                            }
                        } else if (theGraph.edgeData[a].type == EDGE_BACK) {
                            if (leastAncestor > theGraph.vertexData[uneighbor].dfi) {
                                leastAncestor = theGraph.vertexData[uneighbor].dfi;
                            }
                        }
                    }
                    theGraph.vertexData[u].leastAncestor = leastAncestor;
                    theGraph.vertexData[u].Lowpoint = min(leastAncestor, L);
                }
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_CreateSortedSeparatedDFSChildLists() {
        for (node n: sourceGraph.nodes) {
            theGraph.buckets[theGraph.vertexData[n].Lowpoint].push_back(n);
        }
        vector<node> sortedNodes;
        for (auto &bucket: theGraph.buckets) {
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
        /*for (node n: sourceGraph.nodes) {
            for (node v: theGraph.vertexData[n].separatedDFSChildList) {
                cout << theGraph.vertexData[v].Lowpoint << endl;
            }
            cout << "-----" << endl;
        }*/
    }

    void BoyerMyrvoldEdgeAddition::_CreateDFSTreeEmbedding() {
        for (node n: sourceGraph.nodes) {
            if (theGraph.vertexData[n].DFSParent != nullptr) {
                for (adjEntry adj: n->adjEntries) {
                    if (adj->twinNode() == theGraph.vertexData[n].DFSParent) {
                        theGraph.vertexData[n].adjList.pushBack(adj);
                        theGraph.vertexData[theGraph.vertexData[n].DFSParent].rootAdjList.pushBack(adj->twin());
                        break;
                    }
                }
            }
            theGraph.extFace[n].link[0] = theGraph.extFace[n].link[1] = n;
            theGraph.rootExtFace[n].link[0] = theGraph.rootExtFace[n].link[1] = n;
        }
    }

    void BoyerMyrvoldEdgeAddition::_EmbedBackEdgeToDescendant(int RootSide, node RootVertex, node W,
                                                              int WPrevLink) {
        adjEntry fwdArc = theGraph.vertexData[W].adjacentTo;
        adjEntry backArc = fwdArc->twin();

        node parentCopy = theGraph.vertexData[RootVertex].DFSParent;

        theGraph.rootExtFace[RootVertex].link[RootSide] = W;
        theGraph.extFace[W].link[WPrevLink] = RootVertex;

        theGraph.vertexData[parentCopy].fwdArcList.erase(theGraph.fwdListIters[fwdArc]);

        if (RootSide == 0) {
            theGraph.vertexData[RootVertex].adjList.pushFront(fwdArc);
            theGraph.vertexData[W].adjList.pushFront(backArc);
        } else {
            theGraph.vertexData[RootVertex].adjList.pushBack(fwdArc);
            theGraph.vertexData[W].adjList.pushBack(backArc);
        }

        theGraph.rootExtFace[RootVertex].link[RootSide] = W;
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

        if (theGraph.vertexData[theGraph.dfi_sorted[leastLowpoint]].dfi >
            theGraph.vertexData[theGraph.dfi_sorted[theGraph.vertexData[theVertex].leastAncestor]].dfi) {
            leastLowpoint = theGraph.vertexData[theVertex].leastAncestor;
        }

        if (theGraph.vertexData[theGraph.dfi_sorted[leastLowpoint]].dfi < theGraph.vertexData[I].dfi) {
            return VAS_EXTERNAL;
        }

        if (theGraph.vertexData[theVertex].adjacentTo != nullptr ||
            theGraph.vertexData[theVertex].pertinentBicompList.front() !=
            nullptr) {
            return VAS_INTERNAL;
        }
        return VAS_INACTIVE;
    }

    void BoyerMyrvoldEdgeAddition::_InvertVertex(node V, bool isRoot) {
        theGraph.vertexData[V].adjList.reverse();
        node temp;
        if (!isRoot) {
            temp = theGraph.extFace[V].link[0];
            theGraph.extFace[V].link[0] = theGraph.extFace[V].link[1];
            theGraph.extFace[V].link[1] = temp;
        } else {
            temp = theGraph.rootExtFace[V].link[0];
            theGraph.rootExtFace[V].link[0] = theGraph.rootExtFace[V].link[1];
            theGraph.rootExtFace[V].link[1] = temp;
        }
    }

    void BoyerMyrvoldEdgeAddition::_SetSignOfChildEdge(node V, int sign) {
        for (adjEntry a: V->adjEntries) {
            if (theGraph.edgeData[a].type == EDGE_DFSCHILD) {
                theGraph.edgeData[a].sign = sign;
                break;
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_MergeVertex(node W, int WPrevLink, node R) {
        if (WPrevLink == 0) {
            theGraph.vertexData[W].adjList.conc(theGraph.vertexData[R].rootAdjList);
        } else {
            theGraph.vertexData[W].adjList.concFront(theGraph.vertexData[R].rootAdjList);
        }
    }

    void BoyerMyrvoldEdgeAddition::_MergeBicomps() {
        int Rout, ZPrevLink;
        node R, Z, extFaceVertex;

        while (!theGraph.theStack.empty()) {
            R = theGraph.theStack.back().first;
            Rout = theGraph.theStack.back().second;
            theGraph.theStack.pop_back();
            Z = theGraph.theStack.back().first;
            ZPrevLink = theGraph.theStack.back().second;
            theGraph.theStack.pop_back();

            extFaceVertex = theGraph.rootExtFace[R].link[1 ^ Rout];
            theGraph.extFace[Z].link[ZPrevLink] = extFaceVertex;

            if (theGraph.extFace[extFaceVertex].link[0] == theGraph.extFace[extFaceVertex].link[1]) {
                theGraph.extFace[extFaceVertex].link[Rout ^ theGraph.extFace[extFaceVertex].inversionFlag] = Z;
            } else {
                theGraph.extFace[extFaceVertex].link[theGraph.extFace[extFaceVertex].link[0] == R ? 0 : 1] = Z;
            }

            if (ZPrevLink == Rout) {
                if (theGraph.vertexData[R].rootAdjList.cyclicSucc(theGraph.vertexData[R].rootAdjList.begin()) ==
                    theGraph.vertexData[R].rootAdjList.begin()) {
                    _InvertVertex(R, true);
                }
                _SetSignOfChildEdge(R, -1);
            }
            theGraph.vertexData[Z].pertinentBicompList.erase(theGraph.bicompListIters[R]);
            theGraph.vertexData[Z].separatedDFSChildList.erase(theGraph.sepDfsChildIters[R]);
            _MergeVertex(Z, ZPrevLink, R);
        }
    }

    void BoyerMyrvoldEdgeAddition::_RecordPertinentChildBicomp(node I, node RootVertex) {
        node parent = theGraph.vertexData[RootVertex].DFSParent;

        if (theGraph.vertexData[RootVertex].Lowpoint < theGraph.vertexData[I].dfi) {
            theGraph.vertexData[parent].pertinentBicompList.push_back(RootVertex);
        } else {
            theGraph.vertexData[parent].pertinentBicompList.push_front(RootVertex);
        }
    }

    node BoyerMyrvoldEdgeAddition::_GetPertinentChildBicomp(node W) {
        node root;

        /* If the bicomp list is empty, then we just return NIL */

        if ((root = theGraph.vertexData[W].pertinentBicompList.front()) == nullptr)
            return nullptr;

        /* Return the RootVertex, which is computed by adding N because we
            subtracted N before storing it in the bicomp list */

        return root;
    }

    bool BoyerMyrvoldEdgeAddition::pertinent(node V) {
        return theGraph.vertexData[V].adjacentTo != nullptr || !theGraph.vertexData[V].pertinentBicompList.empty();
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
                nextVertex = theGraph.extFace[Zig].link[1 ^ ZigPrevLink];
                ZigPrevLink = theGraph.extFace[nextVertex].link[0] == Zig ? 0 : 1;
                Zig = nextVertex;

                nextVertex = theGraph.extFace[Zag].link[1 ^ ZagPrevLink];
                ZagPrevLink = theGraph.extFace[nextVertex].link[0] == Zag ? 0 : 1;
                Zag = nextVertex;
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_WalkDown(node I, node RootVertex) {
        node W, R, X, Y;
        int WPrevLink, Rout, XPrevLink, YPrevLink, RootSide;
        theGraph.theStack.clear();
        for (RootSide = 0; RootSide < 2; ++RootSide) {
            WPrevLink = 1 ^ RootSide;
            W = theGraph.rootExtFace[RootVertex].link[RootSide];

            while (W != RootVertex) {
                if (theGraph.vertexData[W].adjacentTo != nullptr) {
                    _MergeBicomps();
                    _EmbedBackEdgeToDescendant(RootSide, RootVertex, W, WPrevLink);
                    theGraph.vertexData[W].adjacentTo = nullptr;
                }

                if (!theGraph.vertexData[W].pertinentBicompList.empty()) {
                    theGraph.theStack.emplace_back(W, WPrevLink);
                    R = _GetPertinentChildBicomp(W);

                    X = theGraph.rootExtFace[R].link[0];
                    XPrevLink = theGraph.extFace[X].link[1] == R ? 1 : 0;
                    Y = theGraph.rootExtFace[R].link[1];
                    YPrevLink = theGraph.extFace[Y].link[0] == R ? 0 : 1;

                    if (X == Y && theGraph.extFace[X].inversionFlag) {
                        XPrevLink = 0;
                        YPrevLink = 1;
                    }

                    if (_VertexActiveStatus(X, I) == VAS_INTERNAL) {
                        W = X;
                    } else if (_VertexActiveStatus(Y, I) == VAS_INTERNAL) {
                        W = Y;
                    } else if (pertinent(X)) {
                        W = X;
                    } else {
                        W = Y;
                    }

                    WPrevLink = W == X ? XPrevLink : YPrevLink;
                    Rout = W == X ? 0 : 1;
                    theGraph.theStack.emplace_back(R, Rout);
                } else if (_VertexActiveStatus(W, I) == VAS_INACTIVE) {
                    X = theGraph.extFace[W].link[1 ^ WPrevLink];
                    WPrevLink = theGraph.rootExtFace[X].link[0] == W ? 0 : 1;
                    W = X;
                } else {
                    break;
                }

                if (theGraph.theStack.empty()) {
                    theGraph.rootExtFace[RootVertex].link[RootSide] = W;
                    theGraph.extFace[W].link[WPrevLink] = RootVertex;

                    if (theGraph.extFace[W].link[0] == theGraph.extFace[W].link[1] && WPrevLink == RootSide) {
                        theGraph.extFace[W].inversionFlag = 1;
                    } else {
                        theGraph.extFace[W].inversionFlag = 0;
                    }
                }
                if (!theGraph.theStack.empty() || W == RootVertex) {
                    break;
                }
            }
        }
    }

    int BoyerMyrvoldEdgeAddition::gp_Embed() {
        node W, cur;

        gp_CreateDFSTree();
        gp_LowpointAndLeastAncestor();
        _CreateSortedSeparatedDFSChildLists();
        _CreateDFSTreeEmbedding();
        theGraph._fillVisitedFlags(true, sourceGraph);

        for (int i = sourceGraph.numberOfNodes() - 1; i >= 0; --i) {
            cur = theGraph.dfi_sorted[i];
            for (adjEntry adj: theGraph.vertexData[cur].fwdArcList) {
                W = adj->twinNode();
                theGraph.vertexData[W].adjacentTo = adj;
                _WalkUp(cur, W);
            }

            for (node n: theGraph.vertexData[cur].separatedDFSChildList) {
                if (!theGraph.vertexData[n].pertinentBicompList.empty()) {
                    _WalkDown(cur, n);
                }
            }

            if (!theGraph.vertexData[cur].fwdArcList.empty()) {
                return NONPLANAR;
            }
        }
        return OK;
    }

    void _initGraphAttributes(Graph &G) {
    }
} // ogdf