# 과제 3 - 리눅스 스케줄러 정책 테스트

리눅스 커널 5.15.120 버전에서 다양한 스케줄링 정책(CFS, RT_FIFO, RT_RR)을 변경하면서 프로세스들의 실행 순서 및 실행 시간을 확인하는 프로그램을 구현

## 구현 사항

1. **스케줄링 정책 변경 및 테스트**
   - 4가지 스케줄링 정책 테스트: CFS_DEFAULT, CFS_NICE, RT_FIFO, RT_RR
   - 각 정책을 선택하면 21개의 자식 프로세스 생성
   - 모든 프로세스는 동일한 계산 작업(배열 곱셈)을 수행
   - 프로세스 종료 순서 및 실행 시간 측정

2. **RT_RR 타임 슬라이스 변경 테스트**
   - RT_RR 스케줄링에서 Time Slice를 3가지(10ms, 100ms, 1000ms)로 변경
   - 각 설정에 따른 실행 시간 비교

3. **CPU 코어 제한**
   - 정확한 분석을 위해 CPU 코어를 1개로 제한


## 파일 구조

### 기본 구현
- `scheduler_test.c`: 스케줄링 정책 테스트 프로그램
- `Makefile`: 컴파일 스크립트

### 가산점 구현 (해당 시)
- `rt_sched.c`: RT_FIFO 스케줄러 구현 수정 파일
- `sched_entity.h`: CPU 버스트 시간 추적을 위한 헤더 파일

## 컴파일 및 실행 방법

1. 컴파일:
   ```bash
   gcc -o sched sched.c
   ```

2. CPU 코어 제한 후 실행:
   ```bash
   # RT_FIFO와 RT_RR은 sudo를 이용하여 실행
   sudo ./sched
   ```

3. RT_RR 스케줄링 정책의 Time Slice 설정:
   ```bash
   # Time Slice를 10ms로 설정
   sudo echo "10">/proc/sys/kernel/sched_rr_timeslice_ms
   
   # Time Slice를 100ms로 설정
   sudo echo "100">/proc/sys/kernel/sched_rr_timeslice_ms
   
   # Time Slice를 1000ms로 설정
   sudo echo "1000">/proc/sys/kernel/sched_rr_timeslice_ms
   ```

## 프로그램 사용법

1. 실행 후 스케줄링 정책 선택:
   ```
   Input the Scheduling Policy to apply:
   1. CFS_DEFAULT
   2. CFS_NICE
   3. RT_FIFO
   4. RT_RR
   0. Exit
   ```

2. RT_RR 선택 시 Time Slice 값 지정 (10ms, 100ms, 1000ms)

## 구현 세부 내용

### 배열 곱셈 작업

모든 프로세스는 다음과 같은 동일한 작업을 수행합니다:

```c
while(count < 100) {
    for(k = 0; k < 100; k++) {
        for(i = 0; i < 100; i++) {
            for(j = 0; j < 100; j++) {
                result[k][j] += A[k][i] * B[i][j];
            }
        }
    }
    count++;
}
```

### 스케줄링 정책 구현

1. **CFS_DEFAULT**
   - 기본 Completely Fair Scheduler 정책
   - 모든 프로세스가 공정하게 CPU 시간을 할당받음

2. **CFS_NICE**
   - 첫 7개 프로세스: Nice 값 -19 (높은 우선순위)
   - 다음 7개 프로세스: Nice 값 1 (기본 우선순위)
   - 마지막 7개 프로세스: Nice 값 19 (낮은 우선순위)

3. **RT_FIFO**
   - 실시간 스케줄링 정책으로 선입선출 방식
   - `sched_setattr()` 함수를 사용하여 구현
   - `attr.sched_policy = SCHED_FIFO`로 설정

4. **RT_RR**
   - 실시간 스케줄링 정책으로 라운드 로빈 방식
   - `sched_setattr()` 함수를 사용하여 구현
   - `attr.sched_policy = SCHED_RR`로 설정
   - Time Slice 값을 10ms, 100ms, 1000ms로 변경하여 실험

### 시간 측정 방법

`gettimeofday()` 함수를 사용하여 각 프로세스의 시작 시간과 종료 시간을 측정
- 부모-자식 프로세스 간 시간 정보를 파이프를 통해 전달

## 실험 결과 분석

각 스케줄링 정책마다 다음 항목을 분석했습니다:

1. **CFS_DEFAULT**
   - 모든 프로세스가 완벽히 공정하게 수행됨
   - 모든 프로세스의 실행 시간이 약 7초대로 비슷하게 측정됨
   - 평균 실행 시간: 약 7.013476초

2. **CFS_NICE**
   - 우선 순위에 따라 Nice 값이 -19인 프로세스들이 먼저 실행됨
   - 그 다음 Nice 값이 1인 프로세스들이 실행됨
   - 가장 마지막에 Nice 값이 19인 프로세스들이 실행됨
   - 평균 실행 시간: 약 5.640281초

3. **RT_FIFO**
   - 생성된 순서대로 프로세스들이 실행 완료됨
   - 각 프로세스가 종료될 때까지 다른 프로세스는 CPU를 할당받지 못함
   - 평균 실행 시간: 약 4.045026초

4. **RT_RR**
   - Time Slice에 따라 실행 패턴이 달라짐
   - Time Slice 10ms: 평균 실행 시간 약 7.038203초
   - Time Slice 100ms: 평균 실행 시간 약 5.045066초
   - Time Slice 1000ms: 평균 실행 시간 약 4.003331초
   - Time Slice가 커질수록 문맥 교환 횟수가 줄어들어 성능이 향상됨

## 참고 자료

- Linux 스케줄러 문서: https://www.kernel.org/doc/Documentation/scheduler/sched-design-CFS.txt
- Real-Time 스케줄링 정책: https://man7.org/linux/man-pages/man7/sched.7.html
- taskset 명령어: https://man7.org/linux/man-pages/man1/taskset.1.html