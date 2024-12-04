//#pragma once
#ifndef __MYPT_H__
#define __MYPT_H__

#include <avz.h>
#include <cstdio>
#include<dsl/shorthand.h>
using namespace std;


void mySetGridType(int row,int col,int type){
    AGetMainObject()->MRef<int>(0x168+0x4*(row+6*col))=type;
}
void mySetRowZombieType(int row,int type){
    AGetMainObject()->MRef<int>(0x5D8+0x4*row)=type;
}
void mySetRowType(int row,bool ispool){
    mySetRowZombieType(row-1,ispool?2:1);
    for(int i=0;i<9;i++)    mySetGridType(row-1,i,ispool?3:1);
}
int myGetRowType(int ro){return AGetMainObject()->MRef<int>(0x5D8+0x4*ro);}
bool MyIsSeedUsable(ASeed* seed){
    if(seed==nullptr)return 0;
    return seed->IsUsable() or seed->InitialCd()==0;
}

ATickRunner YYGalwaysRunner;
vector<APlant*> YYGalwaysList;
void mySetYYGAlways(){
    int fix = 5;
    for(auto p:YYGalwaysList)
        if(p==nullptr)continue;
        else if( p->MRef<int>(0x58) == 1)
            AConnect(ANowDelayTime(1+fix),[=]{
                p->ShootCountdown() = 200-fix;
            });
}
void mySetYYGAlwaysList(const std::vector<AGrid>& lst){
    YYGalwaysList.clear();
    for(auto grid:lst){
        auto p = AGetPlantPtr(grid.row, grid.col,AYYG_42);
        if(p!=nullptr)
            YYGalwaysList.push_back(p);
    }
}
void myYYGAlways(const std::vector<AGrid>& lst){
    mySetYYGAlwaysList(lst);
    YYGalwaysRunner.Start(mySetYYGAlways);
}

#endif