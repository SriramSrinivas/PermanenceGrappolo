//
// Created by sriramsrinivas@unomaha.edu on 10/20/20.
//
#include "defs.h"
#include "utilityClusteringFunctions.h"
#include "OverlappingSeeding.h"
#include "GenPermanenceCalculation.h"



using namespace std;

void runOverlappingPermanence(graph *G, clustering_parameters *opts, int *threadsOpt , int numThreads, char **argv, int argc)
{
    cout << "Inside Overlapping Permanence Code" << "\n";
    long    NV        = G->numVertices;
    long    NS        = G->sVertices;
    long    NE        = G->numEdges;
    long    *vtxPtr   = G->edgeListPtrs;
    edge    *vtxInd   = G->edgeList;

    P_Info dummyPI;
    PI_Network PI;
    PI.resize(NV, dummyPI);

    NeighborMin_seed(NV,vtxPtr,vtxInd, &PI);

    optimize_permanence(NV,vtxPtr,vtxInd, &PI);

}