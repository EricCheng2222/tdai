//
//  TDNet.h
//  threes(TD)
//
//  Created by Eric Cheng on 10/13/18.
//  Copyright © 2018 Eric Cheng. All rights reserved.
//

#ifndef TDNet_h
#define TDNet_h

#include <vector>

using namespace std;

float ALPHA = 0.05;//0.10
float DECAY = 0.75;
struct NtupleNet {
    float table[15][15][15][15] = {0.0};

    float val(int a, int b, int c, int d){
        return table[a][b][c][d];
    }
    void setVal(int a, int b, int c, int d, float V){
        table[a][b][c][d] = V;
    }
    void  update(int a, int b, int c, int d, float delta, bool isDecay=false){
        if(isDecay==true){
            table[a][b][c][d] *= (1.0-ALPHA);
            table[d][c][b][a] *= (1.0-ALPHA);
        }
        else{
            table[a][b][c][d] += delta;
            table[d][c][b][a] += delta;
        }
    }
};

class TDLearn {
public:
    TDLearn(){
        LookUpTable.resize(5);
    }
    
    float evalBoard(vector<vector<int> > &board){
        float sum = 0;
        //column 4-tuple
        sum += LookUpTable[0].val(board[0][0], board[0][1], board[0][2], board[0][3]);
        sum += LookUpTable[0].val(board[3][0], board[3][1], board[3][2], board[3][3]);
        sum += LookUpTable[0].val(board[0][0], board[1][0], board[2][0], board[3][0]);
        sum += LookUpTable[0].val(board[0][3], board[1][3], board[2][3], board[3][3]);
    
        //row 4-tuple
        sum += LookUpTable[1].val(board[1][0], board[1][1], board[1][2], board[1][3]);
        sum += LookUpTable[1].val(board[2][0], board[2][1], board[2][2], board[2][3]);
        sum += LookUpTable[1].val(board[0][2], board[1][2], board[2][2], board[3][2]);
        sum += LookUpTable[1].val(board[0][1], board[1][1], board[2][1], board[3][1]);
        
        //L-shape 4-tuple corner
        sum += LookUpTable[2].val(board[0][1], board[0][0], board[1][0], board[2][0]);
        sum += LookUpTable[2].val(board[2][0], board[3][0], board[3][1], board[3][2]);
        sum += LookUpTable[2].val(board[3][2], board[3][3], board[2][3], board[1][3]);
        sum += LookUpTable[2].val(board[1][3], board[0][3], board[0][2], board[0][1]);
        sum += LookUpTable[2].val(board[1][0], board[0][0], board[0][1], board[0][2]);
        sum += LookUpTable[2].val(board[0][2], board[0][3], board[1][3], board[2][3]);
        sum += LookUpTable[2].val(board[2][3], board[3][3], board[3][2], board[3][1]);
        sum += LookUpTable[2].val(board[3][1], board[3][0], board[2][0], board[1][0]);
        
        //square 4-tuple
        sum += LookUpTable[3].val(board[0][0], board[0][1], board[1][0], board[1][1]);
        sum += LookUpTable[3].val(board[0][1], board[0][2], board[1][1], board[1][2]);
        sum += LookUpTable[3].val(board[0][2], board[0][3], board[1][2], board[1][3]);
        sum += LookUpTable[3].val(board[1][0], board[1][1], board[2][0], board[2][1]);
        sum += LookUpTable[3].val(board[1][1], board[1][2], board[2][1], board[2][2]);
        sum += LookUpTable[3].val(board[1][2], board[1][3], board[2][2], board[2][3]);
        sum += LookUpTable[3].val(board[2][0], board[2][1], board[3][0], board[3][1]);
        sum += LookUpTable[3].val(board[2][1], board[2][2], board[3][1], board[3][2]);
        sum += LookUpTable[3].val(board[2][2], board[2][3], board[3][2], board[3][3]);
        
        //L-shape 4-tuple side
        sum += LookUpTable[4].val(board[1][0], board[1][1], board[1][2], board[2][2]);
        sum += LookUpTable[4].val(board[1][2], board[2][2], board[2][1], board[2][0]);
        sum += LookUpTable[4].val(board[1][0], board[2][0], board[2][1], board[2][2]);
        sum += LookUpTable[4].val(board[2][0], board[1][0], board[1][1], board[1][2]);
        
        sum += LookUpTable[4].val(board[0][1], board[0][2], board[1][2], board[2][2]);
        sum += LookUpTable[4].val(board[0][2], board[0][1], board[1][1], board[2][1]);
        sum += LookUpTable[4].val(board[2][1], board[2][2], board[1][2], board[0][2]);
        sum += LookUpTable[4].val(board[2][2], board[2][1], board[1][1], board[0][1]);
        
        sum += LookUpTable[4].val(board[1][1], board[2][1], board[2][2], board[2][3]);
        sum += LookUpTable[4].val(board[2][1], board[1][1], board[1][2], board[1][3]);
        sum += LookUpTable[4].val(board[1][1], board[1][2], board[1][3], board[2][3]);
        sum += LookUpTable[4].val(board[1][3], board[2][3], board[2][2], board[2][1]);
        
        sum += LookUpTable[4].val(board[1][1], board[2][1], board[3][1], board[3][2]);
        sum += LookUpTable[4].val(board[1][2], board[2][2], board[3][2], board[3][1]);
        sum += LookUpTable[4].val(board[1][1], board[1][2], board[2][2], board[3][2]);
        sum += LookUpTable[4].val(board[2][1], board[1][1], board[2][1], board[3][1]);
        

        return sum;
    }
    
