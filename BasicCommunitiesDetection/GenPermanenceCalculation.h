//
// Created by sriramsrinivas@unomaha.edu on 10/21/20.
//

#ifndef GRAPPOLO_GENPERMANENCECALCULATION_H
#define GRAPPOLO_GENPERMANENCECALCULATION_H
#include "defs.h"
#include <algorithm>
typedef pair<long,long> int_double;

using namespace std;
bool sortbysec(const pair<long,long> &a, const pair<long,long> &b)
{
return (a.second <b.second);
}



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
            cout <<"m"<<m<<"n"<<n<<"\n";
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


void compute_permanence_overlap(long NV, long  *vtxPtr,  edge * vtxInd, PI_Network *PI_prev, PI_Network *PI_cur, long  node, long  *perm)
{
    //Map of communities
    //First is the community id
    //Second is the index
    std::map<long,long> communityInfo;
    std::pair<std::map<long,long>::iterator,bool> retE;
    std::map<long,long>::iterator itE;
    vector<vector<long>> C_Info; //stores the connections to each external community
    C_Info.clear();
    vector<long> C_ID; //stores ids of communities;
    vector<bool> C_self; //marking communities that are shared with nodes
    P_Info PI_next;
    long adj1 = vtxPtr[node];	    //Begin
    long adj2 = vtxPtr[node+1];	//End

    //For each neighbor
    for(long i=adj1;i<adj2;i++)
    {
        int neigh=vtxInd[i].tail;

        //find the communities to which it belongs
        for(int j=0;j<PI_cur->at(neigh).ListPI.size();j++)
        {
            int neigh_comm=PI_cur->at(neigh).ListPI[j].first;
            int pos=-1;
            vector<long> dummyV;
            dummyV.clear();

            //update connections to external community
            itE=communityInfo.find(neigh_comm);
            if(itE!=communityInfo.end()) //community already exists find location
            {pos=communityInfo.find(neigh_comm)->second;}
            else //add to map and new entry in shared comms
            { retE=communityInfo.insert(std::pair<long,long>(neigh_comm,communityInfo.size()));
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

    long degree=adj2-adj1;


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
    if(max_index!=-1){
        Emaxd_inv=1.0/(double)(degree*C_Info[max_index].size());}


    double Emaxd2_inv;
    if(max_index2==-1){Emaxd2_inv=1.0/(double)(degree);}
    else{Emaxd2_inv=1.0/(double)(degree*C_Info[max_index2].size());}

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
    for(long i=0;i<C_Info.size();i++)
    {
        long numerator=0;

        if(C_Info[i].size()>1)
        {
            compute_CC_Numerator( NV, vtxPtr, vtxInd,&C_Info[i],&numerator);
cout<<"i"<<i<<"num"<<numerator<<"\n";
            cc_values[i].second=(double)numerator/(double)((C_Info[i].size())*(C_Info[i].size()-1));
            cc_values[i].first=i;
        }
//        else
//        {cc_values[i].second=0.0;}

        numeratorS[i]=numerator;
    }
    std::sort(cc_values.begin(),cc_values.end(),sortbysec); //sort lowest to highest
//        cout <<"checkCC"<<"\n";
//        for(int k=0;k<cc_values.size();k++)
//        {
//            cout<<node <<"::" <<cc_values.at(k).first<<"--- "<<cc_values.at(k).second<<"\n";
//        }


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
        if(cc_values[i].first==max_index)
        {first_fact=Emaxd2_inv;}


        //Take union of current set of seen neighbors and neighbors in community
        long m=C_Info[i].size();
        long n=seen_neighbors.size();
        long t=m+n;
        vector<long> v;
        v.clear();
        v.resize(t,0);
        std::vector<long>::iterator it;
        it=std::set_union (seen_neighbors.begin(), seen_neighbors.end(), C_Info[comm].begin(), C_Info[comm].end(), v.begin());
        v.resize(it-v.begin());


        //number of external neighbors
        double second_fact=(double)(1.0-cc_values[i].second)/(double)(v.size());

        //Update seen neighbors
        seen_neighbors=v;


        //Compute Perm
        Perm =first_fact-second_fact;
        cout<<"node::"<<node <<"First:::"<<first_fact<<" second::::"<<second_fact<<"\n";



        if(Perm <= 0.0) {break;}
        found=true;

        //get total permanence
        Sum_Perm=Sum_Perm+Perm;
        //cout <<"i"<<node<<"::"<<Perm;

        int2_dbl Perm_info;

        Perm_info.first=C_ID[i];
        Perm_info.second=Perm;
        Perm_info.third=numeratorS[i];

        PI_next.ListPI.push_back(Perm_info);
    } //end of for


    *perm=Sum_Perm;



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

void optimize_permanence(long NV, long  *vtxPtr,  edge * vtxInd, PI_Network *PI, long *perm)
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

                long myperm=0.0;
                compute_permanence_overlap(NV,vtxPtr,vtxInd, &PI_prev, PI, i, &myperm);
                sumQ=sumQ+myperm;

            } //end of checking all nodes

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
