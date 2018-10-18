//
//  boardUtilOperation.h
//  Threes
//
//  Created by Eric Cheng on 10/1/18.
//  Copyright © 2018 Eric Cheng. All rights reserved.
//

#ifndef boardUtilOperation_h
#define boardUtilOperation_h

void transpose(vector<vector<int> > &tile) {
    for (int r = 0; r < 4; r++) {
        for (int c = r + 1; c < 4; c++) {
            std::swap(tile[r][c], tile[c][r]);
        }
    }
}

void reflect_horizontal(vector<vector<int> > &tile) {
    for (int r = 0; r < 4; r++) {
        std::swap(tile[r][0], tile[r][3]);
        std::swap(tile[r][1], tile[r][2]);
    }
}

void reflect_vertical(vector<vector<int> > &tile) {
    for (int c = 0; c < 4; c++) {
        std::swap(tile[0][c], tile[3][c]);
        std::swap(tile[1][c], tile[2][c]);
    }
}

void rotate_right(vector<vector<int> > &tile) { transpose(tile); reflect_horizontal(tile); } // clockwise
void rotate_left(vector<vector<int> > &tile) { transpose(tile); reflect_vertical(tile); } // counterclockwise
void reverse(vector<vector<int> > &tile) { reflect_horizontal(tile); reflect_vertical(tile); }

#endif /* boardUtilOperation_h */
