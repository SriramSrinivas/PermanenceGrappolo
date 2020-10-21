//
// Created by sriramsrinivas@unomaha.edu on 7/17/20.
//

#ifndef GRAPPOLO_SEEDING_H
#define GRAPPOLO_SEEDING_H

#include <bits/stdc++.h>
#include<string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <math.h>
#include "internalCC.h"
#include "defs.h"
#include "basic_comm.h"
#include "basic_util.h"
typedef pair<int,double> cc;
void degreeMin_seed(long *NV,long  *vtxPtr ,  edge  *vtxInd , vector <Perm_Info> *vector_info)
{
    vector<int_double> degree_order;
    degree_order.clear();
    int_double node_value;


    for(int i=0; i<vector_info->size();i++)
    {
        node_value.first=i;
        node_value.second=0;
        long adj1 = vtxPtr[i];
        long adj2 = vtxPtr[i+1];

        for(int j=adj1; j<adj2; j++)
        {
            node_value.second=node_value.second+1; // addding Degree
        }//end of for

        degree_order.push_back(node_value);


    }//end of for


    vector<bool> visit(degree_order.size(),false);
    //Sort in decreasing order of degree
    std::sort(degree_order.begin(), degree_order.end(),
              [](const int_double& x, const int_double& y) {
                  // compare second value
                  if (x.second != y.second)
                      return x.second > y.second;

                  // compare first only if second value is equal
                  return x.first < y.first;
              }) ;


    int id=0;
    for(int i=0;i<degree_order.size();i++)
    {
        int myi=degree_order[i].first;
        if(visit[myi]!=true) {
            vector_info->at(myi).Comm = id;
            visit[myi]=true;
        }

        long adj1 = vtxPtr[myi];
        long adj2 = vtxPtr[myi+1];


        for(int j=adj1; j<adj2; j++)
        {
            int n=vtxInd[j].tail;
            if(visit[n]!=true) {
                vector_info->at(n).Comm = id;
                visit[n]=true;
            }
        }//end of for

        id++;

    }//end of for

    return;
}
/* Commeneted code only for Debugging */

//    for(int i=0;i<degree_order.size();i++)
//    {cout <<"Afer";
//        cout<<degree_order[i].first<<"-----"<<degree_order[i].second<<"\n";
//    }
//#endif

//    for(int i=0;i<degree_order.size();i++)
//    {
//        cout<<degree_order[i].first<<"-----"<<degree_order[i].second<<"\n";
//    }

int getCommFromLouvainOutput(vector <Perm_Info> *vector_info, long *C_orig)
{

    int i=0;
    int maxcID=0;
    for(int i=0;i<vector_info->size();i++){
        vector_info->at(i).Comm=C_orig[i];
        if(maxcID< (vector_info->at(i).Comm)) {
            maxcID = vector_info->at(i).Comm;
        }
    }

    return maxcID;

}

int runParallelLouvain(graph *G,long *C_orig, clustering_parameters *opts, int *nT, int *threadsOpt ,vector <Perm_Info> *vector_info,char ** argv)
{

//    string commandToCallParallelLouvain="./driverForGraphClustering ";
//    commandToCallParallelLouvain+= argv[4];
//    commandToCallParallelLouvain+=" -f 5";
//    commandToCallParallelLouvain+=" -o";
//    const char *command=commandToCallParallelLouvain.c_str();
//    cout << "Running file using " << command << endl;
//    system(command);
//
//    cout<<G->numVertices<<"verte"<<"\n";
#pragma omp parallel for
    for (long i=0; i<G->numVertices; i++) {
        C_orig[i] = -1;

    }
  runMultiPhaseBasic(G, C_orig, opts->basicOpt, opts->minGraphSize, opts->threshold, opts->C_thresh, *nT,*threadsOpt);

  int maxcID=getCommFromLouvainOutput(vector_info, C_orig);
    return maxcID;
}
// rename function name to avoid confusion
void calculateClusteringCoeffGraph(long *NV,vector <Perm_Info> *vector_info, vector<cc> *ccVec,long  *vtxPtr , edge  *vtxInd){

//#pragma omp parallel for
        for(int i=0;i<*NV;i++)
        {
            double clusteringCoefficient=0.0;
            clusteringCoefficient= computeClusteringCoeffVertex(NV,vtxPtr,vtxInd,&i, vector_info); //Not ready for parallel still working on removing intersection
            ccVec->push_back(std::make_pair(i,clusteringCoefficient));

        }
}
// Sort in decending order
void sortClusteringCoefficient(vector<cc> *ccVec)
{
    std::sort(ccVec->begin(), ccVec->end(),
              [](const int_double& x, const int_double& y) {
                  // compare second value
                  if (x.second != y.second)
                      return x.second > y.second;

                  // compare first only if second value is equal
                  return x.first < y.first;
              }) ;
}

