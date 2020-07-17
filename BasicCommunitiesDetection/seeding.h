//
// Created by sriramsrinivas@unomaha.edu on 7/17/20.
//

#ifndef GRAPPOLO_SEEDING_H
#define GRAPPOLO_SEEDING_H


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




    //Sort in decreasing order of degree
    std::sort(degree_order.begin(), degree_order.end(),
              [](const int_double& x, const int_double& y) {
                  // compare second value
                  if (x.second != y.second)
                      return x.second < y.second;

                  // compare first only if second value is equal
                  return x.first < y.first;
              }) ;




    int id=0;
    for(int i=0;i<degree_order.size();i++)
    {
        int myi=degree_order[i].first;

        vector_info->at(myi).Comm=id;
        long adj1 = vtxPtr[myi];
        long adj2 = vtxPtr[myi+1];


        for(int j=adj1; j<adj2; j++)
        {
            int n=vtxInd[j].tail;
            vector_info->at(n).Comm=id;
        }//end of for

        id++;

    }//end of for

    return;
}

#endif //GRAPPOLO_SEEDING_H
