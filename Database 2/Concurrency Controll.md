
# Lock-Based Protocols

mode
- exclusive (X) : read + write
	- +) 해당 데이터는 다른 트랜잭션에서 접근 불가능하다. 
- shared (S) : read onlyS

당연한 얘기들~~
- 록이 있어야 연산을 시작할 수 있다. 
- 읽기 연산 -> 록 남발 가능
- 쓰기 연산 -> 단 하나의 록

![[Pasted image 20240322000616.png]]
locking과 unlocking이 감싸는 형태인데 이는 well-formed schedule이라고 한다 그럼 얘는 serializability를 보장할까? 
- 정답은 아니다 ~ read(A)와 read(B) 사이에서 값이 update가 되어버리면 display(A+B)가 달라지게 된다. 
==추가적인 록 규약 필요== 

### Two-phase Locking Protocol

- phase 1 : growing phase (증가하는 단계) 
- phase 2 : shrinking phase (감소하는 단계)

>[!GPT 설명]
>**잠금 획득 단계** : 트랜잭션이 실행되면서 필요한 모든 데이터 항목에 대해 잠금을 획득한다. 이 단계에서는 잠금을 계속 획득할 수 있지만, 일단 잠금을 해제하기 시작하면 더 이상 새로운 잠금을 획득할 수 없다. 이는 트랜잭션이 데이터를 읽거나 수정하기 전에 해당 데이터에 대한 접근 권한을 확보해야 함을 의미한다. 
>**잠금 해제 단계** : 트랜잭션이 필요한 모든 작업을 완료하고, 획득한 잠금을 해제하기 시작한다. 이 단계에서는 더이상 새로운 잠금을 획득할 수 없으며, 모든 잠금을 해제해야만 트랜잭션을 종료할 수 있다.
 

**뭔가 개념이 많이 난잡하지만....**
- Conflict Serializable Schedules
	- 트랜잭션의 직렬 실행이 가능한 순서를 말한다. 즉, 동시에 실행되는 여러 트랜잭션이 있을 때, 이 트랜잭션들이 서로 간섭 없이 순차적으로 실행될 수 있는 순서를 찾는 것이다. 2PL은 이러한 스케줄을 생성하는데 도움을 준다. 
- Cascading Rollback
	- 하나의 트랜잭션이 실패했을 때 다른 트랜잭션들이 줄줄이 롤백 되야하는 암울한 상황....
- Strict Two-Phasing Locking
	- 그냥 2pl은 write에 대한 잠금을 풀어버리면 타 트랜잭션이 이 값을 읽을 수 있으니까 cascading rollback의 위험성이 있다
	- 트랜잭션이 커밋하거나 중단할 때 까지 모든 **독점적 잠금**(x-lock)을 유지한다. (심각한 성능저하가 예상이 됩니다.....)
- Rigorous Two-phasing Locking
	- stric 보다 더 빡센거다 트랜잭션이 **모든 잠금**을 커밋하거나 중단할 때까지 유지해야한다. (write 뿐만아니라 read에 대해서도)

> 2PL 이 생성하지 못하는 conflict serializable이 존재한다. 

### Lock Conversions

> 데이터에 대한 록을 요구할 때 동일 데이터에 대한 록을 기존에 할당 받았다면 기존 록에 대한 모드만 변환된다. (새로 가져오는 것이 아니라)

- First phase(잠금 획득 단계)
	- 트랜잭션이 데이터를 읽은 후에 수정해야 할 필요가 생겼을 때 공유 잠금을 독점 잠금으로 변환할 수 있다. (upgrade)
- Second phase(잠금 해제 단계)
	- 데이터 항목에 대한 변경을 완료한 후, 다른 트랜잭션이 해당 항목을 읽을 수 있도록 독점 잠금을 공유 잠금으로 변환할 수 있다. (이미 독점적으로 가지고 있었기 때문에 추가 승인이 필요하지 않다)

##### Why 2PL ensures conflict serializability
- Serializable 하지 않다고 가정을 하자 
- precedence graph에서 사이클이 존재한다고 생각해보자
- αi는 transaction i가 마지막으로 lock을 받는 시점이다. 
	- 선행 그래프에서 TiàTj 에지가 있으면 반드시 αi < αj 인 것이다.
	- α0 < α1 < … < αn < α0
	- Since α0 < α0 is a contradiction
>[!전제의 이해]
>Ti가 lock을 풀어야 Tj가 lock을 얻는데 Ti가 lock을 푼다는 것은 이미 shrinking에 들어갔다는 말이다. 따라서 αi < αj 일 수밖에 없다. 


