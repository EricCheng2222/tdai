//
//  main.cpp
//  Threes
//
//  Created by Eric Cheng on 9/30/18.
//  Copyright © 2018 Eric Cheng. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
#include <chrono>       // std::chrono::system_clock
#include "Board.h"
#include "agent.h"
using namespace std;

extern float ALPHA;
extern float DECAY;

int main(int argc, const char * argv[]) {
    int evilType = 0;
    int playerType = 1;
    int totalPlay = 1000;
    
    bool isLoad = 1;
    bool isSave = 0;
    
    ALPHA = 0.03;
    printf("ALPHA: %f   DECAY: %f\n", ALPHA, DECAY);
    agent evil(evilType);
    agent player(playerType);
    Board b;
    if(isLoad) player.load();
    for (int i=0; i<totalPlay; i++) {
        b.initialize(&evil, &player);
        evil.initialize();
        player.initialize();
        while (b.isFinish()==false) {
            agent *nowPlay = b.whoseTurn();
            int stat = b.playByAgent(nowPlay);
            if (stat==-1) break;
        }
        b.saveStatistic();
        if(i%1000==1000-1 && isSave) b.showStatistic(i+1);
        
        
        if (i<=3000 && isSave) {
            ALPHA = 0.2;//0.2
        }
        else if(i<12500 && isSave){//12500
            ALPHA = 0.1;//0.1
        }
        else if(isSave){
            ALPHA = 0.05;//0.05
        }
    }
    if(isSave)player.save();
    b.showStatistic(totalPlay);
            
            

    
    
    return 0;
}
