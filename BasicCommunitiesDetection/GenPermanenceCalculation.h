//
// Created by sriramsrinivas@unomaha.edu on 10/21/20.
//

#ifndef GRAPPOLO_GENPERMANENCECALCULATION_H
#define GRAPPOLO_GENPERMANENCECALCULATION_H
#include "defs.h"
#include <algorithm>
typedef pair<int,double > int_double;

using namespace std;
void compute_CC_Numerator(long NV, long  *vtxPtr,  edge * vtxInd, vector<int> *neighbors, int *numerator)
{
    *numerator=0;
    int last_neigh=neighbors->at(neighbors->size()-1);

    for(int i=0; i<neighbors->size()-1; i++)
    {
        int n=neighbors->at(i);

        long adj1 = vtxPtr[n];	    //Begin
        long adj2 = vtxPtr[n+1];	//End
        //get hop 2 neighbors
        //only count for neighbors higher than n--since lower number ids would already have been counted
        for(long j=adj1;j<adj2;j++)
        {
            int m=vtxInd[j].tail;
            if(m>last_neigh) {break;}

            //Check remaining neighbirs to see if they are equal to m
            for(int k=i+1;k<neighbors->size(); k++)
            { int p=neighbors->at(k);
                if(m==p){*numerator=*numerator+2; break;}
            }//end of for
        }//end of for
    }//end of for
    return;
}//end of function

//Computing Overlapping Permanence of Node i
//prev=communities in iteration t-1;
//cur=communities in current: t
//next=communities in next t+1


void compute_permanence_overlap(long NV, long  *vtxPtr,  edge * vtxInd, PI_Network *PI_prev, PI_Network *PI, int  node, double * perm )
{
    //Map of communities
    //First is the community id
    //Second is the index
    std::map<int,int> communityInfo;
    std::pair<std::map<int,int>::iterator,bool> retE;
    std::map<int,int>::iterator itE;

    vector< vector<int> > C_Info; //stores the connections to each external community
    C_Info.clear();

    vector<int> C_ID; //stores ids of comunities;

    P_Info PI_next;
    long adj1 = vtxPtr[node];	    //Begin
    long adj2 = vtxPtr[node+1];	//End


    //For each neighbor
    for(long i=adj1; i<adj2; i++)
    {
        int neigh=vtxInd[node].tail;

        //find the communities to which it belongs
        for(int j=0;j<PI->at(neigh).ListPI.size();j++)
        {
            int neigh_comm=PI->at(neigh).ListPI[j].first;
            int pos=-1;
            vector<int> dummyV;
            dummyV.clear();

            //update connections to external community
            itE=communityInfo.find(neigh_comm);
            if(itE!=communityInfo.end()) //community already exists find location
            {pos=communityInfo.find(neigh_comm)->second;}
            else //add to map and new entry in shared comms
            { retE=communityInfo.insert(std::pair<int,int>(neigh_comm,communityInfo.size()));
                pos=communityInfo.size()-1;
                C_Info.push_back(dummyV);
                C_ID.push_back(neigh_comm);
            }
            C_Info[pos].push_back(neigh); //store neighbor into community as given by index
            C_ID[pos]=neigh_comm;

        }//end of going through communities of neighbors
    }//end of going through neighbors



    /*** Computing Permanence for Each Community wrt node ***/
    //Compute degree
    int degree=adj2-adj1;

    //Find community with largest and community with second largest number of neighbors
    int max_index=-1;
    for(int i=0;i<C_Info.size();i++)
    {
        if(C_Info[i].size()>C_Info[max_index].size())
        {max_index=i;}
    }

    int max_index2=-1;
    // if(max_index==0) {max_index=1;}
    for(int i=0;i<C_Info.size();i++)
    {
        if((C_Info[i].size()>C_Info[max_index2].size()) && (i!=max_index))
        {max_index2=i;}
    }


    //P^c(v)= I^c(v)/(Emax(v)*D(v)-(1-Cin^c(v)*I^c(v)/I(v)
    //P^c(v)=I^c(v)[1/(Emax(v)*D(v)-(1-Cin^c(v))/I(v)]
    //When computing overlapping permanence we keep all communities that produce +ve P^c(v)
    //I^c(v) does not affect whether the value is +ve
    //So we do not need to compute it to find the communities

    if(max_index==-1){max_index=0;}

    double Emaxd_inv;
    if(max_index==-1)
    {Emaxd_inv=1.0/(double)(degree);}
    else
    {Emaxd_inv=1.0/(double)(degree*C_Info[max_index].size());}

    double Emaxd2_inv;
    if(max_index2==-1)
    {Emaxd2_inv=1.0/(double)(degree);}
    else
    {Emaxd2_inv=1.0/(double)(degree*C_Info[max_index2].size());}

    // printf("%f  %f \n", Emaxd_inv, Emaxd2_inv);
    vector<int_double> cc_values;
    int_double dummy1;
    dummy1.first=0;
    dummy1.second=0.0;
    cc_values.resize(C_Info.size(),dummy1);

    vector<int> numeratorS;
    numeratorS.resize(C_Info.size(),0);


    //For each community computing Perm
    for(int i=0;i<C_Info.size();i++)
    {
        int numerator=0;
        if(C_Info[i].size()>1)
        {
            compute_CC_Numerator(NV,vtxPtr,vtxInd,&C_Info[i],&numerator);
            cc_values[i].second=(double)numerator/(double)((C_Info[i].size())*(C_Info[i].size()-1));
            cc_values[i].first=i;
        }

        // print_vector(C_Info[i]);
        //  printf("CC::%d --%d--%f\n", numerator, C_Info[i].size(),cc_values[i].second);

        numeratorS[i]=numerator;
    }
     sort(cc_values.begin(), cc_values.end()); //sort lowest to highest


    //To find the communities that give positive permanence start from highest and add while positive
    //This will work because cc_values are the main driver--but the method is still approximate
    double Perm=0;
    double Sum_Perm=0.0;
    vector<int> seen_neighbors;
    seen_neighbors.clear();
    /*
    vector<int> communities;
    communities.clear();

    vector<double> permS;
    permS.clear();*/

    for(int i=0;i<cc_values.size();i++)
    {
        double first_fact=Emaxd_inv;
        if(cc_values[i].first==max_index)
        {first_fact=Emaxd2_inv;}

        //Take union of current set of seen neighbors and neighbors in community
        int m=C_Info[i].size();
        int n=seen_neighbors.size();
        int t=m+n;

        vector<int> v;
        v.resize(t,0);
        std::vector<int>::iterator it;
        it=set_union(seen_neighbors.begin(), seen_neighbors.end(), C_Info[i].begin(), C_Info[i].end(), v.begin());
        v.resize(it-v.begin());

        //number of external neighbors
        double second_fact=(double)(1-cc_values[i].second)/(double)(v.size());

        //Update seen neighbors
        seen_neighbors=v;

        // print_vector(seen_neighbors);

        //Compute Perm
        Perm =first_fact-second_fact;
        //  printf("%f --%d--%f--%f--%d\n", Perm, C_ID[i], first_fact, second_fact,v.size());
        //  printf("%f --%d\n", Perm, C_ID[i]);

        if(Perm <=0) {break;}

        //get total permanence
        Sum_Perm=Sum_Perm+Perm;

        int2_dbl Perm_info;

        Perm_info.first=C_ID[i];
        Perm_info.second=Perm;
        Perm_info.third=numeratorS[i];

        PI_next.ListPI.push_back(Perm_info);
    } //end of for

    *perm=Sum_Perm;


    //  for(int i=0;i<PI_next.ListPI.size();i++)
    // {printf("%d ...\n",PI_next.ListPI[i].first);}
    //  PI_cur->at(node)=PI_next;


    return;
}

