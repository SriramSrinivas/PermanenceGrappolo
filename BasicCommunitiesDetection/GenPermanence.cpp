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

//    for(int i=0;i<NV;i++)
//    {printf("%d::%d \n",i, PI[i].ListPI[0].first);}


    long max_comm=-1;
#pragma omp parallel for schedule(static)
    for(long i=NV-1;i>=0;i--)
    {
        if(max_comm<PI[i].ListPI[0].first)

        { cout << "Thread"<<omp_get_num_threads()<<"\n";
#pragma omp atomic write

            max_comm=PI[i].ListPI[0].first;


        }
    }


    optimize_permanence(NV,vtxPtr,vtxInd, &PI, &max_comm);

}