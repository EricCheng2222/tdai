//
//  Board.h
//  Threes
//
//  Created by Eric Cheng on 10/1/18.
//  Copyright © 2018 Eric Cheng. All rights reserved.
//

#ifndef Board_h
#define Board_h
#include <vector>
#include <chrono>
using namespace std;
#include "agent.h"

struct statistic {
    statistic();
    statistic(int LT, int s, double opsP, double opsE){
        largestTile = LT;
        score = s;
        opsPlayer = opsP;
        opsEvil = opsE;
    }
    int largestTile = 0;
    int score = 0;
    double opsPlayer;
    double opsEvil;
};

class Board{
public:
    
    void initialize(agent* x, agent* y){
        evil = x;
        player = y;
        playStep = 0;
        status.clear();
        status.resize(4);
        history.clear();
        preHistory.clear();
        histoReward.clear();
        for (int i=0; i<4; i++) {
            for(int j=0; j<4; j++){
                status[i].push_back(0);
            }
        }
        auto start = chrono::high_resolution_clock::now();
        auto dur = start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur.time_since_epoch()).count();
        long long int dummyTime = (long long)ms;
        
        /*FILE *fp = fopen("/Users/eric/Documents/AI/ComputerGamingTheory/Threes/ans.txt", "a");
        fprintf(fp, "dummy:random@%lld|", dummyTime);
        fclose(fp);*/
    }
    bool isFinish(){
        return false;//temp
    }
    agent* whoseTurn(){
        if (playStep<9 || playStep%2==0) return evil;
        return player;
    }
    int playByAgent(agent *a){
        playStep++;
        int rtnVal = a->action(status, player->actionTaken);

        if(a==player && playStep>10 && rtnVal!=-1){
            history.push_back(status);
            preHistory.push_back(preStatus);
            histoReward.push_back(1.0+sqrt((float)rtnVal+3.0));//1.0 + ((float)rtnVal+0.01)/ (1 + ((float)rtnVal+0.01))
            player->TDL.trainBoard(preStatus, status, 1.0+sqrt((float)rtnVal+3.0));//1.0+sqrt((float)rtnVal+10.0)    1.0+tanh(rtnVal)
        }
        
        if(a==player && rtnVal==-1){
            history.push_back(status);
            preHistory.push_back(preStatus);
            histoReward.push_back(-200000);
            player->TDL.trainBoard(preStatus, status, -200000);
            backProp();
        }
        if(a==player){
            preStatus = status;
            preRtnVal = rtnVal;
        }
        
        return rtnVal;
    }
    void showStatistic(int total){
        
        int countT[16] = {0};
        unsigned long long score = 0;
        int maxScore = 0;
        
        double playerTimeSum = 0;
        double evilTimeSum = 0;
        
        for(auto stat : Statistic){
            countT[stat.largestTile]++;
            score += stat.score;
            playerTimeSum += stat.opsPlayer;
            evilTimeSum += stat.opsEvil;
            if(maxScore < stat.score) maxScore = stat.score;
        }
        printf("%d\t", total);
        printf("avg = %d, ", score/total);
        printf("max = %d, ", maxScore);
        printf("ops = %lf (%lf|%lf)\n", (playerTimeSum+evilTimeSum)/(2*total), playerTimeSum/total, evilTimeSum/total);
        
        

        
        int sumUp[16] = {0};
        for (int i=0; i<16; i++) {
            if (countT[i]==0) continue;
            for (int j=i; j<16; j++) {
                sumUp[i] += countT[j];
            }
        }
        for (int i=0; i<16; i++) {
            int tile;
            if (sumUp[i]!=0) {
                if(i<=3) tile = i;
                else tile = 3*pow(2, i-3);
                printf("\t%d", tile);
                printf("\t%.1f%%", (float)sumUp[i]*100/float(total));
                printf("\t(%.1f%%)\n", (float)(sumUp[i]-sumUp[i+1])*100/float(total));
            }
        }
        
        
    }
    void saveStatistic(){
        int maxT = 0;
        for (int i=0; i<4; i++) {
            for(int j=0; j<4; j++){
                if(maxT < status[i][j]) maxT = status[i][j];
            }
        }
        statistic tmp(maxT, player->getScore(), player->getTime(), evil->getTime());
        Statistic.push_back(tmp);
        auto start = chrono::high_resolution_clock::now();
        auto dur = start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur.time_since_epoch()).count();
        long long int dummyTime = (long long)ms;
        /*FILE *fp = fopen("/Users/eric/Documents/AI/ComputerGamingTheory/Threes/ans.txt", "a");
        fprintf(fp, "|dummy:random@%lld\n", dummyTime);
        fclose(fp);*/
    }
    
private:
    agent* evil;
    agent* player;
    int playStep;
    float preRtnVal;
    float endGamePoint;
    vector<vector<int> > status;
    vector<vector<int> > preStatus;
    vector<statistic> Statistic;
    vector<vector<vector<int> > > history;
    vector<vector<vector<int> > > preHistory;
    vector<int> histoReward;
    
    void backProp(){
        int static i;
        for (i=8; i<history.size();) {
            player->TDL.trainBoard(preHistory.back(), history.back(), histoReward.back(),0.500);
            player->TDL.trainBoard(preHistory[preHistory.size()-2], history.back(), 0,   0.250);
            player->TDL.trainBoard(preHistory[preHistory.size()-3], history.back(), 0,   0.125);
            player->TDL.trainBoard(preHistory[preHistory.size()-4], history.back(), 0,   0.0625);
            player->TDL.trainBoard(preHistory[preHistory.size()-5], history.back(), 0,   0.03125);
            player->TDL.trainBoard(preHistory[preHistory.size()-6], history.back(), 0,   0.01506);
            player->TDL.trainBoard(preHistory[preHistory.size()-7], history.back(), 0,   0.00703);
            player->TDL.trainBoard(preHistory[preHistory.size()-8], history.back(), 0,   0.0035);

            
            history.pop_back();
            preHistory.pop_back();
            histoReward.pop_back();
        }
        for (i=0; i<history.size(); ) {
            player->TDL.trainBoard(preHistory.back(), history.back(), histoReward.back());
            history.pop_back();
            preHistory.pop_back();
            histoReward.pop_back();
        }
        
        
    }
};

#endif /* Board_h */
