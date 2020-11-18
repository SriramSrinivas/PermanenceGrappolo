//
// Created by sriramsrinivas@unomaha.edu on 10/21/20.
//

#ifndef GRAPPOLO_OVERLAPPINGSEEDING_H
#define GRAPPOLO_OVERLAPPINGSEEDING_H
using namespace std;
typedef pair<int,int> int_int;
void NeighborMin_seed(long NV, long  *vtxPtr,  edge * vtxInd, PI_Network *PI)
{
    //Set Comm to default
//    int_int dummy;
//    dummy.first=NV+1;
//    dummy.second=-1;
    int2_dbl dummy1;

#pragma omp parallel for schedule(static)
    for(int i=0; i<NV;i++)
    {
        PI->at(i).ListPI.push_back(dummy1);
        PI->at(i).ListPI[0].first=1;
    }

    //Initialize Communities
    int id=0;
#pragma omp parallel for schedule(static)
    for(long i=0;i<NV;i++)
    {
        if(PI->at(i).ListPI[0].first>i){PI->at(i).ListPI[0].first=i;}
        long adj1 = vtxPtr[i];	    //Begin
        long adj2 = vtxPtr[i+1];	//End
        for(long j=adj1; j<adj2; j++)
        {
            long n=vtxInd[j].tail;
            if(PI->at(n).ListPI[0].first>i){PI->at(n).ListPI[0].first=i;}
        }//end of for
    }//end of for
    return;


}



#endif //GRAPPOLO_OVERLAPPINGSEEDING_H
