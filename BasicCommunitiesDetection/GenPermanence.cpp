//
// Created by sriramsrinivas@unomaha.edu on 10/20/20.
//
#include "defs.h"
#include "utilityClusteringFunctions.h"
#include "OverlappingSeeding.h"
#include "GenPermanenceCalculation.h"



using namespace std;

PI_Network runOverlappingPermanence(graph *G, clustering_parameters *opts, int *threadsOpt , int numThreads, char **argv, int argc, PI_Network PI)
{
    cout << "Inside Overlapping Permanence Code" << "\n";
    long    NV        = G->numVertices;
    long    NS        = G->sVertices;
    long    NE        = G->numEdges;
    long    *vtxPtr   = G->edgeListPtrs;
    edge    *vtxInd   = G->edgeList;

    NeighborMin_seed(NV,vtxPtr,vtxInd, &PI);

//    for(int i=0;i<NV;i++)
//    {printf("%d::%d \n",i, PI[i].ListPI[0].first);}


    long perm=0.0;


    optimize_permanence(NV,vtxPtr,vtxInd, &PI, &perm);

    return PI;

}