void printClusteringCoefficient(vector<cc> *ccVec)
{
    for (int i=0;i<ccVec->size();i++)
    {
        cout<<ccVec->at(i).first<<"---"<<ccVec->at(i).second<<"\n";
    }

}

// Select to 10percent vertices which has high CC
vector<int> selecttop10percentClusteringCoefficient(long *NV,vector<cc> *ccVec)
{
    int value=*NV;
    vector<int>selected;
    selected.clear();

    double selectSize=ceil((value* (double)((double)90/(double)100)));
    for(int i=0;i<selectSize;i++)
    {
        selected.push_back(ccVec->at(i).first);
    }
    return selected;

}


void printvectorInfoOutput(vector <Perm_Info> *vector_info)
{
//#pragma omp parallel for
    for(int i=0;i<vector_info->size();i++)
    {
        cout <<i<<"---"<< vector_info->at(i).Comm<<"\n";
    }
}
// change to create new cluster
// This routine is called after vertex has idenfied in seeding to have high CC, next all the neighbors are identified and assign new communities, can be done in parallel still need to discuss
void findOneHopandAssignCommunity(long  *vtxPtr , edge  *vtxInd,vector <Perm_Info> *vector_info,vector<int> *selected, int *maxcID)
{
    vector<bool> visit(vector_info->size(),false);
    int tmpcID=*maxcID+1;
    for(int i=0;i<selected->size();i++) {

        if(visit[selected->at(i)]!=true) {
            vector_info->at(selected->at(i)).Comm = tmpcID;
            cout << selected->at(i) << "--" << vector_info->at(selected->at(i)).Comm << "\n";
            vector<int> tmpneigh = get_neighbors(selected->at(i), vtxPtr, vtxInd,vector_info);

            for (int j = 0; j < tmpneigh.size(); j++) {
                cout << selected->at(i) << "---" << tmpneigh.at(j) << "---" << vector_info->at(tmpneigh.at(j)).Comm << "\n";
                if (visit[tmpneigh.at(j)] != true) {
                    vector_info->at(tmpneigh.at(j)).Comm = vector_info->at(selected->at(i)).Comm;
                    visit[tmpneigh.at(j)] = true;
                    cout << "after" << tmpneigh.at(j) << "---" << vector_info->at(tmpneigh.at(j)).Comm << "\n";
                }
            }
            tmpcID += 1;
        }

    }
}

long renumberClusters(long *C,vector <Perm_Info> *vector_info, int * max_comms)
{
#pragma omp parallel for
    for(int i=0;i<vector_info->size();i++)
    {
        C[i]=vector_info->at(i).Comm;
    }

    renumberClustersContiguously(C, (long)vector_info->size());

    #pragma omp parallel for
    for(int i=0;i<vector_info->size();i++)
    {
        vector_info->at(i).Comm=C[i];
        if(*max_comms<C[i])
        {
            *max_comms=C[i];
        }
    }

}
void  clusteringCoefficient_seed( graph *G,clustering_parameters *opts, int *threadsOpt , int *nT, long *NV,long  *vtxPtr , edge  *vtxInd , vector <Perm_Info> *vector_info, char ** argv, int argc, int  *max_comms)
{
    /** Call Parallel Louvain and get Community**/
    long *C_orig = (long *) malloc (*NV * sizeof(long)); assert(C_orig != 0);
    //run Parallel louvain and get maxCID so that the vertices which have high CC can be assigned to new community and new commmunity ID should be increment of maxCID obtained from Louvain
    int maxcID=runParallelLouvain(G, C_orig, opts, nT ,threadsOpt, vector_info, argv);
    // print is optional just kept here for debugging and testing
    printvectorInfoOutput(vector_info);
  vector<cc> ccVec;
  ccVec.resize(*NV);
  // Calculates CC still uses instersection work is ongoing to remove intersection
  calculateClusteringCoeffGraph(NV, vector_info, &ccVec, vtxPtr,vtxInd); // work still in IP for avoid using intersection
// sorts in decending order to select vertices which have High CC
sortClusteringCoefficient(&ccVec);

// Print to just see sort is perfoming correctly
//printClusteringCoefficient(&ccVec);
vector<int>selected=selecttop10percentClusteringCoefficient(NV,&ccVec);
// next find 1 degree of all selected and assign them same community
findOneHopandAssignCommunity(vtxPtr,vtxInd,vector_info,&selected, &maxcID);

// Map to C all comms and update community ID
long *C = (long *) malloc (*NV * sizeof(long));
//// update C to be used for renumberClustersContiguously
renumberClusters(C, vector_info, max_comms);

/* print method is just for testing*/
printvectorInfoOutput(vector_info);


}




#endif //GRAPPOLO_SEEDING_H
