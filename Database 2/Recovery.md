
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

#### 3. DISK I/O should be atomic

- 디스크 I/O는 페이지 단위로 이루어진다. 
- 당연하게 atomic 해야한다. (read는 큰 문제가 없지만 write에서 문제가 생긴다. )
- os가 단독으로 지원이 불가능해서 하드웨어의 서포트가 필요하다. 
	- ex) battery-backup memory 

#### 4. 데이터 위치

![[Pasted image 20240416100608.png]]

- 동일 데이터가 존재할 수 있는 영역
	- **디스크 블록**
	- **메인 메모리 상의 데이터 버퍼 블록**
		- 버퍼를 두는 이유 : 처리량을 높이기 위해서이다. (disk에서 가져오는 건 너무 큰 비용이 들거든)
		- write를 하면 buffer에 데이터가 있는지 확인을 한다. 만일 없다면 disk에서 가져와야 한다. (Buffer Replacement 정책 때문에 버퍼에서 없어져있을 수도 있다.)
		- ==연산을 하기 위해서는 buffer에 있어야 한다. ==(작업대에 올려야지)
	- **트랜잭션 프로세스 메모리 영역**
- read, write는 사용자가 , input, output 은 os가 
- 데이터를 읽고 쓸떄는 block 단위로 읽는다. 
	- 버퍼까지도 block으로 관리하고 프로그램에서는 순수 데이터로 작업을 한다. 
- commit 은 버퍼에 쓰기 까지이다. 



# 3.2 Logs

#### (1) Recovery approach
- **log-based recovery**
	- 로그를 저장하는 곳은 항상 stable 해야한다. 
	- DBMS에서는 2개의 버퍼 (기존 버퍼 + 로그 버퍼)
- **shadow-paging**
	- 2개의 블락
		- 업데이트 전 
		- 업데이트 후
	- db pointer는 가장 최신 버전 데이터를 가리키고 있다. 
	- abort 가 생기면 old copy of DB 쪽으로 pointing 하면된다. 
	- 단점 : 트랜잭션이 많을 경우 그림자가 많이 생성되서 문제가 발생할 수 있으며 클러스터를 원천적으로 제공하지 못한다. 

#### (2) Logging
- 정상처리 
	- 로그는 로그 레코드에 저장되는데 단방향으로 쭉 저장이된다. 
	- 로그 레코드는 Update가 있을 때 발생한다. 
	- <Ti, X, V1, V2> V1이 업데이트 전, V2가 업데이트 후
- 동시성 문제 
	- strict 2PL : cascading rollback을 방지하기 위함, committed read
	- 로그 블록 하나 내에는 다수개의 트랜잭션 로그 레코드가 저장된다. (트랜잭션은 동시에 다수가 실행되므로)

#### (3) Logging : Checkpoint

> why? (왜 필요한가) : system failure에 대해 원할하게 대응하기 위해서 (redo와 undo의 가이드라인 제시)

- 디스크에 쓰여지기 전에 로그 레코드에 값을 저장해야한다. 
- 검사점 연산 : main memory의 모든 로그 레코드와 변경된 데이터 페이지를 디스에 반영하여, 디스크에 저장되어 있는 데이터베이스 상태와 주기억장치에 저장되어 있는 데이터베이스 상태를 동일하게 하는 것이다. 

![[Pasted image 20240417144055.png]]
T1 : checkpoint 이전에 이미 모든 것이 끝났기 때문에 디스크에도 잘 반영이 된 상태이다. 건들 필요가 없다. 
T2 , T3 : 버퍼에 commit까지 있을 것이다. 하지만 disk 에는 있을지 모른다. 
(log record에 commit 이 반영되어 있으면 redo, 안되어 있으면 undo : 확실한지는 모르겠다....)
T4 : 확실하게 commit이 안된 상태이므로 undo를 해주어야 한다. 

#### (4) Logging : Recovery
- Backwards 방식으로 스켄을 해야한다. 
	- undo-list : checkpoint까지 backword하는데 commit이 없는 트랜잭션
	- redo-list : checkpoint까지 backword하는데 commit이 있는 트랜잭
	- undo -> redo 작

# 3.3 Buffer Management

#### (1) 개요
DB에 저장이 되어 있는 형태는 파일이다. 
buffer는 왜 있는 것일까?
버퍼로 가져와서 여러 트랜잭션이 사요할 수 있게 하기 위함이다. 
**Buffer Manager** : buffer replacement나 allocation 같은 작업을 한다. 

