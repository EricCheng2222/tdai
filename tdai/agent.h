//
//  agent.h
//  Threes
//
//  Created by Eric Cheng on 10/1/18.
//  Copyright © 2018 Eric Cheng. All rights reserved.
//

#ifndef agent_h
#define agent_h
#include <chrono>
#include <algorithm>
#include <random>

#include "Board.h"
#include "boardUtilOperation.h"
#include "TDNet.h"
using namespace std;
default_random_engine generator;
uniform_int_distribution<int> distributionEmptySpace(0,15);
uniform_int_distribution<int> actionSpace(0,3);

int badMove = 2;

class agent{
public:
    int actionTaken = -1;//left-->0 down-->1 right-->2 up-->3 for player only
    TDLearn TDL;

    agent(int t){
        type = t;
    }
    
    int getScore(){
        return point;
    }
    double getTime(){
        return time;
    }
    void initialize(){
        point = 0;
        actionTaken = -1;
        reinitSpace();
        reinitBag();
        time = 0;
    }
    int action(vector<vector<int> > &status, int playerActionTaken = -1){
        if (type==0) {//evil
            auto begin = chrono::high_resolution_clock::now();
            shuffleSpace();
            
            vector<int> candSpace;
            switch (playerActionTaken) {
                case 0:
                    candSpace = slideLeftSpace;
                    break;
                case 1:
                    candSpace = slideDownSpace;
                    break;
                case 2:
                    candSpace = slideRightSpace;
                    break;
                case 3:
                    candSpace = slideUpSpace;
                    break;
                default:
                    candSpace = space;
                    break;
            }
            int i = 0;
            int tile = candSpace[i++];
            while (status[tile/4][tile%4]!=0) {
                if (i==candSpace.size()) return -1;
                tile = candSpace[i++];
            }
            if(bag.empty()==true) reinitBag();
            status[tile/4][tile%4] = bag.back();
            bag.pop_back();
            
            auto end = chrono::high_resolution_clock::now();
            auto dur = end - begin;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
            long long dms = (long long)ms;
            
            char c = '0';
            if(0<=tile && tile<=9) c += tile;
            else if (10<=tile && tile<=15) c+=(7+tile);
            
            /*FILE *fp = fopen("ans.txt", "a");
            //FILE *fp = fopen("/Users/eric/Documents/AI/ComputerGamingTheory/Threes/ans.txt", "a");

            fprintf(fp, "%c%d", c, status[tile/4][tile%4]);
            if(dms!=0) fprintf(fp, "(%lld)", dms);
            fclose(fp);*/
        }
        else if(type==1){//player
            auto begin = chrono::high_resolution_clock::now();

            reinitAction();
            actionTaken = actionL.back();
            actionL.pop_back();
            float score;
            float maxScore=-10000;
            int maxAction;
            
            
            //action selection Start
            while (actionL.empty()==false) {
                score=slide(status, false);
                if (maxScore<score) {
                    maxScore = score;
                    maxAction = actionTaken;
                }
                actionTaken = actionL.back();
                actionL.pop_back();
            }
            score=slide(status, false);
            if (maxScore<score) {
                maxScore = score;
                maxAction = actionTaken;
            }
            if(rand()%badMove==0 && badMove<=-1){
                if(rand()%8==0)
                    badMove++;
                maxAction = actionL[rand()%4];
            }
            //action selection End
            
            
            actionTaken = maxAction;
            int rtn = slide(status, true);
            point += ((rtn>=0)?rtn : 0);
            
            auto end = chrono::high_resolution_clock::now();
            auto dur = end - begin;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
            time = (double)ms;
            
            
            char c;
            char sl = '#';
            switch (actionTaken) {
                case 0:
                    c = 'L';
                    break;
                case 1:
                    c = 'D';
                    break;
                case 2:
                    c = 'R';
                    break;
                case 3:
                    c = 'U';
                    break;
            }
            /*FILE *fp = fopen("/Users/eric/Documents/AI/ComputerGamingTheory/Threes/ans.txt", "a");
            fprintf(fp, "%c%c[%d]", sl, c, score);
            if(dms!=0) fprintf(fp, "(%lld)", dms);
            fclose(fp);*/
            return rtn;

        }
        return 0;
    }
    
