//
// Created by sriramsrinivas@unomaha.edu on 6/9/20.
//

#ifndef GRAPPOLO_PERMANENCECALCULATION_H
#define GRAPPOLO_PERMANENCECALCULATION_H

#include "internalCC.h"
#include <algorithm>
#include <iostream>
#include <list>
typedef pair<int,double>int_double;
struct Perm_Info
{
    double degree; //vertex degree
    vector<list<int>> comm_neighs;//neighbors in each community
    vector<int>  comms;//list of neighboring communities including own
    vector<double> in_degree; //weighed degree for each community
    vector<double> cc; //clustering coefficient for each community
    std::map<int,int> comm_map; //Map between community id and their position in comm_neighs

    int Comm;  //community id
    double perm ; //permanence

    //Constructor
    Perm_Info()
    {
        degree=0.0;
        comm_neighs.clear();
        in_degree.clear();
        comms.clear();
        comm_map.clear();
        cc.clear();

        Comm=-1;
        perm=-1.0;
    }
};

void initialize_perminfo(long *NV,long  *vtxPtr ,  edge  *vtxInd , int *max_comm, vector<Perm_Info> *vector_info)
{
    list<int> dummy;
    dummy.clear();
    std::pair<std::map<int,int>::iterator,bool> ret;

    *max_comm=0;
    for(int i=0;i<vector_info->size();i++)
    {

        //Add entry for own community
        //Update community map and set of neighboring communities if needed
        int myComm=vector_info->at(i).Comm;
        int map_size=(int)vector_info->at(i).comm_map.size();
        ret=vector_info->at(i).comm_map.insert(std::pair<int,int>(myComm,map_size));
        if(ret.second==true)
        {vector_info->at(i).comm_neighs.push_back(dummy);
            vector_info->at(i).comms.push_back(myComm);
            vector_info->at(i).in_degree.push_back(0.0);
            vector_info->at(i).cc.push_back(1.0);
        }//end of if

        long adj1 = vtxPtr[i];
        long adj2 = vtxPtr[i+1];

        //cout <<"Initializing Vertex " << i <<"\n";
        for(int z=adj1;z<adj2;z++)
        {
            //Get Degree
            vector_info->at(i).degree=vector_info->at(i).degree+1;

            //Get Community of neighbors
            int myN=vtxInd[z].tail;
            int myN_comm=vector_info->at(myN).Comm;

            //Update community map and set of neighboring communities if needed
            int map_size=(int)vector_info->at(i).comm_map.size();
            ret=vector_info->at(i).comm_map.insert(std::pair<int,int>(myN_comm,map_size));
            if(ret.second==true)
            {vector_info->at(i).comm_neighs.push_back(dummy);
                vector_info->at(i).comms.push_back(myN_comm);
                vector_info->at(i).in_degree.push_back(0.0);
                vector_info->at(i).cc.push_back(1.0);}

            //Add neighbor to community
            //Map First:Comm Second:Index
            int index=vector_info->at(i).comm_map.find(myN_comm)->second;
            vector_info->at(i).comm_neighs[index].push_back(myN);
            vector_info->at(i).in_degree[index]=vector_info->at(i).in_degree[index]+1;

        }//end of for z

        //Get clustering coefficient for each community
        vector<int> mynewvector;
        double cc;
        for(int z=0;z<vector_info->at(i).in_degree.size();z++)
        {
            //No neighbors in this community
            if(vector_info->at(i).comm_neighs[z].size()==0.0)
            {cc=1.0;}
            //Only one neighbor
            if(vector_info->at(i).comm_neighs[z].size()==1.0)
            {cc=0.0;}
            //More than one neighbor
            if(vector_info->at(i).comm_neighs[z].size()>1)
            {
                vector<int> mynewvector;
               mynewvector.resize(vector_info->at(i).comm_neighs[z].size());

                list<int> listObject=vector_info->at(i).comm_neighs[z];
                copy(listObject.begin(), listObject.end(), mynewvector.begin());

                //mynewvector=list_to_vector(vector_info->at(i).comm_neighs[z]);
                compute_CC(NV,vtxPtr,vtxInd,mynewvector, &cc);
            }
            vector_info->at(i).cc[z]=cc;
        }//end of for z

        //Update max_comm
        if(vector_info->at(i).Comm>*max_comm){*max_comm=*max_comm+1;}


    }//end of for i
    return;
}