- buffer에서 교체하는 block이 수정사항이 있는 block이면 disk에 반영을 해준다. 
- os가 block을 교체할 수도 있고 독립적으로 교체를 할 수도 있다. 

#### (2) Buffer-Replacement 정책

- LRU (Least Recently Used)
	- 오래동안 참조 되지 않은 블락을 교체한다. 
	- worst case : nested-loop join 에서 반복문이 처음부터 끝까지 같다가 다시 처음부터 순차적으로 간다는 특징 때문에 LRU의 worst case이다. 
- Pinned block 
	- 디스크에 block이 flush 되는것을 허용하지 않는다. 
	- WAL 요구사항으로 블록이 pinned 되어야하는 시간이 존재한다. 
		- wal : Write and logging : 로그 레코드에 먼저 저장을 하고 디스크에 저장을 한다. 
	- 자주 쓰이는 block이 디스크로 flush되는 것을 방지
- Toss-immediate strategy
	- 버퍼 블록에 있는 마지막 터플을 처리하면 바로 free
	- 해당 블록을 앞으로 더이상 참조할 필요가 없는 경우에 효과적이다.
- MRU (most recently used)
	- 최근에 쓰인 놈은 꺼지쇼

#### (3) Data Page Buffering
- 이 데이터 페이지를 버퍼링 한다는 것이 무슨 말일까...
- 데이터 블록을 디스크에 쓰기 연산을 할 때 중간에 누가 바꾸면 안되겠지? 
	- latch 혹은 semaphore를 사요한다. 
- what is latch?
	- 운영체제가 지원하는 기능으로서 동기화 문제에 쓰인다.
	- 짧은 시간 소유할 수 있게 한다. 
- 구현
	- 메모리를 잡아서 쓰기
		- 공간 낭비에 대해서 취악하다 
		- 실시간으로 유연하게 버퍼를 잡는 것이 불가능하다 (os가 현재 시스템의 요구 사항에 따라  )
	- virtual memory
		- dual paging 문제가 생길 수 있다. 
		- paging : 내가 원하는 페이지가 없을 때 읽어 들이는 것 
			- 내가 output하고자 하는 블락이 swap에 있을 때 메모리로 불러와서 ouput 해야하기 때문에 dual이다. 
		- 운영 체제가 수정된 페이지를 제거해야 할 필요가 있을 때, 해당 페이지는 디스크 상의 스왑 공간(swap space)에 기록됩니다.
		- 해결 : 블락을 swap 영역에 output하지 말고 그냥 디스크에 바로 output하자
- slotted page Structure
	- 가변길이 레코드를 지원한다. 
	- header page다음에 정해진 수의 slot이 나온다. 
		- slot은 그 페이지에 저장되는 레코드의 주소를 가진다. 
	- Header : id, 소유자, 여부공간,,,,등등의 메타정보 
	- 레코드 : 사진에서 보는 것 같이 끝에서부터 저장이 된다. (메모리 heap같은 느낌)
	- 접근 : 페이지 id와 slot 번호만 가지면 접근 가능하다 
		- 페이지 내에서 레코드가 물리적 위치가 변한다면 slot에 들어가는 페이지 주소만 변경하면 되기 때문에 
![[Pasted image 20240417222431.png]]

#### (4) Steal vs Force Policy

회복 알고리즘은 데이터 페이지 처리 방식에 따라서 여러 방식이 가능하다 
- Force : 커밋하는 시점에 모든 update된 블락을 db로 write한다. 
	- 커밋하는 트랜잭션에 관련된 블락들
	- WAL을 지켜야한다. 
- No force : 굳이 commit 시점에 force하지 않겠다. 
- Steal : 커밋 전에 update된 블락을 write하겠다. 

![[Pasted image 20240417223100.png]]
- Steal 하는 방식에서 fail이 발생하면 undo 작업을 해줘야한다. (commit 전이니까)
- Not Force는 Redo(로그 레코드의 값을 읽어서...!, Wal을 지키기 때문에 log가 있다. )
- Not stal의 문제점은 buffer가 항상 꽉찰 수 있다. 왜냐하면 반영을 안 시켰기 떄문이다. 
- force는 커밋시점에 다 써버리니까 건들일 필요가 없다. =>redo, undo가 필요없음