    void save(){
        
        TDL.save("/Users/eric/Documents/AI/ComputerGamingTheory/tdai/parameter.txt");
    }
    void load(){
        
        TDL.load("/Users/eric/Documents/AI/ComputerGamingTheory/tdai/parameter.txt");
    }
private:
    int type;
    int point; // player only
    vector<int> bag; //evil only
    vector<int> space;//evil only
    vector<int> slideLeftSpace;
    vector<int> slideRightSpace;
    vector<int> slideUpSpace;
    vector<int> slideDownSpace;
    vector<int> actionL; //player only
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    long long time;
    
    
    void shuffleSpace(){
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (space.begin(), space.end(), default_random_engine(seed));
        shuffle (slideRightSpace.begin(), slideRightSpace.end(), default_random_engine(seed));
        shuffle (slideDownSpace.begin(), slideDownSpace.end(), default_random_engine(seed));
        shuffle (slideLeftSpace.begin(), slideLeftSpace.end(), default_random_engine(seed));
        shuffle (slideUpSpace.begin(), slideUpSpace.end(), default_random_engine(seed));
    }
    
    void reinitSpace(){
        space.clear();
        slideLeftSpace.clear();
        slideRightSpace.clear();
        slideUpSpace.clear();
        slideDownSpace.clear();
        for(int i=0; i<16; i++) if(i%4==0) slideRightSpace.push_back(i);
        for(int i=0; i<16; i++) if(i<=3) slideDownSpace.push_back(i);
        for(int i=0; i<16; i++) if(i%4==3) slideLeftSpace.push_back(i);
        for(int i=0; i<16; i++) if(i>=12) slideUpSpace.push_back(i);
        
        
        for(int i=0; i<16; i++) space.push_back(i);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (space.begin(), space.end(), default_random_engine(seed));
        shuffle (slideRightSpace.begin(), slideRightSpace.end(), default_random_engine(seed));
        shuffle (slideDownSpace.begin(), slideDownSpace.end(), default_random_engine(seed));
        shuffle (slideLeftSpace.begin(), slideLeftSpace.end(), default_random_engine(seed));
        shuffle (slideUpSpace.begin(), slideUpSpace.end(), default_random_engine(seed));

    }
    void reinitAction(){
        actionL.clear();
        for(int i=0; i<4; i++) actionL.push_back(i);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (actionL.begin(), actionL.end(), default_random_engine(seed));
    }
    void reinitBag(){
        bag.clear();
        bag.push_back(1);
        bag.push_back(2);
        bag.push_back(3);
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (bag.begin(), bag.end(), default_random_engine(seed));
    }
    float slideLeft(vector<vector<int> > &status, bool isAction){
        static vector<vector<int> > prev;
        prev = status;
        int score = 0;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (c!=3 && status[r][c]==0) {//empty left --> directly shift
                    status[r][c] = status[r][c+1];
                    status[r][c+1] = 0;
                    continue;
                }
                if (c!=3 && ((status[r][c]==1&&status[r][c+1]==2) || (status[r][c]==2&&status[r][c+1]==1))) {//1&&2 merge
                    status[r][c] = 3;
                    status[r][c+1] = 0;
                    score += 3;
                    continue;
                }
                if(c!=3 && status[r][c]>=3 && status[r][c]==status[r][c+1]){//same status merge but not 1/2
                    status[r][c] = status[r][c] + 1;
                    status[r][c+1] = 0;
                    score += pow(3, status[r][c]-2) - 2*pow(3, status[r][c]-3);
                    continue;
                }
                
            }
        }
        float tmp;
        if (prev!=status) {
            tmp = score + TDL.evalBoard(status);
            if(isAction==false) status = prev;
            if(isAction==false) return tmp;
            return score;
        }
        return -1;
    }
    float slide(vector<vector<int> > &status, bool isAction){
        int score;
        switch (actionTaken) {
            case 0:
                score = slideLeft(status, isAction);
                break;
            case 1:
                rotate_right(status);
                score = slideLeft(status, isAction);
                rotate_left(status);
                break;
            case 2:
                reflect_horizontal(status);
                score = slideLeft(status, isAction);
                reflect_horizontal(status);
                break;
            case 3:
                rotate_right(status);
                actionTaken = 2;
                score = slide(status, isAction);
                actionTaken = 3;
                rotate_left(status);
                break;
            default:
                break;
        }
        return score;
    }
};

#endif /* agent_h */
