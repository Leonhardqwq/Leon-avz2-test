#include<myTest.h>
void counter(){
    APlant* p=AGetPlantPtr(3, 6,AYYG_42);
    if(p==nullptr and !haveCount){
        isskiping=0,my_cnt++,haveCount=1;
        At(now){
            At(1)Card(ADPG_10,3,6),
            At(2)Card(AYYG_42,3,6),
            At(3)Card(AKFD_35,3,6),
            At(4)Do{haveCount = 0;}
        };
    }
}
void init1(){//slow
    ASetZombies({
        AGL_7,
        ABC_12,
        AHY_32,
    }
    //,ASetZombieMode::INTERNAL
    );
    ASelectCards({
        ABXGTS_44,
        AYYG_42,
        AKFD_35,
        ANGT_30,
        AM_NGT_30,
        ADPG_10,
        AXGTS_39,
        ADC_21,
        AYTZD_2,
        AWG_17
    });
}
ATickRunner tickRunner;
void kill_zombie(){
    for(auto& z:aAliveZombieFilter){
        if(z.Type() == ABC_12 and int(z.Abscissa())<=500 
            and (z.AtWave()+1 == 1 or z.AtWave()+1==10 or z.AtWave()+1 == 20))
            z.State() = 3;
        if(z.Type()==ABC_12 and int(z.Abscissa())<=482 and z.Row()+1!=3)
            z.State() = 3;
        if(z.Type()==AGL_7 and int(z.Abscissa())<=442)
            z.State() = 3;
        if(z.Abscissa()<=40)
            z.State() = 3;
    }
}
void AScript(){
    myTest();
    init1();
    tickRunner.Start(kill_zombie,myMode);
    countRunner.Start(counter);
    ASkipTick([=](){return isskiping;});
}
