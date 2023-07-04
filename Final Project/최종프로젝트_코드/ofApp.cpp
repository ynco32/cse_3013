#include "ofApp.h"
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // frameRaate을 설정한다.
    srand((unsigned int)time(NULL)); //random하게 사다리를 만들기 위한 변수
    ofBackground(255,255,255); // 배경색을 설정한다.
    ofSetLineWidth(4); // 선의 굵기를 설정한다.
    
    setting_flag = 0; //초기 설정값: 사다리의 크기와 유저 수를 받고 사다리를 초기화한다.
    currentIndex = 0; //현재 선택한 사다리의 index를 초기화한다.
    draw_flag = 0; // 사다리를 그리는지 여부를 flag로 조절한다.
    //ladder_start_flag = 0; //사다리타기를 시작하는지 여부를 flag로 조절한다.
    ladder_set = 0; // 사다리를 설정했는지를 확인한다.
    
    start();
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // 사다리는 검은색, wid 4로 그린다.
    ofSetColor(0);
    ofSetLineWidth(4);
    
    //설정이 완료되었다면
    if (setting_flag){
        //cout <<"in setting flag\n";
        
        for (int i = 0; i < num_of_ladder; i++){
            //세로선 그리기
            ofDrawLine(100 + i*LEN, 100, 100 + i*LEN, 100 + WID * num_of_step + WID );
            //어떤 사다리가 현재 선택되어있는지를 확인하기 위한 점을 추가한다.
            ofDrawCircle(100 + i*LEN, 100 - 20, 5);
        }
        
        //선택된 사다리는 점을 초록색으로 표기한다.
        ofSetColor(0,255,0);
        ofDrawCircle(100 + LEN * currentIndex, 100 - 20, 5);
        ofSetColor(0);
    }
    
    //새로운 사다리가 설정되었다면
    if (ladder_set){
        //cout <<"in ladder_set\n";
        
        //이어진 가로선을 그린다.
        for (int i = 0; i < num_of_ladder-1; i++){
            for (int j = 0; j < num_of_step; j++){
                //사다리가 이어져 있을 때 그림
                if (ladder[i][j] == true){
                    ofDrawLine(100 + i*LEN, 150 + j*WID, 100 + (i+1)*LEN, 150 + j*WID );
                }
            }
        }
    }
    
    //경로의 계산이 끝났다면
    if (draw_flag){
        //경로는 width 5, 빨간색으로 그린다.
        ofSetColor(255,0,0);
        ofSetLineWidth(5);
        
        //line segment에 저장된 path의 좌표를 이용해 그린다.
        int step_idx = 0;
        int i = 0;
        for (; i < vertical_ladder[currentIndex].path.size(); ){
            //path 에 동일한 step이 연달아 저장되었다면, 가로줄을 건넜다는 뜻이다.
            if (i < vertical_ladder[currentIndex].path.size() -1 && vertical_ladder[currentIndex].path[i].second == vertical_ladder[currentIndex].path[i+1].second){
                //세로선 그리기
                ofDrawLine(100 + vertical_ladder[currentIndex].path[i].first * LEN, 100 + step_idx*WID, 100 + vertical_ladder[currentIndex].path[i].first * LEN, 100 + (step_idx+1)*WID);
                
                //가로선 그리기
                ofDrawLine(100 + vertical_ladder[currentIndex].path[i].first * LEN, 150 + step_idx*WID, 100 + vertical_ladder[currentIndex].path[i+1].first * LEN, 150 + step_idx*WID);
                step_idx++;
                i += 2;
                
            }
            //그게 아니라면 아래(세로)로만 움직였다는 뜻이다.
            else {
                //세로선 그리기
                ofDrawLine(100 + vertical_ladder[currentIndex].path[i].first * LEN, 100 + step_idx*WID, 100 + vertical_ladder[currentIndex].path[i].first * LEN, 100 + (step_idx+1)*WID);
                step_idx++;
                i++;
            }
        }
        //마지막 결과로 이어지는 부분을 이어서 그려주고 결과를 알아보기 쉽게 빨간색 점을 그려준다.
        ofDrawLine(100 + vertical_ladder[currentIndex].path[i-1].first * LEN, 100 + num_of_step * WID, 100 + vertical_ladder[currentIndex].path[i-1].first * LEN, 100 + (num_of_step+1)*WID);
        ofDrawCircle(100 + vertical_ladder[currentIndex].path[i-1].first *LEN, 150 + num_of_step * WID + 30, 10);
        
        //안내 메세지를 띄워준다.
        ofSetColor(100, 100, 255);
        ofDrawRectangle(100, 300 + num_of_step * 50 , num_of_ladder * 100, 100 );
        ofSetColor(100, 255, 100);
        ofDrawBitmapString("Press Q to Quit!", 200, 350 + num_of_step * 50);
        ofDrawBitmapString("Press UP to See Other Result!", 200, 370 + num_of_step * 50);
        
    }
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //v를 눌렀을때
    if (key == 'v') {
        cout << "\n\t=> V pressed\n";
        //색에 대한 버퍼
        glReadBuffer(GL_FRONT);
        //timestamp와 함께 스크린샷을 저장한다.
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    
    //q를 누르면 프로그램을 종료한다.
    if( key == 'q' || key == 'Q'){
        cout << "\n\t=> Q pressed\n";
        // flag 전부 초기화
        setting_flag = 0;
        currentIndex = 0;
        draw_flag = 0;
        ladder_start_flag = 0;

        // 벡터 초기화
        for (int i = 0; i < num_of_ladder; i++){
            ladder[i].clear();
            vertical_ladder.clear();
        }
        ladder.clear();
        cout << "\n\n ======= GOOD BYE ======= \n\n";
        _Exit(0);
    }
    
    
    //d를 누르면 그리기를 시작한다.
    if (key == 'd' || key == 'D'){
        //사다리가 그려졌는지를 확인
        cout << "\n\t=> D pressed\n";
        
        if (ladder_set){
            //경로를 계산한다.
            calculate_path(currentIndex);
            //cout << "::Calculate Path\n";
            //cout<<"::draw_ladder\n";
        }
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    //l 키를 눌렀다 땔 때
    if( key == 'l' || key == 'L'){
        cout << "\n\t=> L pressed\n";
        //설정이 완료되었는지 확인한다
        if (setting_flag == 1){
            //설정값으로 사다리를 설정하고
            makeLadder();
            //cout << "::LADDER Made\n";
            
            //설정이 완료되었다는 것을 flag에 저장한다.
            ladder_set = 1;
            
            
            //2차원 배열로 사다리 값을 확인할 수 있다 (추가확인을 위한 print문)
            cout << "\n::LADDER SET\n";
            for (int i = 0; i < num_of_ladder-1;i++){
                for (int j = 0; j < num_of_step;j++){
                    cout << ladder[i][j];
                }
                cout <<endl;
            }
            cout <<endl;
            cout <<endl;
        }
        
    }
    
    //키보드 좌, 우 키를 이용해 선택된 사다리를 조정할 수 있다.
    //index가 범위를 넘어가면 초기화해준다.
    if( key == OF_KEY_RIGHT){
        ladder_start_flag = 0;
        currentIndex++;
        if(currentIndex == num_of_ladder)
            currentIndex = 0;
        
        cout << "Currently selected: "<<currentIndex<<"\n";
    }
    
    if( key == OF_KEY_LEFT){
        ladder_start_flag = 0;
        currentIndex--;
        if (currentIndex < 0)
            currentIndex = num_of_ladder-1;
        cout << "Currently selected: "<<currentIndex<<"\n";
    }
    
    
    // 키보드 위를 이용해 그렸던 경로를 지울 수 있다.
    if( key == OF_KEY_UP){
        draw_flag = 0;
        vertical_ladder[currentIndex].path.clear();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void ofApp::makeLadder(){
    //사다리를 그리기 위한 함수.
    // 가로/세로 당 가로 줄이 몇개씩 있는지를 저장한다.
    vector<int> cnt_ver;
    vector<int> cnt_hor;
    
    for (int j = 0; j < num_of_step; j++){
        cnt_ver.push_back(0);
    }
    
    for (int i = 0; i < num_of_ladder-1; i++){
        cnt_hor.push_back(0);
    }
    
    
    //가장 첫번째 세로줄에 대해 랜덤하게 가로줄을 설정한다.
    for (int j = 0; j < num_of_step; j++){
        int random = rand()%2;
        if (random == 1){
            ladder[0][j] = true;
            cnt_ver[j]++;
            cnt_hor[0]++;
        }
    }
    
    //두번째 줄부터는 왼쪽에 가로줄이 없어야 가로줄을 설정하도록 한다.
    //이어지는 가로줄이 없도록 하기 위해서이다.
    for (int i = 1; i < num_of_ladder-1; i++){
        for (int j = 0; j < num_of_step; j++){
            int random = rand()%2;
            //미로가 한 줄에 밀집되는 것을 최대한 방지하기 위해 최대 step-1개까지 가로줄을 그을 수 있도록 한다.
            if (random == 1 && cnt_ver[j] < num_of_step-1){
                if (ladder[i-1][j] == false){
                    ladder[i][j] = true;
                    cnt_ver[j]++;
                    cnt_hor[i]++;
                }
            }
        }
    }
    
//    for (int j = 0; j < num_of_step; j++){
//        cout << cnt_ver[j] << ", ";
//    }
//    cout <<endl;
//    for (int i = 0; i < num_of_ladder-1; i++){
//        cout << cnt_hor[i] << ", ";
//    }
//    cout <<endl;
    
    
    
    //만일 단절된 부분이 있다면 이어준다.
    for (int i = 0; i < num_of_ladder-1; i++){
        if (cnt_hor[i] == 0){
            for (int j = 0; j < num_of_step; j++){
                if (i < num_of_ladder - 2 && ladder[i+1][j] == false){
                    if (i > 0 && ladder[i-1][j] == false){
                        ladder[i][j] = true;
                        cnt_ver[j]++;
                        cnt_hor[i]++;
                        break;
                        
                    }
                }
                
            }
        }
    }

    
//    printf("\n|| after \n");
//    for (int j = 0; j < num_of_step; j++){
//        cout << cnt_ver[j] << ", ";
//    }
//    cout <<endl;
//    for (int i = 0; i < num_of_ladder-1; i++){
//        cout << cnt_hor[i] << ", ";
//    }
//    cout <<endl;
    
    //설정이 완료되었다는 것을 flag에 저장한다.
    ladder_set = 1;
}



void ofApp::start(){
    cout << " ================  START  ===============\n";
    
    //총 사용자수 = 세로줄 수 = 사다리의 수를 입력받는다.
    cout << "Enter the number of total users:\n";
    cin >> num_of_ladder;
        
    if (num_of_ladder < 2) {
        cout << "Total users must be more than 1";
        cin >> num_of_ladder;
    }
        
    
    // 사다리의 크기 = 가로줄 수를 입력받는다.
    cout << "Enter the size of the ladder:\n";
    cin >> num_of_step;
    
    if (num_of_step < 1) {
        cout << "The size of the ladder must bigger than 1";
        cin >> num_of_step;
    }
    

    
    
    cout << "\nPress L: Set New Ladder\n";
    cout << "Press D: Draw Ladder\n";
    cout << "Press V: Save ScreenShoot\n";
    cout << "Press Q: Quit\n\n";
    
    
    
    
    //사다리(세로줄) 수 만큼 ladder segment를 저장함
    for (int i = 0; i < num_of_ladder; i++){
        //임시 변수 선언
        LadderSegment temp;
        temp.id_number = i;  //몇 번째 사다리인지
        //전체 리스트에 추가.
        vertical_ladder.push_back(temp);
    }
    //사다리 공간도 초기화해준다.
    for (int i = 0; i < num_of_ladder-1; i++){
        vector<bool> temp;
        for (int j = 0; j < num_of_step;j++){
            temp.push_back(false);
        }
        ladder.push_back(temp);
    }
    
    setting_flag = 1;
}



void ofApp::calculate_path(int index){
    //현재 선택된 path를 확인한다.
    cout << "\n\nSelected Path: "<<index<<"\n\n";
    //가장 먼저 시작되는 지점을 저장한다.
    int current_x = index;
    
    //한 칸씩 내려가면서 path에 좌표를 저장한다.
    for (int i = 0; i < num_of_step; i++){
        vertical_ladder[index].path.push_back(make_pair(current_x, i));
        
        //만일 좌/우에 가로줄이 있다면 저장하고 이동한다.
        if (current_x > 0 && ladder[current_x-1][i] == 1){
            current_x -= 1;
            vertical_ladder[index].path.push_back(make_pair(current_x, i));
        }
        
        else if (current_x < num_of_ladder-1 && ladder[current_x][i] == true){
            current_x++;
            vertical_ladder[index].path.push_back(make_pair(current_x, i));
        }
    }
        
    
//    for (int i = 0; i < vertical_ladder[index].path.size(); i++){
//        cout << vertical_ladder[index].path[i].first << ", " <<  vertical_ladder[index].path[i].second <<endl;
//    }
//    cout << "-- -- -- -- -- -- \n";
    
    //path 계산이 완료되었다는 것을 flag에 저장한다.
    draw_flag= 1;
}
