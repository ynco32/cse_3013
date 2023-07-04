//
//  main.cpp
//  maze
//
//  Created by Yunji Choe on 2023/05/29.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

int main(int argc, const char * argv[]) {
    
    srand((unsigned int)time(NULL)); //난수 생성용
    
    
    //입력
    int N, M;
//    int N = 0;
//    int M = 0; // 너비 N 높이 M
    cin >> N;
    cin >> M;
//
//    scanf("%d", &N);
//    scanf("%d", &M);
    
    
    char result[61][61] = {'\0'};
    int current_row[30] = {-1, };
    int next_row[30] = {-1, };
    
    bool wall_R[30][30]; //가로 벽 유무
    bool wall_C[30][30]; //세로 벽 유무
    int random;
    
    //첫번째 줄 init
    int group = 0;
    //g = 0;
//    if (N>31){
//        cin >> N;
//    } else{
    for (int i = 0; i < N; i++){
        current_row[i] = group;
        group++;
//        cout << "group: " << group <<endl;
    }
//    }
    
    //init
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            wall_R[i][j] = true;
            wall_C[i][j] = true;
        }
    }
    
    
    int r, c;
    for (r = 0; r < M-1; r++){
        //!ppt5
        for (c = 0; c < N-1; c++){
            if (current_row[c] != current_row[c+1]){
                random = rand(); //rand:짝수 벽 삭제 | 홀수 벽 유지
                if (random%2 == 0){
                    //벽 삭제
                    wall_R[r][c] = false;
                    current_row[c+1] = current_row[c];
                }
            }
        }
        
        //!ppt6
        random = rand();
        if (random % 2 == 0){
            wall_C[r][0] = false;
        }
        //수직경로 이미 존재
        for (c = 1; c < N; c++) {
            if (wall_C[r][c-1] == true) {
                random = rand();
                if (random % 2 == 0){
                    wall_C[r][c] = false;
                    next_row[c] = current_row[c];
                }
            }
        }
        
        //수직경로 존재하지 않음
        bool flag_path_vertical = false;
        for (c = 0; c < N; c++) {
            flag_path_vertical = false;
            for (int k = 0; k < N; k++) {
                if (current_row[c] == current_row[k]){
                    //이전 그룹 타일에 수직경로 존재하는지 확인
                    if (wall_C[r][k] == false) {
                        flag_path_vertical = true;
                    }
                }
            }
            
            //이전 그룹에 수직경로 존재하지 않음
            if (!flag_path_vertical) {
                wall_C[r][c] = false;
                next_row[c] = current_row[c];
            }
        }
        
        
        //다음 줄로 내려감
        for (c = 0; c < N; c++) {
            if (next_row[c] == -1) {
                group++;
                next_row[c] = group;
            }
        }
   //     cout << "group: " << group <<endl;
        
        for (int init = 0; init < N; init++){
            current_row[init] = next_row[init];
            next_row[init] = -1;
        }
        
    }
        
    for (c = 0; c < N-1; c++){
        if (current_row[c] != current_row[c+1]){
            random = rand()%2; //rand:짝수 벽 삭제 | 홀수 벽 유지
            wall_R[M-1][c] = false;
        }
    }
    
    
        
//        bool flag_path;
//
//        for (c = 0; c < N; c++){
//            //수직 경로 하나 생성했을 때
//            if (flag_path){
//                //같은 그룹에 대해서는 임의로 결정
//                if (current_row[c] == current_row[c+1]){
//                    random = rand();
//                    if (random%2 == 0){
//                        wall_C[r+1][c] = false;
//                        next_row[c] = current_row[c];
//                    }
//                    else continue;
//                }
//                else {
//                    flag_path != false;
//
//                }
//            }
//            else {
//            }
//
//            random = rand();
//            if (random %2 == 0){
//                wall_C[r+1][c] = false;
//                next_row[c] = current_row[c];
//            }
//            else {
//                g++;
//                next_row[c] = g;
//            }
//        }
//
//
//    }
    
    
    
    //미로 그리기
    for (int i = 0; i <= M; i++){
        for (int j = 0; j < 2*N; j+=2){
            result[i*2][j] = '+';
            result[i*2][j+1] = '-';
        }
        result[i*2][2*N] = '+';
    }
    
    for (int i = 0; i < M; i++){
        for (int j = 0; j <= 2*N; j+=2){
            result[i*2+1][j] = '|';
            result[i*2+1][j+1] = ' ';
        }
        //result[i*2+1][2*N-1] = '|';
    }
    
//    cout << " ---------- init ------------------ \n";
//    for (int i = 0; i <= 2*M; i++){
//        for (int j = 0; j <= 2*N; j++){
//            cout << result[i][j];
//        }
//        cout <<endl;
//    }
//
//    cout << " ------------------------------- \n";
    
    
//    for (c = 0; c < 2*N; c+=2) {
//        result[0][c] = '+';
//        result[0][c+1] = '-';
//    }
//
//    for (r = 1; r < M; r++) {
//        for (c = 0; c < N; c++) {
//            result[r][c] = '+';
//
//            if (wall_C[r][c]) {
//                result[r][c] = '-';
//            }
//            else {
//                result[r][c] = ' ';
//            }
//        }
//
//        result[r][N] = '+';
//
//
//        for (c = 0; c < N - 1; c++) {
//            if (!wall_R[r][c]){
//                result[r][c] = '|';
//            }
//
//            else{
//                result[r][c] = ' ';
//            }
////            cout << " ";
//        }
//
//    }
    
//    for (c = 0; c < N*2; c+=2) {
//        result[2*M-1][c] = '+';
//        result[2*M-1][c+1] = '-';
//    }
//    result[2*M][2*N-1] = '+';
    
    /*
    
    cout << " ---------- w_C ------------------ \n";
    for (r = 0; r < M; r++){
        for (c = 0; c < N; c++){
            cout << wall_C[r][c];
        }
        cout <<endl;
    }
    cout << " ---------- w_R ------------------ \n";
    for (r = 0; r < M; r++){
        for (c = 0; c < N; c++){
            cout << wall_R[r][c];
        }
        cout <<endl;
    }
    cout << " -------------------------------- \n";
    
    */
    
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            if (wall_R[i][j] == false){
                int row_res = 2*i+1;
                int col_res = 2*j+2;
                result[row_res][col_res] = ' ';
//                printf("wall_R[%d][%d] == false\n",i,j);
//                printf("%d , %d\n", row_res, col_res);
            }
            //printf("\n");
            if (wall_C[i][j] == false){
                int row_res = 2*i+2;
                int col_res = 2*j+1;
                result[row_res][col_res] = ' ';
//                printf("wall_C[%d][%d] == false\n",i,j);
//                printf("%d , %d\n", row_res, col_res);
            }
        }
    }
    
    
    
    //test
//    cout << " ---------- result ------------------ \n";
    for (int i = 0; i <= 2*M; i++){
        for (int j = 0; j <= 2*N; j++){
            cout << result[i][j];
        }
        cout <<endl;
    }
    
//    cout << " ------------------------------- \n";
    
    
    // txt 파일로 출력
    FILE *output_fp = fopen("maze.maz", "w");
    
    for (int i = 0; i <= 2*M; i++){
        for (int j = 0; j <= 2*N; j++){
            fprintf(output_fp, "%c", result[i][j]);
        }
        fprintf(output_fp, "\n");
    }
    fclose(output_fp);
   
 return 0;
}
