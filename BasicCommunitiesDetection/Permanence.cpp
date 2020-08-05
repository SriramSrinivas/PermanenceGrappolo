//
// Created by sriramsrinivas@unomaha.edu on 5/30/20.
//

#include "defs.h"
#include "utilityClusteringFunctions.h"
# include "PermanenceCalculation.h"
#include "seeding.h"
using namespace std;

void printEdges(long NV, long  *vtxPtr,  edge * vtxInd)
{

    for(int i=0;i<NV;i++) {
        long adj1 = vtxPtr[i];	    //Begin
        long adj2 = vtxPtr[i+1];	//End
        double totalWt = 0;
        cout <<i<<"----";
        for(long j=adj1; j<adj2; j++) {
            cout <<vtxInd[j].tail<<" ";
        }
        cout<<"\n";
    }


}

void initializeVector_info(vector<Perm_Info> * vector_info,long *NV,long  *vtxPtr , edge  *vtxInd)
{
#pragma omp parallel for
    for(int i=0;i<*NV;i++)
    {
        long adj1=vtxPtr[i];
        long adj2=vtxPtr[i+1];
        for(int z=adj1;z<adj2;z++)
        {
            vector_info->at(i).neighbors.push_back(vtxInd[z].tail);
        }
    }
}

void runPermanence(graph *G, clustering_parameters *opts, int *threadsOpt , int numThreads, char **argv, int argc)
{

    cout<<"Inside Permanence Code"<<"\n";
    cout<<"Number of Threads"<<numThreads<<"\n";
    long    NV        = G->numVertices;
    long    NS        = G->sVertices;
    long    NE        = G->numEdges;
    long    *vtxPtr   = G->edgeListPtrs;
    edge    *vtxInd   = G->edgeList;

    printEdges(NV,vtxPtr,vtxInd);
    long* pastCommAss = (long *) malloc (NV * sizeof(long)); assert(pastCommAss != 0);
    long* currCommAss = (long *) malloc (NV * sizeof(long)); assert(currCommAss != 0);

    /* Mahantesh's implementation of initial Community
     *  We will be using Seeding initially past and current community assigniment will be based on degree seeding even for hybrid implementation
     * */
    initCommAss(pastCommAss, currCommAss, NV);
    vector<Perm_Info> vector_info;
    int max_comms=0;
    Perm_Info dummy_perm;
    vector_info.resize(NV, dummy_perm);

    initializeVector_info(&vector_info,&NV, vtxPtr, vtxInd);

    /*Clustering Coefficient Seeding will call Parallel Louvian, get communities, next using Boost, all vertices are ranked based on top Clustering Coefficient.
     * Next the code selects top 10 % of the vertices based on rank, followed by its neighbor and assign them to a community. the seeding is then fed to the initialize perminfo
     * routine
     *
     * */


    if(argc>4) {
        cout <<"Doing Clustering Coefficient Seed \n";
        clusteringCoefficient_seed(G, opts, threadsOpt, &numThreads, &NV, vtxPtr, vtxInd, &vector_info, argv, argc,  &max_comms);

    } else{
        cout <<"Doing Degree  Seed \n";
        degreeMin_seed(&NV,vtxPtr,vtxInd,&vector_info);
    }
    cout <<"Seeding Done \n";

    initialize_perminfo(&NV,vtxPtr,vtxInd,&max_comms,&vector_info);
    cout <<"Initialization Done \n";

    //Compute Permanence
    //Set this to false if you do not want to have singleton vertices
    bool allow_singleton=false;
    cluster_by_permanence_old(&NV,vtxPtr,vtxInd, max_comms, &vector_info,allow_singleton);
    double permanence=0.0;
    for(int z=0;z< vector_info.size();z++)
    {   permanence=permanence+vector_info[z].perm;
        cout << z << "::"<< vector_info[z].Comm << "::" << vector_info[z].perm <<"\n";}
    cout << "Total Permanence : ==" << permanence  << "\n";
    }