    void  trainBoard(vector<vector<int> >&preAfterState, vector<vector<int> >&nowAfterState, float nowReward, float lambda=1.0){
        float delta;
        bool isDecay = false;
        if ((int)nowReward == -200000) {
            isDecay = true;
        }
        if((int)nowReward == -100000)
            delta = DECAY*(evalBoard(nowAfterState) - evalBoard(preAfterState))/41.0;//0.75
        
        else{
            delta = (evalBoard(nowAfterState) - evalBoard(preAfterState) + nowReward)/41.0;//
            delta *= (ALPHA*lambda);
        }
        
        // line-shape
        LookUpTable[0].update(preAfterState[0][0], preAfterState[0][1], preAfterState[0][2], preAfterState[0][3], delta, isDecay);
        LookUpTable[0].update(preAfterState[3][0], preAfterState[3][1], preAfterState[3][2], preAfterState[3][3], delta, isDecay);
        LookUpTable[0].update(preAfterState[0][0], preAfterState[1][0], preAfterState[2][0], preAfterState[3][0], delta, isDecay);
        LookUpTable[0].update(preAfterState[0][3], preAfterState[1][3], preAfterState[2][3], preAfterState[3][3], delta, isDecay);
        
        LookUpTable[1].update(preAfterState[1][0], preAfterState[1][1], preAfterState[1][2], preAfterState[1][3], delta, isDecay);
        LookUpTable[1].update(preAfterState[2][0], preAfterState[2][1], preAfterState[2][2], preAfterState[2][3], delta, isDecay);
        LookUpTable[1].update(preAfterState[0][2], preAfterState[1][2], preAfterState[2][2], preAfterState[3][2], delta, isDecay);
        LookUpTable[1].update(preAfterState[0][1], preAfterState[1][1], preAfterState[2][1], preAfterState[3][1], delta, isDecay);
        
        //L-shape
        LookUpTable[2].update(preAfterState[0][1], preAfterState[0][0], preAfterState[1][0], preAfterState[2][0], delta, isDecay);
        LookUpTable[2].update(preAfterState[2][0], preAfterState[3][0], preAfterState[3][1], preAfterState[3][2], delta, isDecay);
        LookUpTable[2].update(preAfterState[3][2], preAfterState[3][3], preAfterState[2][3], preAfterState[1][3], delta, isDecay);
        LookUpTable[2].update(preAfterState[1][3], preAfterState[0][3], preAfterState[0][2], preAfterState[0][1], delta, isDecay);
        LookUpTable[2].update(preAfterState[1][0], preAfterState[0][0], preAfterState[0][1], preAfterState[0][2], delta, isDecay);
        LookUpTable[2].update(preAfterState[0][2], preAfterState[0][3], preAfterState[1][3], preAfterState[2][3], delta, isDecay);
        LookUpTable[2].update(preAfterState[2][3], preAfterState[3][3], preAfterState[3][2], preAfterState[3][1], delta, isDecay);
        LookUpTable[2].update(preAfterState[3][1], preAfterState[3][0], preAfterState[2][0], preAfterState[1][0], delta, isDecay);
        
        //four square
         LookUpTable[3].update(preAfterState[0][0], preAfterState[0][1], preAfterState[1][0], preAfterState[1][1], delta, isDecay);
         LookUpTable[3].update(preAfterState[0][1], preAfterState[0][2], preAfterState[1][1], preAfterState[1][2], delta, isDecay);
         LookUpTable[3].update(preAfterState[0][2], preAfterState[0][3], preAfterState[1][2], preAfterState[1][3], delta, isDecay);
        
         LookUpTable[3].update(preAfterState[1][0], preAfterState[1][1], preAfterState[2][0], preAfterState[2][1], delta, isDecay);
         LookUpTable[3].update(preAfterState[1][1], preAfterState[1][2], preAfterState[2][1], preAfterState[2][2], delta, isDecay);
         LookUpTable[3].update(preAfterState[1][2], preAfterState[1][3], preAfterState[2][2], preAfterState[2][3], delta, isDecay);
        
         LookUpTable[3].update(preAfterState[2][0], preAfterState[2][1], preAfterState[3][0], preAfterState[3][1], delta, isDecay);
         LookUpTable[3].update(preAfterState[2][1], preAfterState[2][2], preAfterState[3][1], preAfterState[3][2], delta, isDecay);
         LookUpTable[3].update(preAfterState[2][2], preAfterState[2][3], preAfterState[3][2], preAfterState[3][3], delta, isDecay);
        
        //L-shape side
         LookUpTable[4].update(preAfterState[1][0], preAfterState[1][1], preAfterState[1][2], preAfterState[2][2], delta, isDecay);
         LookUpTable[4].update(preAfterState[1][2], preAfterState[2][2], preAfterState[2][1], preAfterState[2][0], delta, isDecay);
         LookUpTable[4].update(preAfterState[1][0], preAfterState[2][0], preAfterState[2][1], preAfterState[2][2], delta, isDecay);
         LookUpTable[4].update(preAfterState[2][0], preAfterState[1][0], preAfterState[1][1], preAfterState[1][2], delta, isDecay);
        
         LookUpTable[4].update(preAfterState[0][1], preAfterState[0][2], preAfterState[1][2], preAfterState[2][2], delta, isDecay);
         LookUpTable[4].update(preAfterState[0][2], preAfterState[0][1], preAfterState[1][1], preAfterState[2][1], delta, isDecay);
         LookUpTable[4].update(preAfterState[2][1], preAfterState[2][2], preAfterState[1][2], preAfterState[0][2], delta, isDecay);
         LookUpTable[4].update(preAfterState[2][2], preAfterState[2][1], preAfterState[1][1], preAfterState[0][1], delta, isDecay);
        
         LookUpTable[4].update(preAfterState[1][1], preAfterState[2][1], preAfterState[2][2], preAfterState[2][3], delta, isDecay);
         LookUpTable[4].update(preAfterState[2][1], preAfterState[1][1], preAfterState[1][2], preAfterState[1][3], delta, isDecay);
         LookUpTable[4].update(preAfterState[1][1], preAfterState[1][2], preAfterState[1][3], preAfterState[2][3], delta, isDecay);
         LookUpTable[4].update(preAfterState[1][3], preAfterState[2][3], preAfterState[2][2], preAfterState[2][1], delta, isDecay);
        
         LookUpTable[4].update(preAfterState[1][1], preAfterState[2][1], preAfterState[3][1], preAfterState[3][2], delta, isDecay);
         LookUpTable[4].update(preAfterState[1][2], preAfterState[2][2], preAfterState[3][2], preAfterState[3][1], delta, isDecay);
         LookUpTable[4].update(preAfterState[1][1], preAfterState[1][2], preAfterState[2][2], preAfterState[3][2], delta, isDecay);
         LookUpTable[4].update(preAfterState[2][1], preAfterState[1][1], preAfterState[2][1], preAfterState[3][1], delta, isDecay);
    }
    
