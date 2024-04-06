
# 3.1 Failures and Recovery

## (1) Failure Classfication

- **Transaction failure** : 너가 잘못짠거 (Deadlock일 때 하나 철회해야한다. 걔한테는 Failure나 마찬가지이다.)
- **System failure (crash)** : main memory에서 문제가 발생해서 나가리 (정전, os 문제, 하드웨어 결함)
- **Disk failure** : 어떤 이유로든 디스크 내용이 사라지는 것

> 항상 트랜잭션이 ACID를 할 수 있도록 지원해야한다. 

## (2) Recovery Algorithm

- database의 consistency와 transaction (atomicity, durability) 를 만족하기 위해서 회복을 시켜야한다. 
> 이 정도면 ACID 다시 보고 와야겠지?

2가지 파트로 나누어져 있는 회복 알고리즘
1. 정상 상태에서 시스템 회복
2. 수행 중인 연산에 장애가 발생했을 때
회복을 하기 위해 ==log를 남겨볼 수 있다. ==


## (3) 알아야할 CS지식

#### 1. 저장 장치
- **Volatile Storage**  
	- main memory, cache memory
- **Nonvolatile Storage**
	- disk, tape, flash memory, non-volatile (battery backed up) RAM
- **Stable storage** 
	- 어떤 장애가 발생하더라도 저장 내용을 유지하는 것을 말한다. (RAID)

#### 2. Stable-Storage 어떻게 구현할거임?

- seperate disks 에다가 블록의 여러 copy를 만들어 저장한다.  



