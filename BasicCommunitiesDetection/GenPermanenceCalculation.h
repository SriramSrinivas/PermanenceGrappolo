//
// Created by sriramsrinivas@unomaha.edu on 10/21/20.
//

#ifndef GRAPPOLO_GENPERMANENCECALCULATION_H
#define GRAPPOLO_GENPERMANENCECALCULATION_H
#include "defs.h"
#include <algorithm>
typedef pair<long,double > int_double;

using namespace std;
void compute_CC_Numerator(long NV, long  *vtxPtr,  edge * vtxInd, vector<long> *neighbors, long *numerator)
{
    *numerator=0;
    int last_neigh=neighbors->at(neighbors->size()-1);

    for(long i=0; i<neighbors->size()-1; i++){
        long n=neighbors->at(i);

        long adj1 = vtxPtr[n];	    //Begin
        long adj2 = vtxPtr[n+1];	//End
        //get hop 2 neighbors
        //only count for neighbors higher than n--since lower number ids would already have been counted
        for(long j=adj1;j<adj2;j++){
            long m=vtxInd[j].tail;
            if(m>last_neigh) {break;}

            //Check remaining neighbirs to see if they are equal to m
            for(long k=i+1;k<neighbors->size(); k++){
                long p=neighbors->at(k);
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


void compute_permanence_overlap(long NV, long  *vtxPtr,  edge * vtxInd, PI_Network *PI_prev, PI_Network *PI, long  node, long  *max_comm)
{
    //Map of communities
    //First is the community id
    //Second is the index
    std::map<long,long> communityInfo;
    std::pair<std::map<long,long>::iterator,bool> retE;
    std::map<long,long>::iterator itE;
    vector<vector<long>> C_Info; //stores the connections to each external community
    C_Info.clear();
    vector<long> C_ID; //stores ids of comunities;
    vector<bool> C_self; //marking communities that are shared with nodes
    P_Info PI_next;
    long adj1 = vtxPtr[node];	    //Begin
    long adj2 = vtxPtr[node+1];	//End
    //Get the communities of node
    vector<long> node_Comm;
    node_Comm.clear();

    for(long i=0;i<PI_prev->at(node).ListPI.size();i++){
        node_Comm.push_back(PI_prev->at(node).ListPI[i].first);
    }
    std::sort(node_Comm.begin(),node_Comm.end());
    //If a neighbor shares communities with node--then only those communities will be considered and not communities that they do not have in common
    vector<long> neigh_Comm;
    vector<long> relevant_Comm;
    long pos=-1;
    vector<long> dummyV;
    dummyV.clear();

    bool mycomm =true;

    //For each neighbor
    for(long i=adj1;i<adj2;i++){
        long neigh=vtxInd[i].tail;
        neigh_Comm.clear();
        //find the communities to which it belongs
        for(long j=0;j<PI_prev->at(neigh).ListPI.size();j++){
            neigh_Comm.push_back(PI_prev->at(neigh).ListPI[j].first);
        }
        std::sort(neigh_Comm.begin(),neigh_Comm.end());

        //Get relevant Comms--either the common communities with node or the set of all communities
        relevant_Comm.clear();
        //        relevant_Comm=intersect(node_Comm, neigh_Comm);
        std::set_intersection(node_Comm.begin(),node_Comm.end(),
                              neigh_Comm.begin(),neigh_Comm.end(),
                              back_inserter(relevant_Comm)

        );

        //If there are no common communities--use all the neigh comm
        if(relevant_Comm.size()==0){ relevant_Comm=neigh_Comm;
            mycomm=false;
        }

        //find the communities to which it belongs
        for(long j=0;j<relevant_Comm.size();j++){
            long neigh_comm=relevant_Comm[j];

            //update connections to external community
            itE=communityInfo.find(neigh_comm);
            if(itE!=communityInfo.end()) //community already exists find location
            {pos=communityInfo.find(neigh_comm)->second;}
            else //add to map and new entry in shared comms
            { retE=communityInfo.insert(std::pair<long,long>(neigh_comm,communityInfo.size()));
                pos=communityInfo.size()-1;
                C_Info.push_back(dummyV);
                C_ID.push_back(neigh_comm);
                C_self.push_back(mycomm);
            }
            C_Info[pos].push_back(neigh); //store neighbor longo community as given by index
            C_ID[pos]=neigh_comm;
            C_self[pos]=mycomm;


        }//end of going through communities of neighbors
    }//end of going through neighbors

    /*** Computing Permanence for Each Community wrt node ***/
    //Compute degree
    long degree=adj2-adj1;

    //cout<<"::::OOO===OO::: \n";

    //Find community with largest and community with second largest number of neighbors
    long max_index=0;
    long max_size=-1;
    long max_size2=-1;

    for(long i=0;i<C_Info.size();i++){
        if((C_Info[i].size()>max_size) && (!C_self[i])){
            max_size=C_Info[i].size();
            max_index=i;
        }
    }

    for(long i=0;i<C_Info.size();i++)
    {
        if((C_Info[i].size()>max_size2) && (!C_self[i]) && (max_size!=max_size2))
        {max_size2=C_Info[i].size();}
    }



    //P^c(v)= I^c(v)/(Emax(v)*D(v)-(1-Cin^c(v)*I^c(v)/I(v)
    //P^c(v)=I^c(v)[1/(Emax(v)*D(v)-(1-Cin^c(v))/I(v)]
    //When computing overlapping permanence we keep all communities that produce +ve P^c(v)
    //I^c(v) does not affect whether the value is +ve
    //So we do not need to compute it to find the communities



    double Emaxd_inv;
    if(max_size==-1){
        Emaxd_inv=1.0/(double)(degree);}
    else{
        Emaxd_inv=1.0/(double)(degree*max_size);}

    double Emaxd2_inv;
    if(max_size2==-1){Emaxd2_inv=1.0/(double)(degree);}
    else{Emaxd2_inv=1.0/(double)(degree*max_size2);}

    //prlongf("%f  %f \n", Emaxd_inv, Emaxd2_inv);
    vector<int_double> cc_values;
    int_double dummy1;
    dummy1.first=0;
    dummy1.second=0.0;
    cc_values.resize(C_Info.size(),dummy1);

    vector<long> numeratorS;
    numeratorS.resize(C_Info.size(),0);

    // cout<<"::::LKLKL::: \n";

    //For each community computing Perm
    for(long i=0;i<C_Info.size();i++){
        long numerator=0;
        cc_values[i].first=i;
        if(C_Info[i].size()>1)
        {
            compute_CC_Numerator( NV, vtxPtr, vtxInd,&C_Info[i],&numerator);
            cc_values[i].second=(double)numerator/(double)((C_Info[i].size())*(C_Info[i].size()-1));
        }
        else
        {cc_values[i].second=0.0;}

        numeratorS[i]=numerator;
    }
    sort(cc_values.begin(),cc_values.end()); //sort lowest to highest
    //    cout <<"checkCC"<<"\n";
    //    for(int k=0;k<cc_values.size();k++)
    //    {
    //        cout<<node <<"::" <<cc_values.at(k).first<<"--- "<<cc_values.at(k).second<<"\n";
    //    }


    //cout<<"::::DDD::: \n";
    // print_vector(cc_values);

    //To find the communities that give positive permanence start from highest and add while positive
    //This will work because cc_values are the main driver--but the method is still approximate
    double Perm=0;
    double Sum_Perm=0.0;
    vector<long> seen_neighbors;
    seen_neighbors.clear();
    /*
     vector<long> communities;
     communities.clear();

     vector<double> permS;
     permS.clear();*/

    //Initially set threshold to >=0 for first step. Then set threshold to >0


    //Such that each node has at least one community--community with highest permanence is selected, even if negative value
    bool found=false;
    for(long i=0;i<cc_values.size();i++){
        long comm=cc_values[i].first;

        double first_fact=Emaxd_inv;
        if(comm==max_index)
        {first_fact=Emaxd2_inv;}

        //Take union of current set of seen neighbors and neighbors in community
        long m=C_Info[comm].size();
        long n=seen_neighbors.size();
        long t=m+n;

        vector<long> v;
        v.clear();
        v.resize(t,0);
        std::vector<long>::iterator it;
        it=std::set_union (seen_neighbors.begin(), seen_neighbors.end(), C_Info[comm].begin(), C_Info[comm].end(), v.begin());
        v.resize(it-v.begin());


        //number of external neighbors
        long second_fact=0.0;
        if(v.size()>0){
            second_fact=(1.0-cc_values[i].second)/(double)(v.size());


        }
        //Update seen neighbors
        seen_neighbors=v;


        //Compute Perm
        Perm =first_fact-second_fact;
        //        cout<<node<<"::"<<"first_fact"<<first_fact<<" ::"<<second_fact<<"\n";


        if(Perm <= 0.0) {break;}
        found=true;

        //get total permanence
        Sum_Perm=Sum_Perm+Perm;
        //cout <<"i"<<node<<"::"<<Perm;

        int2_dbl Perm_info;

        Perm_info.first=C_ID[comm];
        Perm_info.second=Perm;
        Perm_info.third=numeratorS[comm];

        PI_next.ListPI.push_back(Perm_info);
    } //end of for

    //If Perm<0 and not singleton put in new community
    if(!found){

#pragma omp atomic write
        *max_comm=*max_comm+1;

        int2_dbl Perm_info;
        Perm_info.first=*max_comm;
        Perm_info.second=0;
        Perm_info.third=0;
        PI_next.ListPI.push_back(Perm_info);
    }


    PI->at(node)=PI_next;



    //Check if C_ID contains all the communities of the node
    //If not add remaining communities, with itself as neighbor
    //This is to check with own community
    //    bool singleton=false;
    //    if(singleton)
    //    {
    //        vector<int> remaining_comms;
    //        remaining_comms.clear();
    //
    //        vector<int> C_IDx=C_ID;
    //        sort(C_IDx.begin(),C_IDx.end());
    //        remaining_comms=difference(node_Comm,C_IDx);
    //        // print_vector(remaining_comms);
    //        pos=C_ID.size();
    //        for(int i=0;i<remaining_comms.size();i++)
    //        {
    //            C_ID.push_back(remaining_comms[i]);
    //            C_Info.push_back(dummyV);
    //            C_Info[pos].push_back(node);
    //            pos++;
    //        }
    //    }

    return;
}

void optimize_permanence(long NV, long  *vtxPtr,  edge * vtxInd, PI_Network *PI, long * max_comm)
{
    P_Info dummyPI;
    PI_Network PI_prev;
    PI_prev.resize(NV, dummyPI);
    //Iterate until precision is reached
    double sumQ=0.0; //total permannece of network
    double oldQ=-1;
    long max_iter=100;
    long iter=0;

    while( (iter < max_iter) && (oldQ!=sumQ) ) {
        cout << "ITER ======  "<< iter <<"\n";
        //Update permanence
        oldQ=sumQ;
        sumQ=0.0;
#pragma omp parallel
        {
#pragma omp for schedule(dynamic)
            for(long i=0;i<NV; i++) {
                PI_prev[i]=PI->at(i);
            }
            //Adjust position of all nodes by moving/merging them
#pragma omp for schedule(dynamic)
            for(long i=0;i<NV; i++) {
                compute_permanence_overlap(NV,vtxPtr,vtxInd, &PI_prev, PI, i, max_comm);
            } //end of checking all nodes
            vector<double> myP;
            myP.resize(NV,0.0);
#pragma omp for schedule (dynamic)
            for(long i=0;i<NV; i++) {
                for(long j=0;j<PI->at(i).ListPI.size();j++) {
                    myP[i]=myP[i]+PI->at(i).ListPI[j].second;
                }
                //cout<<"::"<<i<<" :::"<<myP[i]<<"\n";
            } //End of for(i)
            sumQ=0.0;
#pragma omp for reduction (+:sumQ)
            for(long i=0; i<NV; i++) {
                sumQ+=myP[i];
            }
        }
        cout << "At Iteration " << iter << " the total permanence is " << sumQ << "\n";
        iter++;
    }//end of while(iter)
    //Prlong out communities for each node
    for(long i=0;i<NV;i++) {
        printf("Node %d: ", i);
        for(long j=0;j<PI->at(i).ListPI.size();j++)
        {printf("%d ",PI->at(i).ListPI[j].first);}
        printf("\n");
    }
    return;
} //End of optimize_permanence()

#endif //GRAPPOLO_GENPERMANENCECALCULATION_H
