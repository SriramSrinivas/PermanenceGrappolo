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

#include <boost/graph/adjacency_list.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

typedef
boost::adjacency_list<
        boost::vecS            // edge list
        , boost::vecS            // vertex list
        , boost::directedS     // directedness
        , float                  // property associated with vertices
>
        Graph;

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


void runParallelLouvain(char **argv)
{
    string commandToCallParallelLouvain="./driverForGraphClustering ";
    commandToCallParallelLouvain+= argv[4];
    commandToCallParallelLouvain+=" -f 5";
    commandToCallParallelLouvain+=" -o";
    const char *command=commandToCallParallelLouvain.c_str();
    cout << "Running file using " << command << endl;
    system(command);
}


void  clusteringCoefficient_seed(long *NV,long  *vtxPtr ,  edge  *vtxInd , vector <Perm_Info> *vector_info, char ** argv)
{
    /** Call Parallel Louvain and get Community**/

  runParallelLouvain(argv);
  /** Call Boost Clustering Coefficient **/



}

#endif //GRAPPOLO_SEEDING_H
