//#pragma once
//#ifndef __MYTAS_H__
//#define __MYTAS_H__

#include <avz.h>
#include<dsl/shorthand.h>
#include <string>
using namespace std;
ALogger<APvzGui> pvzLogger; 
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

bool allow_back = 1;
bool back_constraint = 1;
int back_tick = 10;
bool super_pause = 1;
auto myMode = super_pause?ATickRunner::GLOBAL:ATickRunner::ONLY_FIGHT;
//PS1 请在非选卡或战斗界面启动本插件
//PS2 请先使用pt/ptk修改出怪功能后再启用本插件，否则可能导致崩溃
//PS3 回档功能会在D盘下生成"AAA_LH_SL"文件夹存放存档文件，大小约10M
// 1~10 游戏变速，其中5为原速

// Tab 回档0.5s前(最多回档到10s前)
// Q 快捷铲子
// W 高级暂停
// E 一键启动/停用(AFGZXC)
// R 智能铲套
// T 刷新指示灯

// A 显示信息(血量、波数、时间)
// S 普僵系隐身
// D 女仆秘籍(不前进)
// F 气球进家、南瓜啃穿提示
// G 红眼橄榄计数

// Z (在显示信息模式下)显示最右防御域
// X 显示僵尸总血量
// C 卡槽高亮
// V 显示小喷/阳光菇偏移


bool is_pausing=0;
void myPause(){
    is_pausing=!is_pausing;
    if(!super_pause)
        ASetUpdateWindow(is_pausing);
    else
        ASetAdvancedPause(is_pausing,false,AArgb(0, 0, 0, 0));
}


AReplay myReplay;
void mySetReplay(){
    if(!allow_back)return;
    myReplay.SetSaveDirPath("D:\\AAA_LH_SL\\");
    myReplay.SetShowInfo(false);
    if(!back_constraint){
        myReplay.SetMaxSaveCnt(20);
        myReplay.StartRecord(50);
    }
    else {
        myReplay.SetMaxSaveCnt(1000/back_tick);
        myReplay.StartRecord(back_tick);
    }

}
AOnEnterFight( mySetReplay(); );


bool is_Stealth=0;
ATickRunner StealRunner;
void mySteal(){
    for(auto& z:aAliveZombieFilter){
        if( 
            z.Type()==APJ_0 
        ||  z.Type()==ALZ_2
        ||  z.Type()==ATT_4
        ||  z.Type()==ADB_5
        ||  z.Type()==ATM_6
        ||  z.Type()==AYZ_10
        ){
            if(is_Stealth){
                if(z.MRef<bool>(0x18))
                z.MRef<bool>(0x18)=0;//隐身
            } 
            else{
                if(!z.MRef<bool>(0x18))
                z.MRef<bool>(0x18)=1;//显身
            }                
        }
    }
}


APainter myPainter;
//0 红眼 纯红
//1 植物 纯绿 
//2 花盆 纯黄
//3 白眼 白紫
//4 橄榄 巧克力
//5 纯白 
//6 透白(8) 
//7 纯黑 
//8 高透白(4)
//9 高透黑(4) 
//10 透绿(8) 
//11 纯蓝
//12 透红
void mySetColor(int x){
    if(AGetMainObject()==nullptr) return;

    switch (x){
    case 0: myPainter.SetRectColor(0xFFFF0000);   break;
    case 1: myPainter.SetRectColor(0xFF00FF00);   break;
    case 2: myPainter.SetRectColor(0xFFFFFF00);   break;
    case 3: myPainter.SetRectColor(0xFFCCAAE6);   break;
    case 4: myPainter.SetRectColor(0xFFD2691E);   break;
    case 5: myPainter.SetRectColor(0xFFFFFFFF);   break;
    case 6: myPainter.SetRectColor(0x80FFFFFF);   break;
    case 7: myPainter.SetRectColor(0xFF000000);   break;
    case 8: myPainter.SetRectColor(0x40FFFFFF);   break;
    case 9: myPainter.SetRectColor(0x40000000);   break;
    case 10:myPainter.SetRectColor(0x8000FF00);   break;
    case 11:myPainter.SetRectColor(0xFF0000FF);   break;
    case 12:myPainter.SetRectColor(0x80FF0000);   break;
    }
}
void myBar(int xl,int xr,int yl,int yr){ 
    if(AGetMainObject()==nullptr) return;   
    myPainter.Draw(ARect(xl,yl,xr-xl+1,yr-yl+1));    
}
void myText(std::string text,int x,int y){
    if(AGetMainObject()==nullptr) return;
    aPainter.Draw(AText(text,x,y));
}
void myLogger(std::string text){
    if(AGetMainObject()==nullptr) return;
    pvzLogger.Info(text);
}
void mySetTextColor(){
    if(AGetMainObject()==nullptr) return;
    aPainter.SetTextColor(0xFFFFFFFF);
}
void mySetInitLogger(){
    if(AGetMainObject()==nullptr) return;
    pvzLogger.SetHeaderStyle("");
}


