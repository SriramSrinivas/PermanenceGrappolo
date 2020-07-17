//
// Created by sriramsrinivas@unomaha.edu on 7/17/20.
//

#ifndef GRAPPOLO_INTERNALCC_H
#define GRAPPOLO_INTERNALCC_H
#include <algorithm>
#include <iostream>
#include <list>

void get_neighbors(int node,long  *vtxPtr ,  edge  *vtxInd, vector<int> *neighbors)
{
    //clear neighbors;
    neighbors->clear();
    long adj1 = vtxPtr[node];
    long adj2 = vtxPtr[node+1];

    //Add the neighbors;
    for(int i=adj1;i<adj2;i++)
    {neighbors->push_back(vtxInd[i].tail);}

    return;}


void compute_CC(long *NV,long  *vtxPtr , edge  *vtxInd , vector<int> node_set, double *cc)
{
    if(node_set.size()<2) {return;}
    *cc=0.0;
    double numerator=0.0;
    double denominator=0.0;
    vector<int> common_neighs;


    std::sort(node_set.begin(), node_set.end());
    for(int i=0;i<node_set.size();i++)
    { common_neighs.clear();
        int nx=node_set[i];
        vector<int> myneighbors;
        myneighbors.clear();


        get_neighbors(nx,vtxPtr,vtxInd, &myneighbors);

        //No need to sort myneighbors as neighbors aranged in increasing order
//        common_neighs=intersect(myneighbors, node_set);


        std::set_intersection(myneighbors.begin(), myneighbors.end(),
                              node_set.begin(), node_set.end(),
                              std::back_inserter(common_neighs));


        numerator=numerator+(double)common_neighs.size();

    }//end of for

    double total_nodes=(double)node_set.size();
    denominator=total_nodes*(total_nodes-1);
    *cc=numerator/denominator;

    return;
}

#endif //GRAPPOLO_INTERNALCC_H