    void save(char* path){
        FILE *fp;
        fp = fopen(path, "w+");
        int tmpi, tmpj, tmpk, tmpm;
        for (int i=0; i<LookUpTable.size(); i++) {
            for (int j=0; j<15*15*15*15; j++) {
                tmpi = j%15;
                tmpj = ((j-tmpi)/15)%15;
                tmpk = ((j-tmpi-tmpj*15)/15/15)%15;
                tmpm = ((j-tmpi - tmpj*15 - tmpk*15*15)/15/15/15)%15;
                fprintf(fp, "%f\n", LookUpTable[i].val(tmpi, tmpj, tmpk, tmpm));
            }
        }
        fclose(fp);
    }
    void load(char* path){
        FILE *fp = fopen(path, "r");
        int tmpi, tmpj, tmpk, tmpm;
        float scn;
        for (int i=0; i<LookUpTable.size(); i++) {
            for (int j=0; j<15*15*15*15; j++) {
                tmpi = j%15;
                tmpj = ((j-tmpi)/15)%15;
                tmpk = ((j-tmpi-tmpj*15)/15/15)%15;
                tmpm = ((j-tmpi - tmpj*15 - tmpk*15*15)/15/15/15)%15;
                fscanf(fp, "%f", &scn);
                LookUpTable[i].setVal(tmpi, tmpj, tmpk, tmpm, scn);
            }
        }
        fclose(fp);
    }
    
private:
    vector<NtupleNet>LookUpTable;
};



#endif /* TDNet_h */