**lock의 구현**
read(D)
```
•The operation read(D) is processed as:  
If Ti has a lock on D  
then  
  read(D);  
else begin  
  if necessary wait until no other transaction has a
     lock-X on D;  
  grant Ti a lock-S on D;  
  read(D);  
end;
```
x록을 가지고 있는 트랜잭션만 없다면 s록을 획득하고 읽는다. 


write(D)
```
•write(D) is processed as:  
If Ti has a lock-X on D  
then  
  write(D)  
else begin  
  if necessary wait until no other transaction has any lock on D;  
  if Ti has a lock-S on D  
	  then upgrade lock on D to lock-X  
  #아무 lock도 없는 경우
  else grant Ti a lock-X on D;  
  
  write(D);  
end;
```
- lock manager는 내부적으로 해쉬 테이블을 이용해서 록들을 관리한다. 
![[Pasted image 20240322125029.png]]
왼쪽 작은 네모 박스는 해쉬 버킷을 의미한다. i7과 i23은 동일한 해쉬 값을 가지고 있는 것이다. 

T1이 가지고 있는 lock은 s락이다. 
T2가 요구하는 것은 쓰기 모드여야한다. 

### DeadLock
![[Pasted image 20240322130022.png]]
lock을 놓지 않고 요청을 하는 모습이다. 
victim transaction을 정해야한다. 

starvation : 한 놈만 계속 victim이 되면 기아 상태가 된다. 

>[!note]
>DeadLock은 발생할 수 있지만 Starvation은 무조건 예방해야한다. 

# Graph-based Protocol

- 가정 : 록을 걸려고 하는 데이터에 대해서 부분 순서가 있어야 한다. 
- 장점
	- 교착 상태가 발생하지 않는다. (데이터 간에 부분 순서를 정한 후에 한 쪽 방향으로만 록 요구를 허용한다. )
- 단점
	- 필요 없는 데이터 항목에 대한 록을 그래프 기반 규약상의 이유로 획득하여야 한다는 것이다. (부분순서가 있기 때문에)
	- 회복 불가능한 스케줄을 생성하기도 한다. 회복가능은 commit과 관련이 있다. 

>그래프 기반 규약은 index와 같은 특정 데이터 영역에서만 사용이 된다. 



![[Pasted image 20240326150806.png]]
G를 획득하기 위해 E가 꼭 필요한 것은 아니지만 G와 E가 다 필요하다면 E를 획득하고 G를 잡아야한다. 해제하는 것은 순서가 없다. 

s락 왜 없어?
공유할 필요가 없다...?

#### Tree-based Protocol
- 규약
	- 록 해제는 제약 없이 가능하다. 해제 후 다시 잡을 수도 있다. 
	- 한번 락을 획득하고 해제한 데이터 항목에 대해서는 다시 록을 잡을 수 없다.
- 2PL과 차이점
	- 록을 해제 한 다음에 록을 획득할 수가 있다는 


# ==Multiple Granularity Locking==


![[Pasted image 20240329120742.png]]
다중 단위 계층 예제 
DB - Relation(file-물리적인 측면에서) - Page - Record
- lock을 걸었다면 그 노드에 자식 노드들도 같은 락이 걸리게 된다. 

#### Intention Lock Models
- 어느 lock을 걸려면 그 노드와 하위 노드에 호환이 안되는 lock을 가지고 있는지 모두 탐색해야한다. 
- 이런 cost를 조금이나마 줄이기 위해 명확한 상호 배제 호환이 아닌 경우 내가 나중에 걸거다라고 암시하는 lock을 걸어 충돌을 줄였다. 
- 정말 하위의 같은 노드에서 실제 lock 연산이 일어났을 때만 따지겠다는 것이다.
>하위 노드들을 모두 탐색해야하는 문제를 해결한 아이디어


**호환성** 
![[Pasted image 20240326153928.png]]
- SIX 같은 경우는 이미 S lock을 걸었다고 보면된다. 
	- 그렇기에 IX와 호환이 되지 않는 것이다. 
- IX는 하위 노드에서 x록 모드를 걸겠다는 것이 아니라 s록을 걸 수도 있다 
	- 그래서 IS와 IX가 호환이 되는 것이다. 근데 IX에서 X가 나타나버리면 s,x는 호환이 될 수 없기 때문에 이런 일은 발생할 수가 없다. 
- 상위 노드에서 SIX 록을 가지고 하위 노드에서 SIX 록을 요구하는 것은 올바른 방식이 아니다. (상위 노드에서 S 모드 록을 가지고 있는데 하위 노드에 중복하여 S 모드 록을 요구하므로)
- 록을 획득 하는 것은 root에서 leaf로 가지만 해제하는 것은 leaf에서 root로 간다. 
- 임의 노드에 SIX 록을 가지고 있으면, 하위 노드에 대하여는 IX 또는 X 록을 획득할 수 있다

