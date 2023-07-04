/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include <deque>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(5);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
    isbfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here

	int w = 30; 
	int w_W = WALL_LENGTH * WIDTH;
	int w_H = WALL_LENGTH * HEIGHT;
	ofSetLineWidth(LINE_WIDTH);

	if (isOpen == 1) {
	//basic
		ofDrawLine(0, 0, w_W, 0);
		ofDrawLine(0, w_H, w_W, w_H);
		ofDrawLine(0, 0, 0, w_H);
		ofDrawLine(w_W, 0, w_W, w_H);

		

		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (wall_C[i][j] == true) {
					ofDrawLine(j * WALL_LENGTH, (i + 1) * WALL_LENGTH, (j + 1) * WALL_LENGTH, (i + 1) * WALL_LENGTH);
					
				}
			}
		}


		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (wall_R[i][j] == true) {
					ofDrawLine((j + 1) * WALL_LENGTH, i * WALL_LENGTH, (j + 1) * WALL_LENGTH, (i + 1) * WALL_LENGTH);
					
				}
			}
		}

	}

	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			dfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}
    if (isbfs)
    {
        ofSetColor(200);
        ofSetLineWidth(5);
        if (isOpen) {
            bfsdraw();
        }
        else
            cout << "You must open file first" << endl;
    }
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;


			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			int total_y = buffer.getLines().begin().asString().size();
			int total_x = buffer.size();
			total_x = total_x / (total_y + 1);

			int c_idx;

			input = new char*[total_x + 1];
			c_idx = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				input[c_idx] = new char[total_y + 1];
				strcpy(input[c_idx], line.c_str());
				c_idx++;
			}


			WIDTH = total_y / 2;
			HEIGHT = total_x / 2;
			wall_R.resize(HEIGHT, vector<bool>(WIDTH, 0));
			wall_C.resize(HEIGHT, vector<bool>(WIDTH, 0));

			int i, j;

			
			for (i = 1; i < total_x - 1; i++) {
				if (input_flag == 1) {
					for (j = 1; j < total_y - 1; j += 2) {
						int index1 = (i - 1) / 2;
						int index2 = j / 2;

						if (input[i][j] == '-') {
							wall_C[index1][index2] = true;
						}
						else {
							wall_C[index1][index2] = false;
						}
					}

					input_flag = 0;
					
				}

				else {
					for (j = 2; j < total_y - 1; j += 2) {
						int index1 = i / 2;
						int index2 = (j - 1) / 2;

						if (input[i][j] == '|') {
							wall_R[index1][index2] = true;
						}
						else {
							wall_R[index1][index2] = false;
						}
					}
					input_flag = 1;
				}
			}
			
			/*
			for (i = 0; i < HEIGHT; i++) {
				if (input_flag == 0) {
					for (j = 0; j < WIDTH; j++) {
						int index1 = i * 2;
						int index2 = j+2 +1;

						if (input[index1][index2] == '|') {
							wall_R[i][j] = true;
						}
						else {
							wall_R[i][j] = true;
						}
					}
					input_flag = 1;
				}

				else {
					for (j = 0; j < WIDTH; j++) {
						int index1 = (i - 1) / 2;
						int index2 = j / 2;

						if (input[index1][index2] == '-') {
							wall_R[i][j] = true;
						}
						else {
							wall_R[i][j] = true;
						}
					}

					input_flag = 0;
				}
			}
			*/

			printf("--------------- input maze ----------------\n");
			for (i = 0; i < total_x; i++) {
				for (j = 0; j < total_y; j++) {
					//printf("%c", input[i][j]);
				}
				printf("\n");
			}
			printf("-------------------------------------\n");

			printf("--------------- wall_C ----------------\n");
			for (i = 0; i < HEIGHT; i++) {
				for (j = 0; j < WIDTH; j++) {
					if (wall_C[i][j] == true) {
						printf("0");
					}
					else printf("1");
				}
				printf("\n");
			}
			printf("-------------------------------------\n");

			printf("--------------- wall_R ----------------\n");
			for (i = 0; i < HEIGHT; i++) {
				for (j = 0; j < WIDTH; j++) {
					if (wall_R[i][j] == true) {
						printf("0");
					}
					else printf("1");
				}
				printf("\n");
			}
			printf("-------------------------------------\n");
		}


		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
		return true;
}


