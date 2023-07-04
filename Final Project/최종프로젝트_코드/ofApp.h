#pragma once

#include "ofMain.h"

#define LEN 100
#define WID 50
#define MID 150

//사다리 참여자 하나 (하나의 사다리가 가게 되는 경로, 결과값, 사용자 이름을 저장
typedef struct LadderSegment{
    int id_number; //몇 번째 사다리를 선택했는지
    vector<pair<int, int>> path; //경로를 좌표로 저장한다.
    string result; //결과를 추가해서 저장할 수 있다. 본 코드에서는 사용하지 않았다.
} LadderSegment;



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void start(); // 처음 프로그램이 시작될 때 사용자수, 사다리의 길이, 결과값을 입력받는다.
    void makeLadder(); // 랜덤한 사다리를 그리는 함수
    void draw_ladder(); //만든 사다리를 그림
    void draw_path(int index); //사다리를 따라 내려가는 경로를 그림
    void calculate_path(int index); //사다리를 따라 내려가는 경로를 계산, 저장
    
    int num_of_ladder; // 몇 개의 사다리를 그릴 것인가 = 세로선이 몇 개인가?
    int num_of_step; //몇 칸을 이용할 것인가 = 세로선의 길이
    int currentIndex; //몇 번째 사다리를 선택할 것인가
    
    int setting_flag; //설정완료 여부를 확인하는 flag
    int ladder_set; //사다리가 그려졌는지 확인하는 flag
    int draw_flag; // 사다리를 그리는 flag
    int ladder_start_flag; //사다리 타기를 시작하는 Flag
    
    vector<vector<bool> > ladder; //사다리의 정보를 저장할 변수
    vector<LadderSegment> vertical_ladder; //매 세로선에 대한 정보를 저장할 변수
};
