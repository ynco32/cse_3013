#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));
    createRankList();

    
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
        case MENU_RANK: rank(); break;
        case MENU_RECOMMEND: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
    //6주차 과제 추가
    nextBlock[2] = rand() % 7;
    
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;
    
    //10주차
    recRoot = malloc(sizeof(RecNode));
    recRoot -> lv = 0;
    recRoot -> score = 0;
    for (int initRec = 0; initRec< HEIGHT; initRec++ ){
        for (int initRec2 = 0; initRec2 < WIDTH; initRec2++){
            recRoot -> f[initRec2][initRec] = 0;
        }
    }

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
    
    //8주차 과제 추가 + 수정
    DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15, WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10, 1, 8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17, WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
    
    //8주차 과제
    int k, l;
    for ( k = 0; k < 4; k++){
        move(10 + k, WIDTH+13);
        for ( l = 0; l < 4; l++){
            if( block[nextBlock[2]][0][k][l] == 1 ){
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            }
            else printw(" ");
        }
    }
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
    
    ///10주차
    recommend(recRoot);
    
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();
            //newRank(0);
			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    int i, j;
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            if (block[currentBlock][blockRotate][i][j]==1){
                
                int check_y = blockY + i;
                int check_x = blockX + j;
                
                //범위
                if (check_y > HEIGHT-1) return 0; // y
                if (check_x > WIDTH-1 || check_x < 0) return 0; //x
                
                //필드
                if(f[check_y][check_x]==1) return 0;
            }
        }
    }
    return 1;
    
    
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    //1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
    
    
    int user_x = blockX, user_y=blockY, user_rotate = blockRotate;
    int i,j;
    switch (command) {

        case KEY_UP:
            user_rotate--;
            if (user_rotate == -1) user_rotate = 3;
            break;
            
        case KEY_DOWN:
            user_y--;
            break;
            
        case KEY_LEFT:
            user_x++;
            break;
            
        case KEY_RIGHT:
            user_x--;
            break;
            
        default:
            return;
            break;
            
            
    }
    
    
    //2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            
            if(block[currentBlock][user_rotate][i][j] == 1){
                
                int xed = user_x + j + 1;
                int yed = user_y + i + 1;
                if (yed > 0){
                    move(yed, xed);
                    printw(".");
                }
                
            }
        }
    }
    
    
    int shadow = 0;
    while (CheckToMove(field, currentBlock, user_rotate, user_y + shadow, user_x)==1){
        shadow++;
    }
    shadow = user_y + shadow - 1;
    
    
    int k, l;
    for (k = 0; k < 4; k++){
        for (l = 0; l < 4; l++){
            
            if(block[currentBlock][user_rotate][k][l] == 1){
                
                int xed = user_x + l + 1;
                int yed = k + shadow + 1;
                
                move(yed, xed);
                printw(".");
                
                
            }
        }
    }
    
    //3. 새로운 블록 정보를 그린다.
    DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
    move(2*HEIGHT,2*WIDTH);
    return;
    
	
}

void BlockDown(int sig){
	// user code
	//강의자료 p26-27의 플로우차트를 참고한다.
    
    if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)){
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY,  blockX);
    }
    
    else{
        if (blockY == -1) gameOver = 1;
        
        //AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX); //블록 필드로 합치기
        
        //8주차 과제:: 두번째 블록 아이디 생성 부분 추가
        //스코어 추가
        score = score + AddBlockToField(field,nextBlock[0],blockRotate, blockY, blockX);
        score = score + DeleteLine(field);
        
        
        //블록 하나씩 당기기
        nextBlock[0] = nextBlock[1];
        nextBlock[1] = nextBlock[2];
        nextBlock[2] = rand() % 7;
        
        //초기화
        blockRotate = 0;
        blockY = -1;
        blockX = WIDTH/2 - 2;
        
        
        ///10주차 recommend 재설정
        for(int initRec = 0; initRec< HEIGHT; initRec++ ){
            for(int initRec2 = 0; initRec2 < WIDTH; initRec2++){
                recRoot -> f[initRec][initRec2] = field[initRec][initRec2];
            }
        }
        
        //recommendY = 0;
        recommend(recRoot);
        DrawNextBlock(nextBlock);
        //출력
        PrintScore(score);
        DrawField();
    }
    
    timed_out=0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
    //Block이 추가된 영역의 필드값을 바꾼다.
    int touched = 0;
    int i, j;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            
            int yed = blockY + i;
            int xed = blockX + j;
            
            if(block[currentBlock][blockRotate][i][j]==1){
                f[yed][xed] = 1;
                if (f[yed+1][xed] || yed+1 == HEIGHT) touched++;
            }
        }
    }
    
    touched = touched * 10;

    return touched;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
    int cnt = 0;
    int i, j;
    for(i = 0; i < HEIGHT; i++){
        bool isFull = true;
        for(j = 0; j < WIDTH; j++){
            
            if(f[i][j] != 1){
                isFull = false; //하나라도 비어있으면
                break;
            }
        }
        
        //2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
        if(isFull){
            cnt++;
            for(int k = i; k > 0; k--){
                for(int l = 0; l < WIDTH; l++){
                    field[k][l] = field[k-1][l];
                }
            }
            
        }
    }
    
    int result = cnt * cnt * 100;
    
    return result;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
    int newY = 0;
    while (CheckToMove(field, blockID, blockRotate, y + newY, x)) {
        newY++;
    }
    newY= y + newY-1;
    //newY = newY + y -1;
    
    DrawBlock(newY, x, blockID, blockRotate,'/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
    DrawBlock(y, x, blockID, blockRotate,' ');
    DrawShadow(y, x, blockID, blockRotate);
    DrawRecommend(recommendY, recommendX, blockID, recommendR);
}


