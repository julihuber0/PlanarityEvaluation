//
// Created by huber on 29.11.23.
//

#include "BoyerMyrvoldEdgeAddition.h"

namespace ogdf {

    BoyerMyrvoldEdgeAddition::BoyerMyrvoldEdgeAddition(Graph &g) {
    }

    void BoyerMyrvoldEdgeAddition::_CreateSortedSeparatedDFSChildLists() {
        NodeArray<int> buckets;
        listCollection bin;
        int I, J, N, DFSParent, theList;

        N = theEmbedding->N;
        buckets = theEmbedding->buckets;
        bin = theEmbedding->bin;

        /* Initialize the bin and all the buckets to be empty */

        //LCReset(bin);
        for (I = 0; I < N; I++)
            buckets[I] = NIL;

        /* For each vertex, add it to the bucket whose index is equal to
           the Lowpoint of the vertex. */

        for (I = 0; I < N; I++) {
            J = theEmbedding->V[I].Lowpoint;
            buckets[J] = LCAppend(bin, buckets[J], I);
        }

        /* For each bucket, add each vertex in the bucket to the
           separatedDFSChildList of its DFSParent.  Since lower numbered buckets
           are processed before higher numbered buckets, vertices with lower
           Lowpoint values are added before those with higher Lowpoint values,
           so the separatedDFSChildList of each vertex is sorted by Lowpoint */

        for (I = 0; I < N; I++) {
            if ((J = buckets[I]) != NIL) {
                while (J != NIL) {
                    DFSParent = theEmbedding->V[J].DFSParent;

                    if (DFSParent != NIL && DFSParent != J) {
                        theList = theEmbedding->V[DFSParent].separatedDFSChildList;
                        theList = LCAppend(theEmbedding->DFSChildLists, theList, J);
                        theEmbedding->V[DFSParent].separatedDFSChildList = theList;
                    }

                    J = LCGetNext(bin, buckets[I], J);
                }
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_CreateFwdArcLists() {
        int I, Jfirst, Jnext, Jlast;


        for (I = 0; I < theGraph->N; I++) {
            Jfirst = theGraph->G[I].link[1];

            /* If the vertex has any forward edges, then ... */

            if (theGraph->G[Jfirst].type == EDGE_FORWARD) {
                /* Find the end of the forward edge list */

                Jnext = Jfirst;
                while (theGraph->G[Jnext].type == EDGE_FORWARD)
                    Jnext = theGraph->G[Jnext].link[1];
                Jlast = theGraph->G[Jnext].link[0];

                /* Remove the forward edges from the adjacency list of I */

                theGraph->G[Jnext].link[0] = I;
                theGraph->G[I].link[1] = Jnext;

                /* Make a circular forward edge list */

                theGraph->V[I].fwdArcList = Jfirst;
                theGraph->G[Jfirst].link[0] = Jlast;
                theGraph->G[Jlast].link[1] = Jfirst;
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_CreateDFSTreeEmbedding() {
        int N, I, J, Jtwin, R;

        N = theGraph->N;
        for (I = 0, R = N; I < N; I++, R++) {
            if (theGraph->V[I].DFSParent == NIL) {
                theGraph->G[I].link[0] = theGraph->G[I].link[1] = I;
            } else {
                J = theGraph->G[I].link[0];
                while (theGraph->G[J].type != EDGE_DFSPARENT)
                    J = theGraph->G[J].link[0];

                theGraph->G[I].link[0] = theGraph->G[I].link[1] = J;
                theGraph->G[J].link[0] = theGraph->G[J].link[1] = I;
                theGraph->G[J].v = R;

                //Jtwin = gp_GetTwinArc(theGraph, J);

                theGraph->G[R].link[0] = theGraph->G[R].link[1] = Jtwin;
                theGraph->G[Jtwin].link[0] = theGraph->G[Jtwin].link[1] = R;

                theGraph->extFace[R].link[0] = theGraph->extFace[R].link[1] = I;
                theGraph->extFace[I].link[0] = theGraph->extFace[I].link[1] = R;
            }
        }
    }

    void BoyerMyrvoldEdgeAddition::_EmbedBackEdgeToDescendant(int RootSide, int RootVertex, int W,
                                                              int WPrevLink) {
        int fwdArc, backArc, parentCopy;

        /* We get the two edge records of the back edge to embed.
            The Walkup recorded in W's adjacentTo the index of the forward arc
            from the root's parent copy to the descendant W. */

        fwdArc = theGraph->V[W].adjacentTo;
        //backArc = gp_GetTwinArc(theGraph, fwdArc);

        /* The forward arc is removed from the fwdArcList of the root's parent copy. */

        parentCopy = theGraph->V[RootVertex - theGraph->N].DFSParent;

        if (theGraph->V[parentCopy].fwdArcList == fwdArc)
        {
            if (theGraph->G[fwdArc].link[0] == fwdArc)
                theGraph->V[parentCopy].fwdArcList = NIL;
            else theGraph->V[parentCopy].fwdArcList = theGraph->G[fwdArc].link[0];
        }

        theGraph->G[theGraph->G[fwdArc].link[0]].link[1] = theGraph->G[fwdArc].link[1];
        theGraph->G[theGraph->G[fwdArc].link[1]].link[0] = theGraph->G[fwdArc].link[0];

        /* The forward arc is added to the adjacency list of the RootVertex. */

        theGraph->G[fwdArc].link[1^RootSide] = RootVertex;
        theGraph->G[fwdArc].link[RootSide] = theGraph->G[RootVertex].link[RootSide];
        theGraph->G[theGraph->G[RootVertex].link[RootSide]].link[1^RootSide] = fwdArc;
        theGraph->G[RootVertex].link[RootSide] = fwdArc;

        /* The back arc is added to the adjacency list of W. */

        theGraph->G[backArc].v = RootVertex;

        theGraph->G[backArc].link[1^WPrevLink] = W;
        theGraph->G[backArc].link[WPrevLink] = theGraph->G[W].link[WPrevLink];
        theGraph->G[theGraph->G[W].link[WPrevLink]].link[1^WPrevLink] = backArc;
        theGraph->G[W].link[WPrevLink] = backArc;

        /* Link the two endpoint vertices together on the external face */

        theGraph->extFace[RootVertex].link[RootSide] = W;
        theGraph->extFace[W].link[WPrevLink] = RootVertex;
    }

    int BoyerMyrvoldEdgeAddition::_GetNextVertexOnExternalFace(int curVertex, int *pPrevLink) {
        int arc, nextArc, nextVertex, newPrevLink;

        /* Exit curVertex from whichever link was not previously used to enter it */

        arc = theEmbedding->G[curVertex].link[1 ^ (*pPrevLink)];

        nextArc = gp_GetTwinArc(arc);

        nextVertex = theEmbedding->G[nextArc].link[newPrevLink = 0];
        if (nextVertex >= 2 * theEmbedding->N)
            nextVertex = theEmbedding->G[nextArc].link[newPrevLink = 1];

        /* The setting above is how we exited an edge record to get to the
           next vertex.  The reverse pointer leads back from the vertex to
           the edge record. */

        newPrevLink = 1 ^ newPrevLink;

        /* This if stmt assigns the new prev link that tells us which edge
           record was used to enter nextVertex (so that we exit from the
           opposing edge record).
           However, if we are in a singleton bicomp, then both links in nextVertex
           lead back to curVertex, so newPrevLink may get stop at the zero setting
           when it should become one.
           We want the two arcs of a singleton bicomp to act like a cycle, so the
           edge record given as the prev link for curVertex should be the same as
           the prev link for nextVertex.
           So, we only need to modify the prev link if the links in nextVertex
           are not equal. */

        if (theEmbedding->G[nextVertex].link[0] != theEmbedding->G[nextVertex].link[1])
            *pPrevLink = newPrevLink;

        return nextVertex;
    }

    void BoyerMyrvoldEdgeAddition::_InvertVertex(int V) {}

    void BoyerMyrvoldEdgeAddition::_SetSignOfChildEdge(int V, int sign) {}

    void BoyerMyrvoldEdgeAddition::_MergeVertex(int W, int WPrevLink, int R) {}

    void BoyerMyrvoldEdgeAddition::_MergeBicomps() {}

    void BoyerMyrvoldEdgeAddition::_RecordPertinentChildBicomp(int I, int RootVertex) {}

    int BoyerMyrvoldEdgeAddition::_GetPertinentChildBicomp(int W) {
        int RootId;

        /* If the bicomp list is empty, then we just return NIL */

        if ((RootId = theGraph->V[W].pertinentBicompList) == NIL)
            return NIL;

        /* Return the RootVertex, which is computed by adding N because we
            subtracted N before storing it in the bicomp list */

        return RootId + theGraph->N;
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