void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수

	for (int i = 0; i < WIDTH; i++) {
		wall_R[i].clear();
		wall_C[i].clear();
		is_visited[i].clear();

	}

	wall_R.clear();
	wall_C.clear();
	is_visited.clear();



}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)

	

	is_visited.resize(HEIGHT, vector<int>(WIDTH, 0));
	
	//x, y 
	deque <pair<int, int>> s;
	
	is_visited[0][0] = 1;
	
	//for (int i = 0; i < )

	
	s.push_back({ 0,0 });
	s_R.push_back({ 0,0 });
	pair<int, int> top;
	pair<int, int> top_R;
	cout << "-----------\n";
	while (!s.empty()) {
		top = s.back();
		s.pop_back();
		
		cout << top.first << ", "  << top.second << endl;

		top_R = s_R.back();
		//s_R.pop_back();

		int top_x = top.first;
		int top_y = top.second;

		for (int i = 0; i < 4; i++) {
			int now_x = top.first + dir_x[i];
			int now_y = top.second + dir_y[i];
			
			if (now_x == WIDTH - 1 && now_y == HEIGHT - 1) {
				is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
				break;
			}

			if (now_x >= 0 && now_y >= 0) {
				if (now_x < HEIGHT && now_y < WIDTH) {
					if (is_visited[now_x][now_y] == 0) {

						if (i == 0 && wall_R[top_x][top_y] == false) {
							//아래
							is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
							s.push_back({ top_x, top_y });
							//s_R.push_back({ top_x, top_y });
							s.push_back({ now_x, now_y });
							s_R.push_back({ now_x, now_y });
							break;

						}

						else if (i == 1 && wall_C[top_x][top_y] == false) {
							//오른쪽
							is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
							s.push_back({ top_x, top_y });
							//s_R.push_back({ top_x, top_y });
							s.push_back({ now_x, now_y });
							s_R.push_back({ now_x, now_y });
							break;
						}

						else if (i == 2 && wall_C[now_x][now_y] == false) {
							// 왼쪽
							is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
							s.push_back({ top_x, top_y });
							//s_R.push_back({ top_x, top_y });
							s.push_back({ now_x, now_y });
							s_R.push_back({ now_x, now_y });
							break;
						}

						else if (i == 3 && wall_R[now_x][now_y] == false) {
							// 위
							is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
							s.push_back({ top_x, top_y });
							//s_R.push_back({ top_x, top_y });
							s.push_back({ now_x, now_y });
							s_R.push_back({ now_x, now_y });
							break;
						}
						
						
						

					}
				
					
				} 

			}

		}
	}

	cout << "-----------\n";
	isdfs = true;

	printf("--------------- is_visited ----------------\n");
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			cout << is_visited[i][j];
		}
		printf("\n");
	}
	printf("-------------------------------------\n");

	printf("--------------- S ----------------\n");
	for (int i = 0; i < s.size(); i++) {
		cout << "(" << s[i].first << "," << s[i].second << ")";
		printf("\n");
	}
	printf("-------------------------------------\n");


	printf("--------------- S_R ----------------\n");
	for (int i = 0; i < s_R.size(); i++) {
		cout << "(" << s_R[i].first << "," << s_R[i].second << ")";
		printf("\n");
	}
	printf("-------------------------------------\n");

	return true;
}
void ofApp::dfsdraw()
{



	//printf("--------------- dfsdraw ----------------\n");
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	/*
	ofSetColor(0, 0, 255);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH-1; j++) {
			if (is_visited[i][j] > 0 ) {
				if (is_visited[i][j + 1] > 0) {
					if (wall_C[i][j] == false) {
						float x1 = i;
						float x2 = i + 1;
						float y1 = j;
						float y2 = j;
						ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
					}
				}
				

			}
		}
	}

	for (int i = 0; i < HEIGHT-1; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (is_visited[i][j] > 0) {
				if (is_visited[i + 1][j] > 0) {
					if (wall_R[i][j] == false) {
						float x1 = i;
						float x2 = i;
						float y1 = j;
						float y2 = j + 1;
						ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
						//ofDrawLine(j * ROAD_LENGTH, i * ROAD_LENGTH, (j + 1) * ROAD_LENGTH, i * ROAD_LENGTH);
					}
				}
			}
		}
		
	}
	*/

	ofSetColor(0, 0, 255);

	ofSetColor(0, 0, 255);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH-1; j++) {
			if (i != HEIGHT - 1) {
				if (is_visited[i][j] > 0) {
					if (is_visited[i][j + 1] > 0) {
						if (wall_C[i][j] == false) {
							int x1 = i;
							int x2 = i + 1;
							int y1 = j;
							int y2 = j;
							ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
						}
					}
				}
			}
			else {
				if (is_visited[i][j] > 0) {
					if (is_visited[i][j + 1] > 0) {
						if (wall_R[i][j] == false) {
								int x1 = i;
								int x2 = i;
								int y1 = j;
								int y2 = j + 1;
								ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < HEIGHT-1; i++) {
		for (int j = 0; j < WIDTH; j++) {

			if (j != WIDTH - 1) {
				if (is_visited[i][j] > 0) {
					if (is_visited[i + 1][j] > 0) {
						if (wall_R[i][j] == false) {
							int x1 = i;
							int x2 = i;
							int y1 = j;
							int y2 = j + 1;
							ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
							//ofDrawLine(j * ROAD_LENGTH, i * ROAD_LENGTH, (j + 1) * ROAD_LENGTH, i * ROAD_LENGTH);
						}
					}
				}
			}
			else {

				if (is_visited[i][j] > 0) {
					if (is_visited[i + 1][j] > 0) {
						if (wall_C[i][j] == false) {
							int x1 = i;
							int x2 = i+1;
							int y1 = j;
							int y2 = j;
							ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
							//ofDrawLine(j * ROAD_LENGTH, i * ROAD_LENGTH, (j + 1) * ROAD_LENGTH, i * ROAD_LENGTH);
						}
					}
				}
			}
		}

	}
	
	


	int idx_x = HEIGHT-1;
	int idx_y = WIDTH-1;

	bool draw_flag = false;
	 //draw_flag1 = false;

	while (1) {
		if (idx_x != 0 || idx_y != 0) {
			for (int i = 0; i < 4; i++) {
				int now_x = idx_x + dir_x[i];
				int now_y = idx_y + dir_y[i];

				if (now_x >= 0 && now_y >= 0) {
					if (now_x < HEIGHT && now_y < WIDTH) {
						if (is_visited[now_x][now_y] == is_visited[idx_x][idx_y] - 1) {

							if (i == 0 && wall_R[idx_x][idx_y] == false) {
								//아래
								draw_flag = true;
								//draw_flag1 = true;

							}

							else if (i == 1 && wall_C[idx_x][idx_y] == false) {
								//오른쪽
								draw_flag = true;
								//draw_flag1 = true;
							}

							else if (i == 2 && wall_C[now_x][now_y] == false) {
								// 왼쪽
								draw_flag = true;
								//draw_flag1 = true;
							}

							else if (i == 3 && wall_R[now_x][now_y] == false) {
								// 위
								draw_flag = true;
								//draw_flag1 = true;
							}



							if (draw_flag) {
								//cout << "now: " << now_x << now_y << "\n";
								ofSetColor(255, 0, 0);
								ofDrawLine((now_y + 0.5) * WALL_LENGTH, (now_x + 0.5) * WALL_LENGTH, (idx_y + 0.5) * WALL_LENGTH, (idx_x + 0.5) * WALL_LENGTH);
								idx_y = now_y;
								idx_x = now_x;
								draw_flag = false;
								break;
							}



						}

						


					}
				}
			}

		}
		else {
			break;
		}

	}



}


bool ofApp::BFS()//DFS탐색을 하는 함수
{
    //TO DO
    //DFS탐색을 하는 함수 ( 3주차)

    

    is_visited.resize(HEIGHT, vector<int>(WIDTH, 0));
    
    //x, y
    deque <pair<int, int>> q;
    
    is_visited[0][0] = 1;
    
    //for (int i = 0; i < )

    
    q.push_back({ 0,0 });
    
    pair<int, int> top;
    pair<int, int> top_R;
    cout << "-----------\n";
    while (!q.empty()) {
        top = q.front();
        q.pop_front();
        
        cout << top.first << ", "  << top.second << endl;


        int top_x = top.first;
        int top_y = top.second;

        for (int i = 0; i < 4; i++) {
            int now_x = top.first + dir_x[i];
            int now_y = top.second + dir_y[i];
            
            if (now_x == WIDTH - 1 && now_y == HEIGHT - 1) {
                is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
                break;
            }

            if (now_x >= 0 && now_y >= 0) {
                if (now_x < HEIGHT && now_y < WIDTH) {
                    if (is_visited[now_x][now_y] == 0) {

                        if (i == 0 && wall_R[top_x][top_y] == false) {
                            //아래
                            is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
                            q.push_back({ now_x, now_y });
                            
                            break;

                        }

                        else if (i == 1 && wall_C[top_x][top_y] == false) {
                            //오른�
                            is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
                            
                           
                            q.push_back({ now_x, now_y });
                            
                            break;
                        }

                        else if (i == 2 && wall_C[now_x][now_y] == false) {
                            // 왼�
                            is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
                            
                            
                            q.push_back({ now_x, now_y });
                            
                            break;
                        }

                        else if (i == 3 && wall_R[now_x][now_y] == false) {
                            // 위
                            is_visited[now_x][now_y] = is_visited[top_x][top_y] + 1;
                           
                           
                            q.push_back({ now_x, now_y });
                           
                            break;
                        }
                        
                        
                        

                    }
                
                    
                }

            }

        }
    }

    cout << "-----------\n";
    isbfs = true;

    printf("--------------- is_visited ----------------\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << is_visited[i][j];
        }
        printf("\n");
    }
    printf("-------------------------------------\n");

    printf("--------------- Q ----------------\n");
    for (int i = 0; i < q.size(); i++) {
        cout << "(" << q[i].first << "," << q[i].second << ")";
        printf("\n");
    }
    printf("-------------------------------------\n");

    return true;
}
void ofApp::bfsdraw()
{




    ofSetColor(0, 0, 255);

    ofSetColor(0, 0, 255);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH-1; j++) {
            if (i != HEIGHT - 1) {
                if (is_visited[i][j] > 0) {
                    if (is_visited[i][j + 1] > 0) {
                        if (wall_C[i][j] == false) {
                            int x1 = i;
                            int x2 = i + 1;
                            int y1 = j;
                            int y2 = j;
                            ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
                        }
                    }
                }
            }
            else {
                if (is_visited[i][j] > 0) {
                    if (is_visited[i][j + 1] > 0) {
                        if (wall_R[i][j] == false) {
                                int x1 = i;
                                int x2 = i;
                                int y1 = j;
                                int y2 = j + 1;
                                ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < HEIGHT-1; i++) {
        for (int j = 0; j < WIDTH; j++) {

            if (j != WIDTH - 1) {
                if (is_visited[i][j] > 0) {
                    if (is_visited[i + 1][j] > 0) {
                        if (wall_R[i][j] == false) {
                            int x1 = i;
                            int x2 = i;
                            int y1 = j;
                            int y2 = j + 1;
                            ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
                            //ofDrawLine(j * ROAD_LENGTH, i * ROAD_LENGTH, (j + 1) * ROAD_LENGTH, i * ROAD_LENGTH);
                        }
                    }
                }
            }
            else {

                if (is_visited[i][j] > 0) {
                    if (is_visited[i + 1][j] > 0) {
                        if (wall_C[i][j] == false) {
                            int x1 = i;
                            int x2 = i+1;
                            int y1 = j;
                            int y2 = j;
                            ofDrawLine((y1 + 0.5) * ROAD_LENGTH, (x1 + 0.5) * ROAD_LENGTH, (y2 + 0.5) * ROAD_LENGTH, (x2 + 0.5) * ROAD_LENGTH);
                            //ofDrawLine(j * ROAD_LENGTH, i * ROAD_LENGTH, (j + 1) * ROAD_LENGTH, i * ROAD_LENGTH);
                        }
                    }
                }
            }
        }

    }
    
    


    int idx_x = HEIGHT-1;
    int idx_y = WIDTH-1;

    bool draw_flag = false;
     //draw_flag1 = false;

    while (1) {
        if (idx_x != 0 || idx_y != 0) {
            for (int i = 0; i < 4; i++) {
                int now_x = idx_x + dir_x[i];
                int now_y = idx_y + dir_y[i];

                if (now_x >= 0 && now_y >= 0) {
                    if (now_x < HEIGHT && now_y < WIDTH) {
                        if (is_visited[now_x][now_y] == is_visited[idx_x][idx_y] - 1) {

                            if (i == 0 && wall_R[idx_x][idx_y] == false) {
                                //아래
                                draw_flag = true;
                                //draw_flag1 = true;

                            }

                            else if (i == 1 && wall_C[idx_x][idx_y] == false) {
                                //오른�
                                draw_flag = true;
                                //draw_flag1 = true;
                            }

                            else if (i == 2 && wall_C[now_x][now_y] == false) {
                                // 왼�
                                draw_flag = true;
                                //draw_flag1 = true;
                            }

                            else if (i == 3 && wall_R[now_x][now_y] == false) {
                                // 위
                                draw_flag = true;
                                //draw_flag1 = true;
                            }



                            if (draw_flag) {
                                //cout << "now: " << now_x << now_y << "\n";
                                ofSetColor(255, 0, 0);
                                ofDrawLine((now_y + 0.5) * WALL_LENGTH, (now_x + 0.5) * WALL_LENGTH, (idx_y + 0.5) * WALL_LENGTH, (idx_x + 0.5) * WALL_LENGTH);
                                idx_y = now_y;
                                idx_x = now_x;
                                draw_flag = false;
                                break;
                            }



                        }

                        


                    }
                }
            }

        }
        else {
            break;
        }

    }



}