double get_permanence_old(int i, Perm_Info myvector, int mycomm)
{
    double perm=0.0;

    int index=myvector.comm_map.find(mycomm)->second;

    double internal_degree=myvector.in_degree[index];
    double degree=myvector.degree;
    double cc=myvector.cc[index];

    double mymax_E=0.0;
    for(int x=0;x<myvector.in_degree.size();x++)
    {  if(x==index) {continue;}
        if(mymax_E<myvector.in_degree[x])
        {mymax_E=myvector.in_degree[x];}
    }//end of for

    if(mymax_E==0.0){mymax_E=1.0;}

    perm=internal_degree/(mymax_E * degree)-(1.0-cc);

    //  cout << i <<"::" << internal_degree << "::"<< mymax_E << "::"<< degree <<"::"<< cc<<"::"<<mycomm <<"=="<<perm<<"\n";
    return perm;
}




void cluster_by_permanence_old(long *NV,long  *vtxPtr ,  edge  *vtxInd , int max_comm,vector<Perm_Info> *vector_info, bool allow_singleton)
{

    vector<bool> used_comm; //keeps track of the community ids in use

    //Iterate until precision is reached
    double sumQ=0.0; //total permannece of network
    double oldQ=-1;
    int max_iter=10;
    int iter=0;

    list<int> dummy;
    dummy.clear();
    std::list<int>::iterator it;
    std::pair<std::map<int,int>::iterator,bool> ret;
    int  updates=1;

    while( iter < max_iter && ((updates>0) ||(oldQ!=sumQ)))
    {
        cout << "ITER ======  "<< iter <<"\n";
        cout <<"Here";
        //Update permanence
        oldQ=sumQ;
        sumQ=0.0;
        updates=0;
        cout <<"Here";
        //Adjust position of all nodes by moving/merging them
        for(int i=0;i<*NV; i++)
        {


            //Store previous community
            int oldcomm=vector_info->at(i).Comm;
            int newcomm=oldcomm;


            double myperm=get_permanence_old(i,vector_info->at(i),oldcomm);
//            cout <<"At Vertex " << i <<"\n";

            //Do not move node if it is in perm=1.0

            if(myperm==1.0)
            {   vector_info->at(i).perm=myperm;
                sumQ=sumQ+1.0;
                // cout << i << "=="<< vector_info->at(i).Comm <<"=="<<vector_info->at(i).perm <<"\n";
                continue;}

            //Find communities with most in_degrees;
            vector<int> viable_comms;
            viable_comms.clear();
            double max_degree =*max_element(vector_info->at(i).in_degree.begin(), vector_info->at(i).in_degree.end());

            for (int z=0;z<vector_info->at(i).in_degree.size();z++)
            {
                if((vector_info->at(i).in_degree[z]==max_degree) && (vector_info->at(i).comms[z]!=oldcomm))
                {viable_comms.push_back(vector_info->at(i).comms[z]);}
            }


            //Find Permanence for only the viable communities
            for(int z=0;z<viable_comms.size();z++)
            {
                int thiscomm=viable_comms[z];
                double thisperm=get_permanence_old(i,vector_info->at(i),thiscomm);

                //Update Community and Permanence
                if(thisperm>myperm)
                {   myperm=thisperm;
                    newcomm=thiscomm;}

                if(myperm==1.0){break;}

            }//end of for



            //If perm<0; check if moving to new community helps
            //Do not move in first iteration to help adjust the seeding
            if(allow_singleton)
            {
                if((myperm<0.0) && (iter>0))
                {   myperm=0.0;
                    newcomm=max_comm;
                    max_comm=max_comm+1;

                    //Add new community to list
                    //Update community map and set of neighboring communities if needed
                    int map_size=(int)vector_info->at(i).comm_map.size();
                    ret=vector_info->at(i).comm_map.insert(std::pair<int,int>(newcomm,map_size));
                    if(ret.second==true)
                    {vector_info->at(i).comm_neighs.push_back(dummy);
                        vector_info->at(i).comms.push_back(newcomm);
                        vector_info->at(i).in_degree.push_back(0.0);
                        vector_info->at(i).cc.push_back(1.0);}

                }//end of if
            }



            //Update community and permanence
            vector_info->at(i).Comm=newcomm;
            vector_info->at(i).perm=myperm;
            sumQ=sumQ+myperm;

            //Update the values in vector_info if node moves
            if(oldcomm!=newcomm)
            {
                updates++;

                //Update for neighbors
                //Add node i to new internal neighbors
                //Remove node i to old internal neighbors
                //Always do old_comm BEFORE new_comm
                vector<int> mynewvector;
                double cc;
                long adj1 = vtxPtr[i];
                long adj2 = vtxPtr[i+1];

                for(int z=adj1;z<adj2;z++)
                {
                    int myN=vtxInd[z].tail;

                    int index_old=vector_info->at(myN).comm_map.find(oldcomm)->second;
                    vector_info->at(myN).comm_neighs[index_old].remove(i);
                    vector_info->at(myN).in_degree[index_old]=vector_info->at(myN).in_degree[index_old]-1;

                    //Update Clustering Coefficient
                    //No neighbors in this community

                    if(vector_info->at(myN).comm_neighs[index_old].size()==0.0)
                    {vector_info->at(myN).cc[index_old]=1.0;}
                    //Only one neighbor

                    if(vector_info->at(myN).comm_neighs[index_old].size()==1)
                    {vector_info->at(myN).cc[index_old]=0.0;}
                    //More than one neighbor

                    if(vector_info->at(myN).comm_neighs[index_old].size()>1)
                    {
                        vector<int> mynewvector;
                        mynewvector.resize(vector_info->at(i).comm_neighs[index_old].size());
//                        cout <<"At VertexiimQQ 1" << i<<"\n";
                        list<int> listObject=vector_info->at(i).comm_neighs[index_old];
                        copy(listObject.begin(), listObject.end(), mynewvector.begin());

                        // mynewvector=list_to_vector(vector_info->at(myN).comm_neighs[index_old]);

                        compute_CC(NV,vtxPtr,vtxInd,mynewvector, &cc);
                        vector_info->at(myN).cc[index_old]=cc;
                    }


                    //Check if new_comm was already a neighboring community
                    int map_size=(int)vector_info->at(myN).comm_map.size();
                    ret=vector_info->at(myN).comm_map.insert(std::pair<int,int>(newcomm,map_size));
                    if(ret.second==true)
                    {vector_info->at(myN).comm_neighs.push_back(dummy);
                        vector_info->at(myN).comms.push_back(newcomm);
                        vector_info->at(myN).in_degree.push_back(0.0);
                        vector_info->at(myN).cc.push_back(1.0);
                    }

                    int index_new=vector_info->at(myN).comm_map.find(newcomm)->second;
                    vector_info->at(myN).comm_neighs[index_new].push_back(i);
                    vector_info->at(myN).in_degree[index_new]=vector_info->at(myN).in_degree[index_new]+1;

                    //Update Clustering Coefficient
                    //No neighbors in this community
                    if(vector_info->at(myN).comm_neighs[index_new].size()==0.0)
                    {vector_info->at(myN).cc[index_new]=1.0;}
                    //Only one neighbor
                    if(vector_info->at(myN).comm_neighs[index_new].size()==1)
                    {vector_info->at(myN).cc[index_new]=0.0;}
                    //More than one neighbor
                    if(vector_info->at(myN).comm_neighs[index_new].size()>1)
                    {


                        vector<int> mynewvector;
                        mynewvector.resize(vector_info->at(i).comm_neighs[index_new].size());

                        list<int> listObject=vector_info->at(i).comm_neighs[index_new];
                        copy(listObject.begin(), listObject.end(), mynewvector.begin());


                        //mynewvector=list_to_vector(vector_info->at(myN).comm_neighs[index_new]);
                        compute_CC(NV,vtxPtr,vtxInd,mynewvector, &cc);
                        vector_info->at(myN).cc[index_new]=cc;
                    }



                }//end of for neighbors

            }//end of if node moved


            //cout << i << "=="<< vector_info->at(i).Comm <<"=="<<vector_info->at(i).perm <<"\n";


        }//end of checking all nodes

        cout << "At Iteration " << iter << " the total permanence is " << sumQ << "\n";
        iter++;
    }//end of while


    //Update the final permanences
    for(int i=0;i<vector_info->size();i++)
    {
        double thisperm=get_permanence_old(i,vector_info->at(i),vector_info->at(i).Comm);
        vector_info->at(i).perm=thisperm;

    }


    return;
}


/******* End of Functions **************/


#endif //GRAPPOLO_PERMANENCECALCULATION_H