///////////////////////////////////////////////////////////////////////////

void createRankList(){
	// user code
    // 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
    // 1. "rank.txt"열기
    // 2. 파일에서 랭킹정보 읽어오기
    // 3. LinkedList로 저장
    // 4. 파일 닫기
    FILE *fp;
    int i, j;
    
    head = (Node *)malloc(sizeof(Node));
    head -> next = NULL;
    Node * current = head;
    

    //1. 파일 열기
    fp = fopen("rank.txt", "r");
    if (fp == NULL) {
        fp = fopen("rank.txt", "w");
        return;
    }

    // 2. 정보읽어오기
    /* int fscanf(FILE* stream, const char* format, ...);
    stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
    format: 형식지정자 등등
    변수의 주소: 포인터
    return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
    // EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
    if (fscanf(fp, "%d", &score_number) != EOF) {
        int cnt = score_number;
        for (int i = 0; i < cnt; i++){
            Node * new;
            new = (Node *)malloc(sizeof(Node));
            
            int score_now;
            fscanf(fp, "%s", new->name);
            fscanf(fp, "%d", &score_now);
            
            new -> score = score_now;
            new -> next = new;
            
            current -> next = new;
            current = current -> next;
        }
    }
    else {
        return;
    }
    // 4. 파일닫기
    fclose(fp);
}


void rank(){
	// user code
    //목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
    //1. 문자열 초기화
    int X=1, Y=score_number, ch, i, j;
    clear();

    //2. printw()로 3개의 메뉴출력
    printw("1. list ranks from x to y\n");
    printw("2. list ranks by a specific name\n");
    printw("3. delete a specific rank \n");
    
    //3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
    ch = wgetch(stdscr);
    
    //4. 각 메뉴에 따라 입력받을 값을 변수에 저장
    //4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
    if (ch == '1') {
        printw("X: ");
        echo();
        scanw("%d", &X);
        printw("Y: ");
        scanw("%d", &Y);
        noecho();
        
        
        printw("       name       |   score     \n");
        printw("--------------------------------\n");
        

        if (X > Y) printw("search failure: no rank in the list\n");
        else if (X < 1) printw("search failure: no rank in the list\n");
        else if (Y > score_number) printw("search failure: no rank in the list\n");
        
        else{
            
            Node* current;
            current = head;
            
            for(i = 0; i < X; i++){
                current = current->next;
            }
            
            for(i=0; i <= (Y-X); i++){
                printw(" %-17s| %-10d\n",current->name,current->score);
                current = current->next;
            }
        }
       
    }

    //4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
    else if ( ch == '2') {
        char str[NAMELEN+1];
        int check = 0;
        //int i;
        printw("input the name: ");
        echo();
        scanw("%s", str);
        noecho();


        printw("       name       |   score     \n");
        printw("--------------------------------\n");

        Node * current;
        current = head;

        for (i = 0; i < score_number; i++){
            current = current -> next;
            bool cmp = strcmp(str,current->name);
            if (!cmp){
                printw(" %-17s| %-10d\n", current->name, current->score);
                check = 1;
            }
            
        }
        if (check == 0){
            printw("\nsearch failure: no name in the list\n");
        }

    }

    //4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
    else if ( ch == '3') {
        int num;
        //int i;

        printw("input the rank: ");
        echo();
        scanw("%d", &num);
        noecho();

        if (num < 1 || num > score_number) {
            printw("search failure: the rank not in the list\n");
        }

        else if (num == 1) {
            Node * current;
            current = head;
            head = head -> next;
            score_number--;
            free(current);
        }

        else {
            Node * current;
            Node * delete;
            current = head;

            for (j = 1; j < num; j++){
                current = current -> next;
            }
            
            delete = current -> next;
            current -> next = current -> next -> next;
            score_number--;
            free(delete);
            //printw("\n");
            printw("\nresult: the rank deleted\n");
        }
        writeRankFile();
    }
    getch();
}

void writeRankFile(){
	// user code
    // 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
    int sn, i;
    //1. "rank.txt" 연다
    FILE *fp = fopen("rank.txt", "w");
    
    //2. 랭킹 정보들의 수를 "rank.txt"에 기록
    fprintf(fp, "%d\n", score_number);
    
    
    Node * current = head;
    sn = 0;

    //3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
    
//    if (score_number == 1){
//        fprintf(fp, "%s %d\n", head->name, head -> score);
//    }
//
////    else if (score_number == 0){
////f
////    }
//
//    else {
//        if ( sn == score_number) {
//            //fprintf(fp, "%s %d\n", current->name, current -> score);
//            return;
//        }
//        current = current -> next;
//        fprintf(fp, "%s %d\n", current->name, current -> score);
//        sn++;
//        //fprintf(fp, "%s %d\n", current->name, current -> score);
//
//    }
    
    for (int i = 0; i < score_number; i++){
            current = current->next;
            fprintf(fp, "%s %d\n", current->name, current->score);
    }

//    for ( i= 1; i < score_number+1 ; i++) {
//        free(current.rank_name[i]);
//    }
//    free(current.rank_name);
//    free(current.rank_score);

    

    fclose(fp);

}

void newRank(int score){
	// user code
    // 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
    char str[NAMELEN+1];
    int i, j;
    clear();
    
   
    Node* new = (Node*)malloc(sizeof(Node));
    new->next = NULL;
    
    //1. 사용자 이름을 입력받음
    printw("your name: ");
    echo();
    scanw("%s", new->name);
    noecho();
    
    new -> score = score;
    score_number++;

    //2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
    if(head != NULL) {
        Node* current = head;
        while( current -> next != NULL){
            Node * temp = current -> next;
            if(temp -> score > score) current = current->next;
            else break;
        }

        if(head -> next == NULL){
            if(head->score < score){
                head = new;
                new->next=current;
            }
            else {
                current -> next = new;
            }
        }
        else
        {
            Node* nextNode = current->next;
            current -> next = new;
            new -> next = nextNode;
        }
    }
    
    else {
        head = new;
    }
    writeRankFile();
}

//////10주차////
void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
    DrawBlock(y, x, blockID, blockRotate,'R');
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
    
	// user code
    int rec=0;
    int l, h, r, posX, posY;
    int cindex = 0;
    l = root -> lv;
    int bl = nextBlock[l];
    
    for (r = 0; r < 4; r++){

        for (posX = 0 ; posX < WIDTH; posX++){
            
            
            //posX = i;
            posY = 0;
            
            if (CheckToMove(root -> f, bl, r, posY, posX)){
                
                //
                RecNode * new = (RecNode*)malloc(sizeof(RecNode));
                new -> lv = l+1;
                new -> score = 0;
                
                
                
                for(int initRec = 0; initRec< HEIGHT; initRec++ ){
                    for(int initRec2 = 0; initRec2 < WIDTH; initRec2++){
                        new -> f[initRec][initRec2] = root -> f[initRec][initRec2];
                    }
                }
                //
                
            
                while (CheckToMove(new->f, bl, r, posY, posX)) {
                    posY++;
                }
                posY--;
                
//                while (CheckToMove(new -> f, bl, r, posY+1, posX)) {
//                    posY++;
//                }
//                //posY--;
                
                //int temp =
                
                new -> score += AddBlockToField(new -> f, bl, r, posY, posX);
                new -> score += DeleteLine(new->f);
                rec = new -> score;
                root->c[cindex++] = new;
                
                
                if (l < BLOCK_NUM){
                    rec += recommend(new);
                    
                    if (max < rec){
                        max = rec;
                        if (l == 0){
                            recommendR = r;
                            recommendX = posX;
                            recommendY = posY;
                        }
                    }
                }
            }
            
        }
    }
    return max;
}

void recommendedPlay(){
	// user code
}