>동일한 노드에서 락을 요구하는 것인지 아니면 하위노드에 락을 요구하는 것인지에 따라 다르다. 

# 2.3 Deadlock

- 자원을 가지고 있으면서 풀지 않고 다른 걸 가지려고 하는 경우

**애초에 차단하는 방식**
- 그래프 기반 locking protocol
- 트랜잭션이 수행되기 전에 모든 데이터에 대한 lock을 선언하는 방식
	- 근데 어떤 데이터를 쓸지 모르기 떄문에 이건 실효성이 없다 
##### Deadlock Handling
- Timeout-based Schemes
	- lock을 기다리는 시간을 잰다. 
	- Timeout 나면 roll-back 시켜버린다. 
	- 문제점 : 기아 상태 (특정 트랜잭션에 대해서만 abort)
- 교착 상태 방지 방식 (wait만 보면 발작해요)
	- wait-die (노인 공경)
		- 비선점이다. 
		- young한 얘가 old가 가지고 있는 자원을 wait하는 중이다 하면 rollback 시켜버린다. 
		- 죽은 얘의 time stamp는 맨 처음의 실행될 때의 timestamp를가지고 있어야한다. 
	- wond-wait
		- 선점한다. 
		- old가 younger를 rollback 시키게 강제한다. 그러나 young이 wait할 수도 있다. 그래서 wait-die보다는 rollback이 덜 일어날 수 있다. 
		- 마찬가지로 기존 타임스탬프를 가지고 재시동 해야한다. 
- 교착 상태 감지 및 해결 방식
	- 자 Deadlock이 생겨버렸다 어떻게 하면 될까?
	- Victime을 선정하면 된다. (비용이 작은 얘로)
		- 상용 데이터 베이스에서는 current blocker를 철회하는 방식을 가진다. current blocker는 사이클을 완성하는 대기를 제공하는 트랜잭션이다. 
	- Starvation이 생길 수 있다. 

# 2.4 Insert and Delete Operations

##### Insert와 Delete는 lock이 필요할까?
ex) Delete하고 Read하게 되면 문제가 발생한다. 
-> S-lock을 걸고 읽은 후에 삭제해야 한다. (당연히 읽기부터 먼저 해라~ 라는 이야기다)

- delete는 ==X-lock==이 필요하다
	- 기존에 그 자원에 대해서 s-lock이 있으면 안된다~
- insert도 ==x-lock==이 필요하다 

**팬텀**현상
- Insert나 delete 때문에 발생하는 현상이다. 
- tuple level locking에서 발생 
- lock을 걸 때 데이터의 단위가 recod
	- insert나 delete에는 새로운 lock이 걸리는 것이기 때문에 기존 얘와 걸리는 록이 없다. 

–T1 : Read(Account[100], Account[200], Account[300])

–T2 : insert(Account[400, Busan, 700])

–T2 : read(Assets[Busan])  // returns 1500

–T2 : write(Assets[Busan])    // writes 2200

–T1 : read(Assets[Busan])   // returns 2200

2phase locking 했는데 : lock 얻을 때 얻고 풀 때 쫙 푸는 
그러면 위 결과가  < T1, T2 > 나 <T2, T1> 과 같아야한다. (complict serializable이기 때문에 ) 
- < T1, T2 >이라면 read(Assets[Busan])가 1500이어야 하는데 consistency하지 않다. (2200)이 나온다. 
- Insert 때문에 그런거다
- T1은 T2가 insert한 걸 모른다. (Phantom, 유령인 거지~)

**tuple(record) 단위가 아니라 table(relation)단위로 쓰면 팬텀 현상이 생길까? (locking)을** 
- 절대 안생김 (물론 당연히 2PL)
- 위 같은 경우도 테이블을 읽으려 했는데 중간에 값을 넣어버려서 그런거다 그렇기 때문에 테이블에 locking을 걸어버리면 T1이 할 거 다 하고 준다 
- T1이 처음에 Account()에 s-lock을 걸어버리기 때문에 T2가 insert를 하지 못함
- concurrency가 떨어진다. 

>새로운 방식이 필요하다!!

### 인덱스가 뭔데?

![[Pasted image 20240420230948.png]]

인덱스는 데이터를 빠르게 탐색할 수 있도록 하는 자료구조이다. 
B+트리는 아래의 데이터들도 연결해 놓는데 이러면 장점이 무엇이냐!
>바로 ==범위 탐색이 빨라진다!==

인덱스는 컬럼을 복사해서 정렬을 해둔다. 그림의 나와있는 숫자도 어느 테이블의 나이나 학년 칼럼일 수도 있는 것이다. 
- 정렬을 해두어야 빠르게 찾지!!