bool is_counting=0;
int giga_total_num=0;
ATickRunner CountGigaRunner;
void myShowCount(){
    if(!is_counting)return;
    int tmpS=AGetMainObject()->Scene(); //01,23,45
    int giga_cnt[6] = {0,0,0,0,0,0};
    int gl_cnt[6] = {0,0,0,0,0,0};
    int y[6] = {};
    int row_h = 0;
    if(tmpS<=1 or tmpS>=6)         y[0]=80+30,row_h=100;
    else if (tmpS<=3)   y[0]=80+30,row_h=85;
    else if (tmpS<=5)   y[0]=170+30,row_h=85;
    for(int i = 1;i<6;i++)  y[i]=y[i-1]+row_h;
    for(auto& z:aAliveZombieFilter){
        if(z.Type()==AHY_32)
            giga_cnt[z.Row()]++;
        else if(z.Type()==AGL_7)
            gl_cnt[z.Row()]++;
    }
    for(int i =0;i<6;i++){
        if(tmpS>=2 and tmpS<=3){    if(i==2 or i==3)    continue;}
        else{   if(i==5)    continue;}
        myText(std::to_string(giga_cnt[i])+"/"+std::to_string(gl_cnt[i])
        ,10,y[i]);
    }

    myText(std::to_string(giga_total_num), 10, 85);
    int t = ANowTime(ANowWave());
    if(t!=1)return;
    giga_total_num = 0;
    int w = AGetMainObject()->Wave();
    auto list = AGetMainObject()->ZombieList();
    for(int i = 0;i<50*w;i++)
        if( *(list+i) == AHY_32 )
            giga_total_num++;
}


