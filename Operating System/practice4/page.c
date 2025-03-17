#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define EBIT 262144 //18bit
#define NBIT 524288 //19bit
#define TBIT 1048576 //20bit
#define ONEKB 1024 //1KB
#define TWOKB 2048 // 2KB
#define FOUREKB 4096 //4KB
#define ADDRESSC 5000 //가상주소 개수 


int vlength;  //가상주소의 길이
int psize; //페이지 크기
int pnum; //페이지 개수
int fnum; //프레임 개수



int isValidNumber(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

void getNumericInput(int *num) {
    char input[100];

    while (1) {
        scanf("%s", input);
        if (isValidNumber(input)) {
            *num = atoi(input);
            break;
        } else {
            printf("올바른 숫자가 아닙니다. 다시 입력하세요: ");
        }
    }
}


void createFile(){
    srand(time(NULL)); // 난수 생성기 시드 초기화
    
    FILE *file = fopen("input.in", "w"); //파일 열기 (덮어쓰기 모드)

    if(file == NULL){
        perror("파일 열기 실패");
    }

    for(int i=0; i<ADDRESSC; i++){
        int virtualAdd = rand() % vlength;

        fprintf(file, "%d\n", virtualAdd);
    }

    fclose(file);
}

void opt(FILE *inputFile){

    FILE *outFile = fopen("output.opt", "w");

    if(outFile == NULL){
        perror("파일 열기 실패");
    }

    fprintf(outFile, "%-15s %-15s %-15s %-15s %-15s %-15s\n", "No.", "V.A" ,"Page No.", "Frame No.", "P.A.", "Page Fault");
    
    int virtualAdd;
    int addressCount = 0;
    int frameTable[fnum]; // 자료구조
    int refString[ADDRESSC]; //reference string
    int offset[ADDRESSC];
    int frameNum[ADDRESSC]; //할당된 frame 번호를 저장할 공간 - virtual 주소 하나마다 있는거
    char isFault[ADDRESSC]; //페이지 fault가 일어 났는지 저장하는 공간
    int physicalAdd[ADDRESSC]; //물리 메모리 주소를 저장해둘 공간
    int faultcount =0; //fault가 몇번 일어났는지
    int frameAllocationOrder[fnum]; //동일한 우선순위에서 fifo방식구현을 위한 배열
    memset(frameAllocationOrder, 0, sizeof(frameAllocationOrder));
    int allocationCounter = 1;


    while(fscanf(inputFile, "%d", &virtualAdd) == 1){
        refString[addressCount] = virtualAdd/psize;
        offset[addressCount] = virtualAdd%psize;
        addressCount++;
    }

    //frameTable의 원소 값이 -1이면 해당 인덱스는 참조가 안된 frmae number이다. 
    memset(frameTable, -1, sizeof(frameTable));

    for(int i=0; i<ADDRESSC; i++){

        int isHit = 0;
        //비어있는 frame이 있는가를 나타냄 
        int is_pg_full = 1;
        
        //페이지 프레임을 찾는다. 
        for(int j =0; j<fnum; j++){

            //page framse 에서  page reference 찾기
            if(refString[i] == frameTable[j]){

                isHit = 1;
                isFault[i] = 'H';
                frameNum[i] = j;
                physicalAdd[i] = j*psize+offset[i];

                break;
            }

        }

        //hit가 안된경우
        if(isHit == 0){
            isFault[i] = 'F';
            faultcount++;
            
            //빈 frame을 찾아야한다. 
            for(int j =0; j<fnum; j++){
            
                if(frameTable[j]==-1){
                    frameTable[j] = refString[i];
                    frameAllocationOrder[j] = allocationCounter++;
                    is_pg_full = 0;
                    frameNum[i] = j;
                    physicalAdd[i] = j*psize+offset[i];
                    break;
                }
                
            }   

            //꽉차있으니까 page replacement 수행
            if(is_pg_full){

                //가장 오래 참조되지 않을 인덱스를 가리키는 변수
                int page_idx = i;
                int frame_idx;
                int NoReferFrame_idx = -2;
                int foundUnrefer = 0; //참조가 되지 않은 페이지가 있는지 여부
                int oldestFrameOrder = 5001; //5000개로 테스트하기 때문에 가장 늦은 순서가 5000을 넘길 수 없음
                

                for(int j =0; j<fnum; j++){
                    
                    //reference string에 해당 page number가 없는 경우를 나타내는 변수
                    int isUsed = 0;

                    for(int k =i+1; k<ADDRESSC; k++){
                        if(refString[k] == frameTable[j]){
                            if(k > page_idx){
                                
                                //교체 대상이 되는 frame number update
                                frame_idx = j;
                                page_idx = k;
                            }
                            isUsed = 1;
                            break;
                        }
                        

                    }
                    //reference string에 해당 page number가 없는 경우
                    if(!isUsed && frameAllocationOrder[j] < oldestFrameOrder){
                        oldestFrameOrder = frameAllocationOrder[j];
                        NoReferFrame_idx = j;
                        foundUnrefer = 1;
                    }

                }

                //모두 참조가 된경우
                if(!foundUnrefer){
                    //페이지 교체 
                    frameTable[frame_idx] = refString[i];
                    frameNum[i] = frame_idx;
                    physicalAdd[i] = frame_idx*psize+offset[i];
                }else{
                    //하나라도 reference String에서 참조가 안된경우
                    //페이지 교체 
                    frameTable[NoReferFrame_idx] = refString[i];
                    frameNum[i] = NoReferFrame_idx;
                    physicalAdd[i] = NoReferFrame_idx*psize+offset[i];
                }

                

            }
        }

    
    
        fprintf(outFile, "%-15d %-15d %-15d %-15d %-15d %-15c\n", i+1, refString[i]*psize+offset[i], refString[i], frameNum[i], physicalAdd[i], isFault[i]);
    

    }

    fprintf(outFile, "Total Number of Page Faults: %d\n", faultcount);


}


void fifo(FILE *inputFile){
    FILE *outFile = fopen("output.fifo", "w");

    if(outFile == NULL){
        perror("파일 열기 실패");
    }

    fprintf(outFile, "%-15s %-15s %-15s %-15s %-15s %-15s\n", "No.", "V.A" ,"Page No.", "Frame No.", "P.A.", "Page Fault");
    
    int virtualAdd;
    int addressCount = 0;
    int frameTable[fnum]; // 자료구조
    int front = 0; //replacement될 위치를 가리키는 변수
    int rear = 0; //frame이 모두 찼는지를 확인하는 변수
    int refString[ADDRESSC]; //reference string
    int offset[ADDRESSC];
    int frameNum[ADDRESSC]; //할당된 frame 번호를 저장할 공간 - virtual 주소 하나마다 있는거
    char isFault[ADDRESSC]; //페이지 fault가 일어 났는지 저장하는 공간
    int physicalAdd[ADDRESSC]; //물리 메모리 주소를 저장해둘 공간
    int faultcount =0; //fault가 몇번 일어났는지

    while(fscanf(inputFile, "%d", &virtualAdd) == 1){
        refString[addressCount] = virtualAdd/psize;
        offset[addressCount] = virtualAdd%psize;
        addressCount++;
    }


    for(int i=0; i<ADDRESSC; i++){
        int isHit = 0;
        //frame table이 꽉 차지 않았을 때
        if(rear < fnum){
            for(int j =0; j<rear; j++){
                if(refString[i] == frameTable[j]){
                    isFault[i] = 'H';
                    isHit = 1;
                    frameNum[i] = j;
                    physicalAdd[i] = j*psize+offset[i];
                }
            }
            if(isHit == 0){
                faultcount++;
                isFault[i] = 'F';
                frameNum[i] = rear;
                physicalAdd[i] = rear*psize+offset[i];
                frameTable[rear] = refString[i];
                rear ++;

            }
        //framtable이 꽉 찬경우
        }else{
            for(int j =0; j<rear; j++){
                //hit인 경우
                //framTable을 바꿔줄 필요가 없다. 
                if(refString[i] == frameTable[j]){
                    isFault[i] = 'H';
                    isHit = 1;
                    frameNum[i] = j;
                    physicalAdd[i] = j*psize+offset[i];
                }
            }
            // 다 돌았지만 fault가 난경우
            //framtable을 새로운 page number로 바꿔주어야 한다. 
            if(isHit==0){
                faultcount++;
                isFault[i] = 'F';
                frameNum[i] = front;
                physicalAdd[i] = front*psize+offset[i];
                frameTable[front] = refString[i];
                front = (front + 1) % fnum;
            }


        }

        isHit = 0;

        
        fprintf(outFile, "%-15d %-15d %-15d %-15d %-15d %-15c\n", i+1, refString[i]*psize+offset[i], refString[i], frameNum[i], physicalAdd[i], isFault[i]);
        
    }
    fprintf(outFile, "Total Number of Page Faults: %d\n", faultcount);

    fclose(outFile);

}


void lru(FILE *inputFile){

    
    FILE *outFile = fopen("output.lru", "w");

    if(outFile == NULL){
        perror("파일 열기 실패");
    }

    fprintf(outFile, "%-15s %-15s %-15s %-15s %-15s %-15s\n", "No.", "V.A","Page No.", "Frame No.", "P.A.", "Page Fault");
    
    int virtualAdd;
    int addressCount = 0;
    int frameTable[fnum]; // 자료구조
    int refString[ADDRESSC]; //reference string
    int offset[ADDRESSC];
    int frameNum[ADDRESSC]; //할당된 frame 번호를 저장할 공간 - virtual 주소 하나마다 있는거
    char isFault[ADDRESSC]; //페이지 fault가 일어 났는지 저장하는 공간
    int physicalAdd[ADDRESSC]; //물리 메모리 주소를 저장해둘 공간
    int faultcount =0; //fault가 몇번 일어났는지

    int ru_queue[fnum];
    int rear = -1;
    int pointer;

    memset(frameTable, -1, sizeof(frameTable));
    memset(ru_queue, 0, sizeof(ru_queue));

    while(fscanf(inputFile, "%d", &virtualAdd) == 1){

        refString[addressCount] = virtualAdd/psize;
        offset[addressCount] = virtualAdd%psize;
        addressCount++;
    }

    for(int i=0; i<ADDRESSC; i++){

        int isHit = 0;
        //비어있는 frame이 있는가를 나타냄 
        int is_pg_full = 1;
        
        //페이지 프레임을 찾는다. 
        for(int j =0; j<fnum; j++){

            //page framse 에서  page reference 찾기
            if(refString[i] == frameTable[j]){

                isHit = 1;
                isFault[i] = 'H';
                frameNum[i] = j;
                physicalAdd[i] = j*psize+offset[i];

                for(int k =0; k<=rear; k++){
                    if(ru_queue[k] == j){
                        pointer = k;
                        break;
                    }
                }
                

                for(int k = pointer; k< rear; k++){
                    //최근에 참조된 frame번호 이후의 frame 번호들을 앞으로 땡긴다.(더 오래됨을 나타냄) 
                    ru_queue[k] = ru_queue[k+1];
                }
                //가장 최근에 참조된 frame번호(j)는 기존에 있는 것을 바꾸고 마지막에 위치하게 된다. 
                ru_queue[rear] = j;
                break;
            }

        }

        //if page reference is not in page frames
        if(isHit == 0){
            isFault[i] = 'F';
            faultcount++;
            
            //빈 frame을 찾아야한다. 
            for(int j =0; j<fnum; j++){
            

                if(frameTable[j]==-1){
                    frameTable[j] = refString[i];
                    ru_queue[++rear] = j;
                    is_pg_full = 0;
                    frameNum[i] = j;
                    physicalAdd[i] = j*psize+offset[i];
                    break;
                }
                
            }   

            //꽉차있으니까 제일 참조 안된게 빠지고 땡기고 빠지게 된게 가장 최근에 참조 된 것이므로 가장 rear에 들어가야된다. 
            if(is_pg_full){
                
                //ru_queue 에 0번 인덱스에 있는게 가장 오래 참조가 안된 frame number로 교체 대상이된다. 
                pointer = ru_queue[0];

                //frame table에서 새로운 page number로 교체를 한다. 
                frameTable[pointer] = refString[i];
                frameNum[i] = pointer;
                physicalAdd[i] = pointer*psize+offset[i];

                for(int j =0; j<rear; j++){
                    ru_queue[j] = ru_queue[j+1];
                }
                //교체 대상이 된 frame number는 최근에 불려졌으니 마지막에 배치
                ru_queue[rear] = pointer;

            }
        }

        
        fprintf(outFile, "%-15d %-15d %-15d %-15d %-15d %-15c\n", i+1, refString[i]*psize+offset[i], refString[i], frameNum[i], physicalAdd[i], isFault[i]);
        
    }
    fprintf(outFile, "Total Number of Page Faults: %d\n", faultcount);

    fclose(outFile);

}


void sc(FILE *inputFile){
    
    FILE *outFile = fopen("output.sc", "w");

    if(outFile == NULL){
        perror("파일 열기 실패");
    }

    fprintf(outFile, "%-15s %-15s %-15s %-15s %-15s %-15s\n", "No.", "V.A","Page No.", "Frame No.", "P.A.", "Page Fault");
    
    int virtualAdd;
    int addressCount = 0;
    int frameTable[fnum]; // 자료구조
    int refString[ADDRESSC]; //reference string
    int offset[ADDRESSC];
    int frameNum[ADDRESSC]; //할당된 frame 번호를 저장할 공간 - virtual 주소 하나마다 있는거
    char isFault[ADDRESSC]; //페이지 fault가 일어 났는지 저장하는 공간
    int physicalAdd[ADDRESSC]; //물리 메모리 주소를 저장해둘 공간
    int faultcount =0; //fault가 몇번 일어났는지

    //각 frame number들이 참조가 되었는지를 표시해주는 배열
    int is_ref[fnum];
    //page replacement가 일어날 때 어디 부터 탐색을 할지를 가리키는 변수
    //가장 최근 replacement가 일어난 다음 인덱스부터 수행하면된다. 
    int victim_idx = 0;
    
    memset(is_ref, 0, sizeof(is_ref));
    memset(frameTable, -1, sizeof(frameTable));

    while(fscanf(inputFile, "%d", &virtualAdd) == 1){

        refString[addressCount] = virtualAdd/psize;
        offset[addressCount] = virtualAdd%psize;
        addressCount++;
    }
    
    for(int i=0; i<ADDRESSC; i++){

        int isHit = 0;
        //비어있는 frame이 있는가를 나타냄 
        int is_pg_full = 1;
        
        //페이지 프레임을 찾는다. 
        for(int j =0; j<fnum; j++){

            //page framse 에서  page reference 찾기
            if(refString[i] == frameTable[j]){

                isHit = 1;
                isFault[i] = 'H';
                frameNum[i] = j;
                //hit한 경우 해당 frame을 1로 update한다. 
                is_ref[j] = 1;
                physicalAdd[i] = j*psize+offset[i];

                break;
            }

        }

        //hit가 안된경우
        if(isHit == 0){
            isFault[i] = 'F';
            faultcount++;
            
            //빈 frame을 찾아야한다. 
            for(int j =0; j<fnum; j++){
            
                if(frameTable[j]==-1){
                    frameTable[j] = refString[i];
                    is_pg_full = 0;
                    frameNum[i] = j;
                    physicalAdd[i] = j*psize+offset[i];
                    break;
                }
                
            }   

            //꽉차있으니까 page replacement 수행
            if(is_pg_full){
                
                //ref bit가 0인 frameNumber를 만나면 1로 값이 바뀜 -> 페이지 교체가 일어났다는 뜻이다
                int isUpdate=0;

                //page replacement가 일어날때까지 while 문
                while(isUpdate == 0){
                    
                    for(int k =victim_idx; k<fnum; k++){
                        
                        //해당 ref bit가 1인경우 chance를 준다. 
                        if(is_ref[k] == 1){
                            //0으로 값 update
                            is_ref[k] = 0;
                        }else{
                            //ref bit가 0인 frame number로 교체 대상이다.
                            //교체 후에 ref bit는 그대로 0이기에 ref bit를 update하지 않는다. 
                            frameTable[k] = refString[i];
                            isUpdate = 1;
                            frameNum[i] = k;
                            physicalAdd[i] = k*psize+offset[i];
                            //교체 한 다음 index부터 다시 시작하면된다. 
                            victim_idx = k+1;
                            break;
                        }
                        
                    }  
                    //for 문을 돌고 victim_idx++한 값이 frame의 개수와 같다면 0으로 초기화(circular)
                    //victim_idx의 최대 값은 frame의 개수 -1이기 때문에 
                    if(victim_idx == fnum){
                        victim_idx = 0;
                    }else if(isUpdate==0){
                        //이 경우는 victim_idx가 한바퀴 다 돌았지만 0인것을 발견하지 못한경우
                        //처음부터 다시 순회해야하기 때문에 victim_idx를 0으로 초기화한다. 
                        victim_idx = 0;
                    }
                }                

            }
        }

        fprintf(outFile, "%-15d %-15d %-15d %-15d %-15d %-15c\n", i+1, refString[i]*psize+offset[i], refString[i], frameNum[i], physicalAdd[i], isFault[i]);
        
    }
    fprintf(outFile, "Total Number of Page Faults: %d\n", faultcount);

    fclose(outFile);


}


int main()
{
    int select1, select2, select3, select4, select5;
    while(1){
        printf("A. Simulation에 사용할 가상주소 길이를 선택하시오 (1. 18bits  2. 19bits 3. 20bits) : ");
        getNumericInput(&select1);
        
        if(select1 == 1){
            vlength = EBIT;
            break;
        } else if(select1 == 2){
            vlength = NBIT;
            break;
        } else if(select1 == 3) {
            vlength = TBIT;
            printf("%d\n", vlength);
            break;
        }
        else{
            printf("올바른 입력이 아닙니다. 다시 입력하세요 \n");
        }

    }

    while(1){
        printf("B. Simulation에 사용할 페이지(프레임)의 크기를 선택하시오 (1. 1KB  2. 2KB 3. 4KB) : ");
        getNumericInput(&select2);
        if(select2 == 1){
            psize = ONEKB;
            break;
        }else if (select2 == 2){
            psize = TWOKB;
            break;
        } else if( select2 == 3) {
            psize = FOUREKB;
            break;
        }
        else{
            printf("올바른 입력이 아닙니다. 다시 입력하세요 \n");
            sleep(1000);
        }

    }

    pnum = vlength / psize;

    while(1){
        
        printf("C. Simulation에 사용할 물리메모리의 크기를 선택하시오 (1. 32KB  2. 64KB ) : ");
        getNumericInput(&select3);
        if(select3 == 1 || select3 == 2 ) {
            if(select3 == 1){
                if(select2 == 3){
                    fnum = 32/4;
                }else
                    fnum = 32/select2;
            }
            else{
                 if(select2 == 3){
                    fnum = 64/4;
                }else
                    fnum = 64/select2;
            }
            break;
        }
        else{
            printf("올바른 입력이 아닙니다. 다시 입력하세요 \n");
        }

    }

    while(1){
        
        printf("D. Simulation에 적용할 Page Replacement 알고리즘을 선택하시오\n");
        printf("1. opt  2. FIFO 3. LRU 4. Second-Chance) : ");
        getNumericInput(&select4);
        if(select4 == 1 || select4 == 2 || select4 == 3 || select4 == 4) {
            break;
        }
        else{
            printf("올바른 입력이 아닙니다. 다시 입력하세요 \n");
        }

    }

    while(1){
        
        printf("E. 가상주소 스트링 입력방식을 선택하시오\n ");
        printf("1. input.in 자동 생성  2. 기존 파일 사용) : ");
        getNumericInput(&select5);
        
        // input.in파일 생성
        if(select5 == 1){
            
            createFile();

            FILE *inputFile = fopen("input.in", "r");

            if(inputFile == NULL){
                perror("파일 열기 실패");
            }
            
            switch (select4)
            {
            case 1:
                opt(inputFile);
                break;
            case 2:
                fifo(inputFile);
                break;
            case 3:
                lru(inputFile);
                break;
            case 4:
                sc(inputFile);
                break;
            
            default:
                break;
            }


            fclose(inputFile);
            break;

        // 기존 파일 사용
        } else if(select5 == 2)  {
            
            char filename[100];
            FILE *inputFile;

            while(1){
                printf("입력 파일 이름을 입력하시오:");
                scanf("%s", filename);

                inputFile = fopen(filename, "r"); //파일 열기 (읽기 모드)

                if(inputFile == NULL){
                    perror("파일 열기 실패");
                    printf("파일이 존재하지 않거나 열 수 없습니다. 다시 입력해주세요.\n");
                }else{
                    break; 
                }

            }

            switch (select4)
            {
            case 1:
                opt(inputFile);
                
                break;
            case 2:
                fifo(inputFile);
                break;
            case 3:
                lru(inputFile);
                break;
            case 4:
                sc(inputFile);
                break;
            
            default:
                break;
            }

            fclose(inputFile);

            break;
        }
        else{
            printf("올바른 입력이 아닙니다. 다시 입력하세요 \n");
        }

    }
            
            
}
