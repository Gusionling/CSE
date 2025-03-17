# 과제 4 - 페이지 교체 알고리즘 시뮬레이터

가상 메모리 관리에 사용되는 주요 페이지 교체 알고리즘들(Optimal, FIFO, LRU, Second-Chance)의 동작을 시뮬레이션하는 프로그램을 구현

## 구현 사항

다음 4가지 페이지 교체 알고리즘의 동작을 시뮬레이션하는 프로그램 구현:

1. **Optimal 알고리즘**
   - 앞으로 가장 오랫동안 사용되지 않을 페이지를 교체
   - 미래의 참조 문자열을 미리 알고 있다고 가정

2. **FIFO (First-In-First-Out) 알고리즘**
   - 가장 먼저 들어온 페이지를 교체

3. **LRU (Least Recently Used) 알고리즘**
   - 가장 오랫동안 사용되지 않은 페이지를 교체

4. **Second-Chance (One-handed Clock) 알고리즘**
   - FIFO의 변형으로, 참조 비트를 활용한 2차 기회 제공

## 시뮬레이션 설정

- **가상주소 길이**: 18~20bits 설정 가능
- **페이지(프레임) 크기**: 1KB, 2KB, 4KB 중 선택
- **물리 메모리 크기**: 32KB, 64KB 중 선택
- **참조 패턴**: 
  - 입력 파일에서 5,000개의 가상주소 읽기 또는
  - 랜덤 함수로 5,000개의 가상주소 생성

## 파일 구조

- `page_simulator.c`: 메인 시뮬레이터 프로그램
- `optimal.c`: Optimal 알고리즘 구현
- `fifo.c`: FIFO 알고리즘 구현
- `lru.c`: LRU 알고리즘 구현
- `second_chance.c`: Second-Chance 알고리즘 구현
- `Makefile`: 컴파일 스크립트

## 컴파일 및 실행 방법

1. 컴파일:
   ```bash
   make
   ```

2. 실행:
   ```bash
   ./page_simulator
   ```

## 실행 화면 예시

```
A. Simulation에 사용할 가상주소 길이를 선택하시오 (1. 18bits 2. 19bits 3. 20bits): 1
B. Simulation에 사용할 페이지(프레임)의 크기를 선택하시오 (1. 1KB 2. 2KB 3. 4KB): 3
C. Simulation에 사용할 물리메모리의 크기를 선택하시오 (1. 32KB 2. 64KB): 1
D. Simulation에 적용할 Page Replacement 알고리즘을 선택하시오
   (1. Optimal 2. FIFO 3. LRU 4. Second-Chance): 2
E. 가상주소 스트링 입력방식을 선택하시오
   (1. input.in 자동 생성 2. 기존 파일 사용): 1
```

## 프로그램 사용법

1. 시뮬레이션 설정:
   - 가상주소 길이 선택 (18bits, 19bits, 20bits)
   - 페이지(프레임) 크기 선택 (1KB, 2KB, 4KB)
   - 물리 메모리 크기 선택 (32KB, 64KB)
   - 페이지 교체 알고리즘 선택 (Optimal, FIFO, LRU, Second-Chance)

2. 가상주소 스트링 입력 방식 선택:
   - 입력 파일 생성
   - 기존 파일 사용

## 출력 형식

각 알고리즘별로 출력 파일(output.opt, output.fifo, output.lru, output.sc)이 생성되며, 다음 정보를 포함:

- 가상주소(V.A.)
- 페이지 번호(Page No.)
- 프레임 번호(Frame No.)
- 물리주소(P.A.)
- 페이지 폴트 발생 여부(Hit/Fault)
- 마지막에 총 페이지 폴트 발생 횟수

## 알고리즘 구현 세부 내용

### 1. Optimal 알고리즘

```c
int findOptimalVictim(int page_frames[], int n_frames, int future_references[], int n_refs, int current_pos) {
    int farthest = -1, victim_idx = -1;
    
    for (int i = 0; i < n_frames; i++) {
        int page = page_frames[i];
        int j;
        
        for (j = current_pos; j < n_refs; j++) {
            if (future_references[j] == page) {
                if (j > farthest) {
                    farthest = j;
                    victim_idx = i;
                }
                break;
            }
        }
        
        // 앞으로 참조되지 않는 페이지가 있으면 그 페이지를 바로 선택
        if (j == n_refs) {
            return i;
        }
    }
    
    return (victim_idx == -1) ? 0 : victim_idx;
}
```

### 2. Second-Chance 알고리즘

```c
int findSecondChanceVictim(int page_frames[], int reference_bits[], int n_frames, int *clock_hand) {
    while (1) {
        if (reference_bits[*clock_hand] == 0) {
            int victim = *clock_hand;
            *clock_hand = (*clock_hand + 1) % n_frames;
            return victim;
        } else {
            reference_bits[*clock_hand] = 0;
            *clock_hand = (*clock_hand + 1) % n_frames;
        }
    }
}
```

## 실험 결과 분석

알고리즘별 성능 비교를 위해 다음 항목을 분석했습니다:

### 1. 페이지 폴트 발생 횟수 비교

테스트 결과, Page Fault 횟수는 다음과 같이 측정되었습니다:
- Optimal: 3064회
- FIFO: 4349회
- LRU: 4342회
- Second-Chance: 4354회

### 2. 알고리즘별 성능 분석

1. **Optimal 알고리즘**
   - 미래 참조 패턴을 미리 알고 있어 가장 성능이 좋음
   - 가장 오랫동안 사용되지 않을 페이지를 교체하여 Page Fault 횟

## 참고 자료

- 운영체제 교재: Operating System Concepts (Silberschatz, Galvin, Gagne)
- 가상 메모리 관리: https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/9_VirtualMemory.html
- 페이지 교체 알고리즘: https://www.geeksforgeeks.org/page-replacement-algorithms-in-operating-systems/