void optimize_permanence(long NV, long  *vtxPtr,  edge * vtxInd, PI_Network *PI)
{
    P_Info dummyPI;
    PI_Network PI_prev;
    PI_prev.resize(NV, dummyPI);

    //Iterate until precision is reached
    double sumQ=0.0; //total permannece of network
    double oldQ=-1;
    int max_iter=10;
    int iter=0;

    while( iter < max_iter || (oldQ!=sumQ))
    {
        cout << "ITER ======  "<< iter <<"\n";

        //Update permanence
        oldQ=sumQ;
        sumQ=0.0;

        vector<double> perm;
        perm.resize(NV,0.0);

        //Adjust position of all nodes by moving/merging them
#pragma omp parallel for schedule(dynamic)

        for(int i=0;i<NV; i++)
        {
            //cout <<"At Vertex " << i <<"\n";

            //store previous value
            PI_prev[i]=PI->at(i);
            double myperm=0.0;
            compute_permanence_overlap(NV,vtxPtr,vtxInd, &PI_prev, PI,  i, &myperm);

            //perm[i]=myperm;
            sumQ=sumQ+myperm;

        }//end of checking all nodes



        cout << "At Iteration " << iter << " the total permanence is " << sumQ << "\n";
        iter++;
    }//end of while


    //Print out communities for each node
    for(int i=0;i<NV;i++)
    {

        printf("Node %d: ", i);
        for(int j=0;j<PI->at(i).ListPI.size();j++)
        {printf("%d ",PI->at(i).ListPI[j].first);}

        printf("\n");
    }




    return;
}









#endif //GRAPPOLO_GENPERMANENCECALCULATION_H
