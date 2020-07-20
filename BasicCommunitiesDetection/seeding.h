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


void runParallelLouvain(char ** argv)
{

    string commandToCallParallelLouvain="./driverForGraphClustering ";
    commandToCallParallelLouvain+= argv[4];
    commandToCallParallelLouvain+=" -f 5";
    commandToCallParallelLouvain+=" -o";
    const char *command=commandToCallParallelLouvain.c_str();
    cout << "Running file using " << command << endl;
    system(command);
}

void calculateClusteringCoefficient(long *NV, vector<cc> *ccVec,long  *vtxPtr , edge  *vtxInd){
        for(int i=0;i<*NV;i++)
        {
            double clusteringCoefficient=0.0;
            clusteringCoefficient= computeClusteringCoefficient(NV,vtxPtr,vtxInd,&i);
            ccVec->push_back(std::make_pair(i,clusteringCoefficient));
        }
}
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
vector<int> selecttop10percentClusteringCoefficient(long *NV,vector<cc> *ccVec)
{
    int value=*NV;
    vector<int>selected;
    selected.clear();

    double selectSize=ceil((value* (double)((double)10/(double)100)));
    for(int i=0;i<selectSize;i++)
    {
        selected.push_back(ccVec->at(i).first);
    }
    return selected;

}

void readLouvainOutput(vector <Perm_Info> *vector_info, char **argv)
{
    string fileName=argv[4];
    fileName+= "_clustInfo";
    cout<<fileName;
    std::ifstream infile(fileName);
    int a;
    std::string str;
    int i=0;
    while (std::getline(infile, str)) {
        vector_info->at(i).Comm=std::stoi(str);
        i++;
    }

}

void printvectorInfoOutput(vector <Perm_Info> *vector_info)
{
    for(int i=0;i<vector_info->size();i++)
    {
        cout <<i<<"---"<< vector_info->at(i).Comm<<"\n";
    }
}

void findDegree1andAssignCommunity(long  *vtxPtr , edge  *vtxInd,vector <Perm_Info> *vector_info,vector<int> *selected)
{
    vector<bool> visit(vector_info->size(),false);
    for(int i=0;i<selected->size();i++) {
        cout<<selected->at(i)<<"--"<<"\n";
        vector<int>tmpneigh= get_neighbors(selected->at(i),vtxPtr,vtxInd);

        for(int j=0;j<tmpneigh.size();j++)
        {cout <<selected->at(i)<<"---" <<tmpneigh.at(j)<<"---"<<vector_info->at(j).Comm<<"\n";
            if(visit[tmpneigh.at(j)]!=true) {
                vector_info->at(j).Comm = vector_info->at(selected->at(i)).Comm;
                visit[tmpneigh.at(j)]=true;
                cout<<"after" <<tmpneigh.at(j)<<"---"<<vector_info->at(j).Comm<<"\n";
            }
        }
    }
}
void  clusteringCoefficient_seed(long *NV,long  *vtxPtr , edge  *vtxInd , vector <Perm_Info> *vector_info, char ** argv, int argc)
{
    /** Call Parallel Louvain and get Community**/

    runParallelLouvain(argv);
    readLouvainOutput(vector_info,argv);

    /* print method is just for testing*/
  //  printvectorInfoOutput(vector_info);

  /** Compute Clustering Coefficient **/
  vector<cc> ccVec;
calculateClusteringCoefficient(NV, &ccVec, vtxPtr,vtxInd);
sortClusteringCoefficient(&ccVec);
// Print to just see sort is perfoming correctly
//printClusteringCoefficient(&ccVec);
vector<int>selected=selecttop10percentClusteringCoefficient(NV,&ccVec);
// next find 1 degree of all selected and assign them same community
findDegree1andAssignCommunity(vtxPtr,vtxInd,vector_info,&selected);
/* print method is just for testing*/
//printvectorInfoOutput(vector_info);

}

#endif //GRAPPOLO_SEEDING_H