bool is_SmartS=0;
ATickRunner SmartSRunner;
struct myPlant{
    APlant* PlantPtr;
    bool _exist;
    myPlant(APlant* p){
        _exist=1;
        PlantPtr=p;
    }
    void shovel(){  _exist=0;}
    bool exist(){   return _exist;  }
};
struct myPair{
    AZombie* ZombiePtr;
    vector<myPlant*> myPlantPtrV;
    myPair(AZombie* z){
        ZombiePtr=z;
        myPlantPtrV.clear();
    }
    bool isPlantExist(int i){
        if(i<myPlantPtrV.size())    return myPlantPtrV[i]->exist();
        return 0;
    }
    APlant* getPlant(int i){
        if(isPlantExist(i)) return myPlantPtrV[i]->PlantPtr;
        return nullptr;
    }
    myPlant* getMyPlant(int i){
        if(isPlantExist(i)) return myPlantPtrV[i];
        return nullptr;
    }
    void pushPlant(myPlant* p){ myPlantPtrV.push_back(p);   }
    void shovelPlant(int i){myPlantPtrV[i]->shovel();}
    int getSize(){return myPlantPtrV.size();}
};
int myGetPlantId(myPlant* myPlantPtr){ return AGetPlantIndex(myPlantPtr->PlantPtr->Row()+1,myPlantPtr->PlantPtr->Col()+1,myPlantPtr->PlantPtr->Type());}
void myShovel(myPlant* myPlantPtr){    
    myPlantPtr->shovel();
    AShovel(myPlantPtr->PlantPtr->Row()+1,myPlantPtr->PlantPtr->Col()+1,myPlantPtr->PlantPtr->Type());  
    myLogger("auto shovel");
}
void mySmartS(){
    if(!is_SmartS)return;
    mySetColor(2);
    myBar(0,30,570,600);
    vector<myPair> PV;
    PV.clear();
    for(auto& z:aAliveZombieFilter)
    if((z.Type()==ABY_23 || z.Type()==AHY_32) && z.State()==70){
        float completing = (AGetAnimationArray()+z.MRef<uint16_t>(0x118))->CirculationRate();
        if(0.64242 <= completing && completing<= 0.64485){
            if(z.FixationCountdown() || z.FreezeCountdown())    continue;
            PV.push_back(myPair(&z));
        }
    }
    if(!PV.size())    return;
    for(auto& p:aAlivePlantFilter){
        if(p.Type()==AYMJNP_47)continue;
        bool is_used=0;
        myPlant* mp=new myPlant(&p);
        for(int i=0;i<PV.size();i++){
            AZombie& z = *(PV[i].ZombiePtr);
            int zl=z.Abscissa()-30,zr=zl+89;
            int pl,pr;
            switch (p.Type()){
            case ANGT_30:   pl=p.Abscissa()+20,pr=pl+60; break;
            case AGJG_23:   pl=p.Abscissa()+30,pr=pl+40; break;
            default:        pl=p.Abscissa()+30,pr=pl+20; break;
            }
            if(p.Row()==z.Row() && pr>=zl && zr>=pl){
                is_used=1;
                PV[i].pushPlant(mp);
            }
        }
        if(is_used)continue;
        delete mp;
    }
    for(int i=0;i<PV.size();i++){
        myPair pr=PV[i];
        vector<myPlant*> v1,v2;
        v1.clear();v2.clear();
        for(int j=0;j<pr.getSize();j++){
            myPlant* tmp=pr.getMyPlant(j);
            if(tmp==nullptr)continue;
            if(v1.empty()){ v1.push_back(tmp);continue;}
            if(tmp->PlantPtr->Col()==v1[0]->PlantPtr->Col()){   v1.push_back(tmp);continue; }
            v2.push_back(tmp);
        }
        if(v1.empty())continue;
        if(v2.empty()){
            for(int j=0;j<v1.size();j++){
                for(int k=j+1;k<v1.size();k++){//k>j
                    if(v1[k]->PlantPtr->Type()==ANGT_30){myPlant* tmp=v1[j];v1[j]=v1[k];v1[k]=tmp;continue;}
                    if(v1[j]->PlantPtr->Type()==AHP_33){myPlant* tmp=v1[j];v1[j]=v1[k];v1[k]=tmp;continue;}
                }
            }
            if(v1[0]->PlantPtr->Type()==ANGT_30 && v1.size()==1)    myShovel(v1[0]);
            else if((v1[0]->PlantPtr->Type()==AXPG_8 || v1[0]->PlantPtr->Type()==AYGG_9) && v1.size()==1 
            &&  v1[0]->PlantPtr->Abscissa() > (v1[0]->PlantPtr->Col())*80+40)
                myShovel(v1[0]);          
            continue;
        }
        if(v1[0]->PlantPtr->Col()>v2[0]->PlantPtr->Col()){  vector<myPlant*> tmp;tmp=v1;v1=v2;v2=tmp;    }
        int max1=-1,min2=10000;
        for(int j=0;j<v1.size();j++){
            for(int k=j+1;k<v1.size();k++){//k>j
                if(v1[k]->PlantPtr->Type()==ANGT_30){myPlant* tmp=v1[j];v1[j]=v1[k];v1[k]=tmp;continue;}
                if(v1[j]->PlantPtr->Type()==AHP_33){myPlant* tmp=v1[j];v1[j]=v1[k];v1[k]=tmp;continue;}
            }
            int tmp=myGetPlantId(v1[j]);
            max1=tmp>max1?tmp:max1;
        }
        for(int j=0;j<v2.size();j++){
            for(int k=j+1;k<v2.size();k++){//k>j
                if(v2[k]->PlantPtr->Type()==ANGT_30){myPlant* tmp=v2[j];v2[j]=v2[k];v2[k]=tmp;continue;}
                if(v2[j]->PlantPtr->Type()==AHP_33){myPlant* tmp=v2[j];v2[j]=v2[k];v2[k]=tmp;continue;}
            }
            int tmp=myGetPlantId(v2[j]);
            min2=tmp<min2?tmp:min2;
        }     
        if(max1<min2){
            if(v1[0]->PlantPtr->Type()==ANGT_30 && v1.size()==1)    myShovel(v1[0]);
            else if((v1[0]->PlantPtr->Type()==AXPG_8 || v1[0]->PlantPtr->Type()==AYGG_9) && v1.size()==1 
            &&  v1[0]->PlantPtr->Abscissa() > (v1[0]->PlantPtr->Col())*80+40)
                myShovel(v1[0]); 
            else continue;
        }
        for(int li=0;li<v1.size();li++){
            int nl=myGetPlantId(v1[li]);
            if(nl<min2)
                myShovel(v1[li]);
            else{
                for(int ri=0;ri<v2.size();ri++)
                    if(nl<myGetPlantId(v2[ri]))     
                        myShovel(v2[ri]);   
            }
        }
    }
}
ATickRunner SmartS2Runner;
void mySmartS2(){
    if(!is_SmartS)return;
    int tmpS=AGetMainObject()->Scene();
    bool isW=(tmpS==2 || tmpS==3);
    APlant* myP[6]={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    for(auto& p:aAlivePlantFilter){
        if(p.Type()!=ANGT_30) continue;
        int Ri=p.Row();
        if(isW && (Ri==2 || Ri==3))continue;
        if(!myP[Ri]){   myP[Ri]=&p;continue;    }
        if(myP[Ri]->Col()<p.Col()){   myP[Ri]=&p;continue;    }
    }
    AZombie* myZ[6]={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    for(auto& z:aAliveZombieFilter){
        if(z.Type()!=ABC_12 && z.Type()!=ATL_22) continue;
        int Ri=z.Row();
        if(isW && (Ri==2 || Ri==3))continue;
        if(!myZ[Ri]){   myZ[Ri]=&z;continue;    }
        if(myZ[Ri]->Abscissa()>z.Abscissa()){   myZ[Ri]=&z;continue;}
    }
    for(int i=0;i<6;i++){
        if(!myP[i])continue;
        if(!myZ[i])continue;
        if(myZ[i]->Abscissa()+10 < myP[i]->Abscissa()+80+1.0){
            AShovel(myP[i]->Row()+1,myP[i]->Col()+1,ANGT_30);
            myLogger("auto shovel");
        }
    }
}


bool qq_rmding=0,ngt_rmding=0,is_rmd=0;
ATickRunner QQRunner;
ATickRunner NGTRunner;
bool QQdanger(){
    for(auto& z:aAliveZombieFilter)
        if(z.Type()==AQQ_16 && z.Abscissa()<=70)
            return 1;
    return 0;
}
bool NGTdanger(){
    for(auto& p:aAlivePlantFilter)
        if(p.Type()==ANGT_30 && p.Hp()<=500)
            return 1;
    return 0;
}
void rmd_qq(){
    if(!is_rmd)return;
    if(QQdanger()){if (!qq_rmding) {
        myLogger("eyes disability");
        //myPause();
        qq_rmding=1;
        return; 
    }}
    else    qq_rmding=0;  
}
void rmd_ngt(){
    if(!is_rmd)return;
    if(NGTdanger()){if(!ngt_rmding){
        myLogger("pay attention to NGT");
        ngt_rmding=1;
        return;
    }}
    else    ngt_rmding=0;
}


bool is_drawing=0;
ATickRunner drawRunner;
void show_ice_road(int t){
    auto ob_ptr = AGetMainObject();
    std::string info = "";
    for (int i=0;i<6;i++){
        int xi = ob_ptr->MRef<int>(0x60C + 0x4*i),
            ti = ob_ptr->MRef<int>(0x624 + 0x4*i),
            co = 1+int((xi-30)/80),
            value = ti+((t<-10000 or t>10000)?0:t);
        if(co<10) info = info + std::to_string(i+1)+"-"+ std::to_string(co)+":"+std::to_string(value)+"\n";
    }
    myText(info, 900,300);
}
void show_crater(int t){
    std::vector<APlaceItem*> item_list;
    item_list.clear();
    for (auto& item:aAlivePlaceItemFilter)
        if (item.Type() == 0x2)
            item_list.push_back(&item);
    std::sort(item_list.begin(),item_list.end(),[](APlaceItem* a,APlaceItem* b){return a->Value()<b->Value();});
    std::string info = "";
    for(auto item_ptr:item_list){
        int value = item_ptr->Value()+((t<-10000 or t>10000)?0:t),
            co = item_ptr->Col(),
            ro = item_ptr->Row();
        info = info + std::to_string(ro+1)+"-"+ std::to_string(co+1)+":"+std::to_string(value)+"\n";
        int xi = AAsm::GridToAbscissa(ro, co),
            yi = AAsm::GridToOrdinate(ro, co);
        int tmp=61*item_ptr->Value()/18000,
            xl=xi+10,xr=xi+70;
        mySetColor(7);
        myBar(xl,xl-1+tmp,yi+75,yi+78);
        mySetColor(6);
        myBar(xl+tmp,xr,yi+75,yi+78);    
    }
    myText(info, 900,100);
} 
void showWave(){
    auto tmp = aPainter.GetRectColor();
    aPainter.SetRectColor(0x00000000);
    int w=AGetMainObject()->Wave();
    if(w<10)    myText("W0"+std::to_string(w),33,10);
    else        myText("W" + std::to_string(w),33,10);
    aPainter.SetRectColor(tmp);
}
void showTime(){
    auto tmp = aPainter.GetRectColor();
    aPainter.SetRectColor(0x00000000);
    int w=AGetMainObject()->Wave();
    int t=ANowTime(ANowWave());
    int ct=AGetMainObject()->RefreshCountdown();
    int x=27,y=35;
    if((w==9 or w==19) and ct<=6)   t=ANowTime(ANowWave()+1);
    if(t<-10000 or t>10000)  myText("????",x,y);
    else if(t<=-100)     myText("-"+std::to_string(-t),x,y);
    else if(t<=-10) myText("-0"+std::to_string(-t),x,y);
    else if(t<0)    myText("-00"+std::to_string(-t),x,y);
    else if(t<10)   myText("000"+std::to_string(t),x,y);
    else if(t<100)  myText("00"+std::to_string(t),x,y);
    else if(t<1000) myText("0"+std::to_string(t),x,y);
    else            myText(std::to_string(t),x,y);
    aPainter.SetRectColor(tmp);
    show_crater(t);
    show_ice_road(t);
}
void myDrawer(){
    if(is_drawing){
        showWave();
        showTime();
        //x:10-70,
        //植物 y:15-18
        //花盆 y:75-78
        //南瓜 y:65-68
        for(auto& p : aAlivePlantFilter)
            if( p.Hp()<p.HpMax() ){
                int Xi=p.Abscissa(),Yi=p.Ordinate(),tmp=61*p.Hp()/p.HpMax();
                int Xl=Xi+10,Xr=Xi+70;
                if(p.Type()==ANGT_30){
                    mySetColor(1);
                    myBar(Xl,Xl-1+tmp,Yi+60,Yi+68);
                    mySetColor(5);
                    myBar(Xl+tmp,Xr,Yi+60,Yi+68);
                }
                else if(p.Type()==AHY_16 || p.Type()==AHP_33){
                    mySetColor(2);
                    myBar(Xl,Xl-1+tmp,Yi+75,Yi+78);
                    mySetColor(5);
                    myBar(Xl+tmp,Xr,Yi+75,Yi+78);
                }
                else{
                    mySetColor(1);
                    myBar(Xl,Xl-1+tmp,Yi+15,Yi+18);
                    mySetColor(5);
                    myBar(Xl+tmp,Xr,Yi+15,Yi+18);
                }
            }
        //白眼 x:40-50,y:31-100(35/42/70,1500/1800/3000)
        //红眼 x:40-50,y:21-110(27/45/72/90,1800/3000/4800/6000)
        //橄榄 x:42-48,y:21-113(5/93,90/1670)              
        for(auto& z:aAliveZombieFilter){
            if(z.Type()==ABY_23){
                int Xi=z.Abscissa(),Yi=z.Ordinate();
                int tmp=70*z.Hp()/3000;

                int Xl=Xi+40,
                    Xr=Xi+50,
                    Yl=Yi+31,
                    Yr=Yi+100,
                    Yn=Yr+1-tmp,
                    y1=Yr+1-35,
                    y2=Yr+1-42;

                mySetColor(3);
                myBar(Xl,Xr,Yn,Yr);
                mySetColor(6);
                myBar(Xl,Xr,Yl,Yn-1);
        
                mySetColor(7);
                myBar(Xl,Xr,Yn,Yn);

                myBar(Xl,Xl,Yl,Yr);
                myBar(Xr,Xr,Yl,Yr);
                myBar(Xl,Xr,Yl,Yl);
                myBar(Xl,Xr,Yr,Yr);

                myBar(Xl,Xr,y1,y1);
                myBar(Xl,Xr,y2,y2);
            }        
            else if(z.Type()==AHY_32){
                int Xi=z.Abscissa(),Yi=z.Ordinate();
                int tmp=90*z.Hp()/6000;

                int Xl=Xi+40,
                    Xr=Xi+50,
                    Yl=Yi+21,
                    Yr=Yi+110,
                    Yn=Yr+1-tmp;
                int y1=Yr+1-27,
                    y2=Yr+1-45,
                    y3=Yr+1-72;     

                mySetColor(0);
                myBar(Xl,Xr,Yn,Yr);
                mySetColor(6);
                myBar(Xl,Xr,Yl,Yn-1);
                
                mySetColor(7);
                myBar(Xl,Xr,Yn,Yn);

                myBar(Xl,Xl,Yl,Yr);
                myBar(Xr,Xr,Yl,Yr);
                myBar(Xl,Xr,Yl,Yl);
                myBar(Xl,Xr,Yr,Yr);

                myBar(Xl,Xr,y1,y1);
                myBar(Xl,Xr,y2,y2);
                myBar(Xl,Xr,y3,y3); 
            }     
            else if(z.Type()==AGL_7){
                int Xi=z.Abscissa(),Yi=z.Ordinate();
                int tmp=93*(z.Hp()+z.OneHp())/1670;
                int Xl=Xi+42,
                    Xr=Xi+48,
                    Yl=Yi+21,
                    Yr=Yi+113,
                    Yn=Yr+1-tmp,
                    y1=Yr+1-5;;
                mySetColor(4);
                myBar(Xl,Xr,Yn,Yr);
                mySetColor(6);
                myBar(Xl,Xr,Yl,Yn-1);
        
                mySetColor(7);
                myBar(Xl,Xr,Yn,Yn);

                myBar(Xl,Xl,Yl,Yr);
                myBar(Xr,Xr,Yl,Yr);
                myBar(Xl,Xr,Yl,Yl);
                myBar(Xl,Xr,Yr,Yr);

                myBar(Xl,Xr,y1,y1);
            }        
        }

    }
}


bool is_Offset=0;
ATickRunner offsetRunner;
void myShowOffset(){
    if(!is_Offset)return;
    for(auto&& p:aAlivePlantFilter)
    if(p.Type()==AXPG_8 || p.Type()==AYGG_9){
        int Xi=p.Abscissa(),Yi=p.Ordinate(),Xstd=p.Col()*80+40;
        int x=Xstd+10,y=Yi+40;
        if(Xi>Xstd)
        myText("R",x,y);
        else if(Xi<Xstd)
        myText("L",x,y);
    }
}


bool is_showCD=0;
ATickRunner CDRunner;
ABasicFilter<ASeed> mySeedFilter;
void myShowCD(){
    if(is_showCD){
        int cnt=0;
        for(auto& s:mySeedFilter){
            int Xl=cnt*51+90,   Xr=Xl+46,
                Yl=10,          Yr=Yl-1+68;
            if(s.IsUsable()){
                mySetColor(10);
                myBar(Xl,Xr,Yl,Yr);
            }
            else{
                int tmp=68*(s.InitialCd()-s.Cd()+1)/(s.InitialCd()+1);
                mySetColor(9);
                myBar(Xl,Xr,Yl,Yl-1+tmp);
                mySetColor(8);
                myBar(Xl,Xr,Yl+tmp,Yr);
            }
            cnt++;
        }
    }
}


bool is_showTrig=0;
bool is_showTotalHp=0;
ATickRunner TrigRunner;
void myShowTrig(){
    if(is_showTrig){
        int t=AGetMainObject()->RefreshCountdown();
        int w=AGetMainObject()->Wave();
        if(w<=0){
            if(t<=200){
                int tmp=51*(200-t)/200;
                mySetColor(11);
                myBar(20,78,8,58-tmp);            
                mySetColor(0);
                myBar(20,78,58+1-tmp,58);
            }   
            else{
                mySetColor(11);
                myBar(20,78,8,58);
            }
            return;
        }       
        if((w==9 or w==19) and t<=6 and -ANowTime(ANowWave()+1)<=200){
            t=-ANowTime(ANowWave()+1);
            int tmp=51*(200-t)/200;
            mySetColor(11);
            myBar(20,78,8,58-tmp);            
            mySetColor(0);
            myBar(20,78,58+1-tmp,58);
            return;
        }
        int currentHp = AAsm::ZombieTotalHp(w - 1);
        int refreshHp = AGetMainObject()->ZombieRefreshHp();
        int totalHp = AGetMainObject()->MRef<int>(0x5598);
        int trig_pivot = 59*(refreshHp)/totalHp;
        int pivot = 59*currentHp/totalHp;
        if(t<=200){
            int tmp=51*(200-t)/200;
            mySetColor(11);
            myBar(20,20-1+pivot,8,58-tmp); 
            mySetColor(7);
            myBar(20+pivot,78,8,58-tmp); 
            mySetColor(0);
            myBar(20+trig_pivot, 20+trig_pivot, 8, 58-tmp);          

            mySetColor(0);
            myBar(20,78,58+1-tmp,58);
        }   
        else{
            mySetColor(11);
            myBar(20,20-1+pivot,8,58);
            mySetColor(7);
            myBar(20+pivot,78,8,58);

            mySetColor(0);
            myBar(20+trig_pivot, 20+trig_pivot, 8, 58);
        } 
    }
    else if(is_showTotalHp){
        int t=AGetMainObject()->RefreshCountdown();
        int w=AGetMainObject()->Wave();
        if(w<=0){
            if(t<=200){
                int tmp=51*(200-t)/200;
                mySetColor(11);
                myBar(20,78,8,58-tmp);            
                mySetColor(0);
                myBar(20,78,58+1-tmp,58);
            }   
            else{
                mySetColor(11);
                myBar(20,78,8,58);
            }
            return;
        }       
        if((w==9 or w==19) and t<=6 and -ANowTime(ANowWave()+1)<=200){
            t=-ANowTime(ANowWave()+1);
            int tmp=51*(200-t)/200;
            mySetColor(11);
            myBar(20,78,8,58-tmp);            
            mySetColor(0);
            myBar(20,78,58+1-tmp,58);
            return;
        }
        if(w <1) return;
        int currentHp = AAsm::ZombieTotalHp(w - 1);
        int totalHp = AGetMainObject()->MRef<int>(0x5598);
        int pivot = 59*currentHp/totalHp;
        mySetColor(11);
        myBar(20,20-1+pivot,8,58);
        mySetColor(7);
        myBar(20+pivot,78,8,58);
    }
}


bool is_defending=0;
ATickRunner defendRunner;
void myDefend(){
    if(is_defending){
        int tmpS=AGetMainObject()->Scene();
        bool isW=(tmpS==2 || tmpS==3);
        APlant* myP[6]={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
        for(auto& p:aAlivePlantFilter){
            int Ri=p.Row();
            if(isW && (Ri==2 || Ri==3))continue;
            if(!myP[Ri]){   myP[Ri]=&p;continue;    }
            int Ci=p.Col();
            if(myP[Ri]->Col()<Ci){   myP[Ri]=&p;continue;    }
            if(myP[Ri]->Col()==Ci && p.Type()==ANGT_30){   myP[Ri]=&p;continue;    }
        }
        for(int i=0;i<6;i++){
            if(!myP[i])continue;

            int pr=myP[i]->Abscissa();
            if(myP[i]->Type()==ANGT_30)     pr+=80;
            else if(myP[i]->Type()==AGJG_23)    pr+=70;
            else if(myP[i]->Type()==AYMJNP_47)  pr+=120;
            else    pr+=50;

            int yl=myP[i]->Ordinate()+30,yr=yl+40;
            int zgl=42,zjr=40;
            //jr +40[-30,58]  gl +42[50,69]
            int Gr=zgl+pr-50,
                Jr=zjr+pr+30;
            mySetColor(8);
            myBar(Gr,Jr,yl,yr);
            mySetColor(7);
            myBar(Jr,Jr,yl,yr);
            myBar(Gr,Gr,yl,yr);
        }
    }  
}


bool is_helping=0;
void myHelp(){
    is_helping=!is_helping;
    is_counting=is_rmd=is_drawing=is_showCD=is_showTotalHp=is_defending=is_helping;
}


void myTAS(){
    ASetReloadMode(AReloadMode::MAIN_UI_OR_FIGHT_UI);

    ASetGameSpeed(1);
    AConnect('1',[=]{ASetGameSpeed(0.1);});
    AConnect('2',[=]{ASetGameSpeed(0.25);});
    AConnect('3',[=]{ASetGameSpeed(0.5);});
    AConnect('4',[=]{ASetGameSpeed(0.75);});
    AConnect('5',[=]{ASetGameSpeed(1);});
    AConnect('6',[=]{ASetGameSpeed(1.5);});
    AConnect('7',[=]{ASetGameSpeed(2);});
    AConnect('8',[=]{ASetGameSpeed(3);});
    AConnect('9',[=]{ASetGameSpeed(5);});
    AConnect('0',[=]{ASetGameSpeed(10);});

    mySetInitLogger();
    mySetTextColor();

    drawRunner.Start(myDrawer,myMode);
    AConnect('A',[=]{
        is_drawing=!is_drawing;
    });

    CDRunner.Start(myShowCD,myMode);
    AConnect('C',[=]{
        is_showCD=!is_showCD;
    });

    TrigRunner.Start(myShowTrig,myMode);
    AConnect('X',[=]{
        is_showTotalHp=!is_showTotalHp;
    }); 
    AConnect('T',[=]{
        is_showTrig=!is_showTrig;
    }); 

    defendRunner.Start(myDefend,myMode);
    AConnect('Z',[=]{
        if(is_drawing)
        is_defending=!is_defending;
    });

    StealRunner.Start(mySteal,myMode);
    AConnect('S',[=]{
        is_Stealth=!is_Stealth;
    });

    is_pausing=0;
    AConnect('W', []{
        myPause();
    });

    AConnect('D',[=]{
        static bool isDancing = false;
        isDancing=!isDancing;
        if(isDancing)   AMaidCheats::Dancing();
        else            AMaidCheats::Stop();
    });
    
    AConnect('Q',[=]{
        int& tmp=AGetMainObject()->MouseAttribution()->Type();
        if(!tmp)tmp=6;
    });
    AConnect('E',[=]{
        myHelp();
    });

    QQRunner.Start(rmd_qq,myMode);
    NGTRunner.Start(rmd_ngt,myMode);
    AConnect('F',[=]{
        is_rmd=!is_rmd;
    });

    SmartSRunner.Start(mySmartS,myMode);
    SmartS2Runner.Start(mySmartS2,myMode);
    AConnect('R',[=]{
        is_SmartS=!is_SmartS;
    });

    offsetRunner.Start(myShowOffset,myMode);
    AConnect('V',[=]{
        is_Offset=!is_Offset;
    });

    giga_total_num=0;
    CountGigaRunner.Start(myShowCount,myMode);
    AConnect('G',[=]{
        is_counting=!is_counting;
    });
    
    AConnect(VK_TAB, [] {
        //myTickBack();
        if(!allow_back)return;
        if(!back_constraint)
            myReplay.ShowOneTick(myReplay.GetRecordIdx() - 1);    
        else{
            int t=ANowTime(ANowWave());
            int w=AGetMainObject()->Wave();
            if(w == 0)  t = t+599;
            if(t>52)
                myReplay.ShowOneTick(myReplay.GetRecordIdx() - (50/back_tick));
            //else if(t>12)
            //    myReplay.ShowOneTick(myReplay.GetRecordIdx() - 10);
            else if(t>back_tick+1)
                myReplay.ShowOneTick(myReplay.GetRecordIdx() - 1);
        }
    });
}