+) 단점은 삽입 수정 삭제 할 때 살짝 에로 사항이....
#### Index 
- 그냥 쓰면 concurrency가 떨어진다. (다 잡고 있을거니까)
- 모든 relation에서 적어도 하나 이상의 인덱스를 가진다.
- 트랜잭션은 튜플을 하나 이상의 인덱스를 가지고 찾을 수 있다. 
- 특정 인덱스 버킷에 대한 잠금을 요구함으로써, 높은 동시성을 제공한다. 

#### Index + 2PL
- 인덱스는 트리 구조를 가질 수 있는데 실제 데이터의 인덱스는 리프 노드에 있고 비리프 노드는 리프 노드로의 포인터를 포함한다. 
- 조회(look up)을 수행하는 트랜잭션은 s-mode로 접근하는 모든 비리프/리프 노드를 잠가야한다. 
- 관계R(테이블)에서 삽입, 갱신, 삭제하는 트랜잭션 T는 삽입/갱신/삭제에 ==영향을 받는 모든 비리프/리프 노드에 대해 독점 잠금==(x-lock인듯?)을 얻어야한다.
- 팬텀이 절대 발생하지 않도록한다.트랜잭션이 인덱스를 통해 데이터에 접근할 때, 그 인덱스 구조상의 모든 관련 노드를 잠금으로써 다른 트랜잭션이 해다 노드들을 동시에 변경할 수 없게 한다. 

but 성능이 너무 떨어진다. 그래서 그래프 기반 록킹 규약을 사용한다. (색인 구조가) 동시성 저하가 너무 일어나니까
그래서 록을 미리 해제를 하는 방법이 고안되었다. 내부 노드에 대해서 

#### Crabbing (동시성 제어기술)
- root 노드부터 락을 건다. 
- child 록을 잡고 부모 락을 푼다. (락을 잡고 풀고 잡고- 뒤뚱 뒤뚱)
- 부노 노드에 구조 변화가 필요하면 (split, merge) 부모 노드에 대한 락을 요청한다. (부모 노드를 업데이트 해야 되기 때문이다.)
- 단점 ; Deadlock이 발생할 수 있다. (split/merge시에)
	- ex) parent 노드를 업데이트를 해야 될 상황


#### 색인 구조 동시성 제어
- 실제로는 nonserializable을 허용한다 
- 노드에 대한 split/merge가 필요하면 노드에 대한 록을 해제하고 B-tree split/merge를 한다. 
- 그럼 어떻게 동시성 제어를 하는가? (동시에 구조 변경이 들어갔을 때)
	- latch( semaphore) 를 써서 배타적인 수행을 보장한다. (색인 구조 변경 시에 배타적인 접근을 보장한다.)


# Transaction Isolation in SQL

- 트랜잭션에 관한 SQL 언어
	- set transcation : 트랜잭션에 관한 변수를 설정하는데 사용한다. 
	- commit 
	- rellback
	- savepoint : checkpoint와 같은 것이다. 
	- rollback to savepoint

- 완화된 분리도
	- Weak Levels of Consistency
	- 트랜잭션에 대한 완전 분리도를 제공하는 것이 아니다. 
	- trade off 
		- 정확성을 내리고 성능을 높인다. 
		- ex) 
			- 대략적 데이터를 읽기 위한 읽기 전용 트랜잭션
			- 쿼리 최적화를 위한 데이터베이스 통계
	- Degree-two Consistency(이단계 일치성)
		- s-lock을 제한 없이 해제할 수 있어서 직렬가능성 스케줄이 보장되지는 않는다. 
		- 커서 안정 방식(Cursor stability)
			- 데이터베이스 커서가 위치하는 동안에만 읽기 록을 잡고 있는 방식이다. 커서가 있으면 그냥 뭐 없음

#### Transaction Isolation in SQL
분리도를 사용자가 잘 맞춰서 지정을 해야하는데 
SQL 시스템은 총 4가지 실행 방식을 지원한다. 
- Serializable (얘만 직렬 실행 가능이다. )
- repeatable read : cascading rollback을 막겠다. - index locking이 필요가 없다. 
- read committed : x-lock은 끝까지 들고있어야한다. read-lock은 풀 수가 있다. (Degree-two consistency랑 같다. ) 읽기 연산에 대해서 lock을 잡는다. 
- read uncommitted : uncommitted 된 데이터도 읽을 수가 있다. 그래서 읽기 연산에서 그냥 lock을 잡지 않는다. 그래서 타 트랜잭션이 overwrite하는 것만 막는 것이다. 

![[Pasted image 20240421200944.png]]
- read uncommitted에서도 쓰기 록은 트랜잭션 마지막까지 들고 있다. 

