//
// Created by huber on 29.11.23.
//

#include <ogdf/basic/Graph_d.h>
#include "BoyerMyrvoldEdgeAddition.h"

namespace ogdf {

    BoyerMyrvoldEdgeAddition::BoyerMyrvoldEdgeAddition(Graph &g) : theGraph(g), sourceGraph(g) {
    }

    int BoyerMyrvoldEdgeAddition::gp_CreateDFSTree() {
        int DFI = 0;
        node uparent, u;
        adjEntry e;

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
            for (adjEntry adj: n->adjEntries) {
                if (theGraph.vertexData[adj->twinNode()].visited != 0) {
                    if (theGraph.vertexData[n].dfi > theGraph.vertexData[adj->twinNode()].dfi &&
                        theGraph.edgeData[adj->twin()].type != EDGE_DFSCHILD) {
                        if (theGraph.vertexData[n].leastAncestor > theGraph.vertexData[adj->twinNode()].dfi) {
                            theGraph.vertexData[n].leastAncestor = theGraph.vertexData[adj->twinNode()].dfi;
                        }
                    }
                }
                dfsStack.emplace(adj->twinNode(), adj);
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

                    for (adjEntry adj: u->adjEntries) {
                        dfsStack.emplace(adj->twinNode(), adj);
                    }
                } else {
                    if (theGraph.vertexData[uparent].dfi < theGraph.vertexData[u].dfi) {
                        theGraph.edgeData[e].type = EDGE_FORWARD;
                        theGraph.vertexData[uparent].fwdArcList.push_back(e);
                        theGraph.fwdListIters[e] = prev(theGraph.vertexData[uparent].fwdArcList.end());
                    } else if (theGraph.edgeData[e->twin()].type == EDGE_DFSCHILD) {
                        theGraph.edgeData[e].type = EDGE_DFSPARENT;
                    } else {
                        theGraph.edgeData[e].type = EDGE_BACK;
                    }
                }
            }
        }

        return OK;
    }

    void BoyerMyrvoldEdgeAddition::gp_LowpointAndLeastAncestor() {
        vector<node> st;
        node u, uneighbor;
        int L, leastAncestor;
        for (node n: sourceGraph.nodes) {
            theGraph.vertexData[n].visited = 0;
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
            node parent = theGraph.vertexData[n].DFSParent;
            if (parent != nullptr) {
                theGraph.vertexData[parent].separatedDFSChildList.push_back(n);
                theGraph.sepDfsChildIters[n] = prev(theGraph.vertexData[parent].separatedDFSChildList.end());
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_CreateDFSTreeEmbedding() {
        for (node n: sourceGraph.nodes) {
            if (theGraph.vertexData[n].DFSParent != nullptr) {
                for (adjEntry adj: n->adjEntries) {
                    if (theGraph.edgeData[adj].type == EDGE_DFSPARENT) {
                        theGraph.vertexData[n].adjList.pushBack(adj);
                        theGraph.vertexData[theGraph.vertexData[n].DFSParent].adjList.pushBack(adj->twin());
                        theGraph.edgeData[adj].isTargetRoot = true;
                        break;
                    }
                }
                theGraph.extFace[n].link[0] = theGraph.extFace[n].link[1] = pair(n, true);
                theGraph.rootExtFace[n].link[0] = theGraph.rootExtFace[n].link[1] = pair(n, false);
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_EmbedBackEdgeToDescendant(int RootSide, bNode RootVertex, bNode W,
                                                              int WPrevLink) {
        adjEntry fwdArc = theGraph.getVertexData(W).adjacentTo;
        adjEntry backArc = fwdArc->twin();

        bNode parentCopy = make_pair(theGraph.vertexData[RootVertex.first].DFSParent, false);

        theGraph.getExtFace(RootVertex).link[RootSide] = W;
        theGraph.getExtFace(W).link[WPrevLink] = RootVertex;

        theGraph.getVertexData(parentCopy).fwdArcList.erase(theGraph.fwdListIters[fwdArc]);

        if (RootSide == 0) {
            theGraph.getVertexData(RootVertex).adjList.pushFront(fwdArc);
            theGraph.getVertexData(W).adjList.pushFront(backArc);
        } else {
            theGraph.getVertexData(RootVertex).adjList.pushBack(fwdArc);
            theGraph.getVertexData(W).adjList.pushBack(backArc);
        }

        theGraph.getExtFace(RootVertex).link[RootSide] = W;
        theGraph.getExtFace(W).link[WPrevLink] = RootVertex;
    }

    int BoyerMyrvoldEdgeAddition::_VertexActiveStatus(bNode theVertex, bNode I) {
        int leastLowpoint;
        node DFSCHILD;
        if (!theGraph.getVertexData(theVertex).separatedDFSChildList.empty()) {
            DFSCHILD = theGraph.getVertexData(theVertex).separatedDFSChildList.front();
        } else {
            DFSCHILD = nullptr;
        }

        if (DFSCHILD == nullptr) {
            leastLowpoint = theGraph.getVertexData(theVertex).dfi;
        } else {
            leastLowpoint = theGraph.vertexData[DFSCHILD].Lowpoint;
        }

        if (theGraph.vertexData[theGraph.dfi_sorted[leastLowpoint]].dfi >
            theGraph.vertexData[theGraph.dfi_sorted[theGraph.getVertexData(theVertex).leastAncestor]].dfi) {
            leastLowpoint = theGraph.getVertexData(theVertex).leastAncestor;
        }

        if (theGraph.vertexData[theGraph.dfi_sorted[leastLowpoint]].dfi < theGraph.getVertexData(I).dfi) {
            return VAS_EXTERNAL;
        }

        if (theGraph.getVertexData(theVertex).adjacentTo != nullptr ||
            theGraph.getVertexData(theVertex).pertinentBicompList.front() !=
            nullptr) {
            return VAS_INTERNAL;
        }
        return VAS_INACTIVE;
    }

    void BoyerMyrvoldEdgeAddition::_InvertVertex(bNode V) {
        theGraph.getVertexData(V).adjList.reverse();
        bNode temp;
        temp = theGraph.getExtFace(V).link[0];
        theGraph.getExtFace(V).link[0] = theGraph.getExtFace(V).link[1];
        theGraph.getExtFace(V).link[1] = temp;
    }

    void BoyerMyrvoldEdgeAddition::_SetSignOfChildEdge(bNode V, int sign) {
        for (adjEntry a: V.first->adjEntries) {
            if (theGraph.edgeData[a].type == EDGE_DFSCHILD) {
                theGraph.edgeData[a].sign = sign;
                break;
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_MergeVertex(bNode W, int WPrevLink, bNode R) {
        if (WPrevLink == 0) {
            theGraph.getVertexData(W).adjList.conc(theGraph.getVertexData(R).adjList);
        } else {
            theGraph.getVertexData(W).adjList.concFront(theGraph.getVertexData(R).adjList);
        }
    }

    void BoyerMyrvoldEdgeAddition::_MergeBicomps() {
        int Rout, ZPrevLink;
        bNode R, Z;
        bNode extFaceVertex;

        while (!theGraph.theStack.empty()) {
            R = theGraph.theStack.back().first;
            Rout = theGraph.theStack.back().second;
            theGraph.theStack.pop_back();
            Z = theGraph.theStack.back().first;
            ZPrevLink = theGraph.theStack.back().second;
            theGraph.theStack.pop_back();

            extFaceVertex = theGraph.getExtFace(R).link[1 ^ Rout];
            theGraph.getExtFace(Z).link[ZPrevLink] = extFaceVertex;

            if (theGraph.getExtFace(extFaceVertex).link[0] == theGraph.getExtFace(extFaceVertex).link[1]) {
                theGraph.getExtFace(extFaceVertex).link[Rout ^ theGraph.getExtFace(extFaceVertex).inversionFlag] = Z;
            } else {
                theGraph.getExtFace(extFaceVertex).link[theGraph.getExtFace(extFaceVertex).link[0] == R ? 0 : 1] = Z;
            }

            if (ZPrevLink == Rout) {
                if (theGraph.getVertexData(R).adjList.cyclicSucc(theGraph.getVertexData(R).adjList.begin()) ==
                    theGraph.getVertexData(R).adjList.begin()) {
                    _InvertVertex(R);
                }
                _SetSignOfChildEdge(R, -1);
            }
            theGraph.getVertexData(Z).pertinentBicompList.erase(theGraph.bicompListIters[R.first]);
            theGraph.getVertexData(Z).separatedDFSChildList.erase(theGraph.sepDfsChildIters[R.first]);
            _MergeVertex(Z, ZPrevLink, R);
        }
    }

    void BoyerMyrvoldEdgeAddition::_RecordPertinentChildBicomp(bNode I, bNode RootVertex) {
        bNode parent = make_pair(theGraph.vertexData[RootVertex.first].DFSParent, false);
        bNode dfsChild = make_pair(RootVertex.first, false);

        if (theGraph.getVertexData(dfsChild).Lowpoint < theGraph.getVertexData(I).dfi) {
            theGraph.getVertexData(parent).pertinentBicompList.push_back(RootVertex.first);
            theGraph.bicompListIters[RootVertex.first] = prev(theGraph.getVertexData(parent).pertinentBicompList.end());
        } else {
            theGraph.getVertexData(parent).pertinentBicompList.push_front(RootVertex.first);
            theGraph.bicompListIters[RootVertex.first] = (theGraph.getVertexData(parent).pertinentBicompList.begin());
        }
    }

    BoyerMyrvoldEdgeAddition::bNode BoyerMyrvoldEdgeAddition::_GetPertinentChildBicomp(bNode W) {
        node root;

        if ((root = theGraph.getVertexData(W).pertinentBicompList.front()) == nullptr)
            return make_pair(nullptr, true);

        return make_pair(root, true);
    }

    bool BoyerMyrvoldEdgeAddition::pertinent(bNode V) {
        return theGraph.vertexData[V.first].adjacentTo != nullptr ||
               !theGraph.getVertexData(V).pertinentBicompList.empty();
    }

    void BoyerMyrvoldEdgeAddition::_WalkUp(bNode I, bNode W) {
        bNode Zig, Zag, nextVertex;
        bNode ParentCopy, R;
        int ZigPrevLink, ZagPrevLink;

        Zig = Zag = W;
        ZigPrevLink = 1;
        ZagPrevLink = 0;

        while (Zig != I) {
            if (theGraph.getVertexData(Zig).visitor == I) break;
            if (theGraph.getVertexData(Zag).visitor == I) break;

            theGraph.getVertexData(Zig).visitor = I;
            theGraph.getVertexData(Zag).visitor = I;

            if (Zig.second) {
                R = Zig;
            } else if (Zag.second) {
                R = Zag;
            } else {
                R = make_pair(nullptr, false);
            }

            if (R.first != nullptr) {
                ParentCopy = make_pair(theGraph.vertexData[R.first].DFSParent, false);
                if (ParentCopy != I) {
                    _RecordPertinentChildBicomp(I, R);
                }
                Zig = Zag = ParentCopy;
                ZigPrevLink = 1;
                ZagPrevLink = 0;
            } else {
                nextVertex = theGraph.getExtFace(Zig).link[1 ^ ZigPrevLink];
                ZigPrevLink = theGraph.getExtFace(nextVertex).link[0] == Zig ? 0 : 1;
                Zig = nextVertex;

                nextVertex = theGraph.getExtFace(Zag).link[1 ^ ZagPrevLink];
                ZagPrevLink = theGraph.getExtFace(nextVertex).link[0] == Zag ? 0 : 1;
                Zag = nextVertex;
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_WalkDown(bNode I, bNode RootVertex) {
        bNode W, R, X, Y;
        int WPrevLink, Rout, XPrevLink, YPrevLink, RootSide;
        theGraph.theStack.clear();
        for (RootSide = 0; RootSide < 2; ++RootSide) {
            WPrevLink = 1 ^ RootSide;
            W = theGraph.getExtFace(RootVertex).link[RootSide];

            while (W != RootVertex) {
                if (theGraph.getVertexData(W).adjacentTo != nullptr) {
                    _MergeBicomps();
                    _EmbedBackEdgeToDescendant(RootSide, RootVertex, W, WPrevLink);
                    theGraph.getVertexData(W).adjacentTo = nullptr;
                }


                if (!theGraph.getVertexData(W).pertinentBicompList.empty()) {
                    theGraph.theStack.emplace_back(W, WPrevLink);
                    R = _GetPertinentChildBicomp(W);

                    X = theGraph.getExtFace(R).link[0];
                    XPrevLink = theGraph.getExtFace(X).link[1] == R ? 1 : 0;
                    Y = theGraph.getExtFace(R).link[1];
                    YPrevLink = theGraph.getExtFace(Y).link[0] == R ? 0 : 1;

                    if (X == Y && theGraph.getExtFace(X).inversionFlag) {
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
                    X = theGraph.getExtFace(W).link[1 ^ WPrevLink];
                    WPrevLink = theGraph.getExtFace(X).link[0] == W ? 0 : 1;
                    W = X;
                } else {
                    break;
                }
            }

            if (theGraph.theStack.empty()) {
                theGraph.getExtFace(RootVertex).link[RootSide] = W;
                theGraph.getExtFace(W).link[WPrevLink] = RootVertex;

                if (theGraph.getExtFace(W).link[0] == theGraph.getExtFace(W).link[1] && WPrevLink == RootSide) {
                    theGraph.getExtFace(W).inversionFlag = 1;
                } else {
                    theGraph.getExtFace(W).inversionFlag = 0;
                }
            }
            if (!theGraph.theStack.empty() || W == RootVertex) {
                break;
            }
        }
    }

    int BoyerMyrvoldEdgeAddition::gp_Embed() {
        bNode W, cur;

        gp_CreateDFSTree();
        gp_LowpointAndLeastAncestor();
        _CreateSortedSeparatedDFSChildLists();
        _CreateDFSTreeEmbedding();

        for (int i = sourceGraph.numberOfNodes() - 1; i >= 0; --i) {
            cur = make_pair(theGraph.dfi_sorted[i], false);
            for (adjEntry adj: theGraph.getVertexData(cur).fwdArcList) {
                W = theGraph.getTarget(adj);
                theGraph.vertexData[W.first].adjacentTo = adj;
                _WalkUp(cur, W);
            }

            for (node n: theGraph.getVertexData(cur).separatedDFSChildList) {
                if (!theGraph.vertexData[n].pertinentBicompList.empty()) {
                    _WalkDown(cur, make_pair(n, true));
                }
            }

            if (!theGraph.getVertexData(cur).fwdArcList.empty()) {
                return NONPLANAR;
            }
        }
        return OK;
    }
} // ogdf