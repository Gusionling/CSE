
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
그럼 얘는 serializability를 보장할까? 
- 정답은 아니다 ~ read(A)와 read(B) 사이에서 값이 update가 되어버리면 display(A+B)가 달라지게 된다. 
==추가적인 록 규약 필요== 

### Two-phase Locking Protocol

- phase 1 : growing phase (증가하는 단계) 
- phase 2 : shrinking phase (감소하는 단계)

>[!GPT 설명]
>**잠금 획득 단계** : 트랜잭션이 실행되면서 필요한 모든 데이터 항목에 대해 잠금을 획득한다. 이 단계에서는 잠금을 계속 획득할 수 있지만, 일단 잠금을 해제하기 시작하면 더 이상 새로운 잠금을 획득할 수 없다. 이는 트랜잭션이 데이터를 읽거나 수정하기 전에 해당 데이터에 대한 접근 권한을 확보해야 함을 의미한다. 
>**잠금 해제 단계** : 트랜잭션이 필요한 모든 작업을 완료하고, 획득한 잠금을 해제하기 시작한다. 이 단계에서는 더이상 새로운 잠금을 획득할 수 없으며, 모든 잠금을 해제해야만 트랜잭션을 종료할 수 있다.

설명이 맞는지 확인하는 작업이 필요하다. 

**뭔가 개념이 많이 난잡하지만....**
- Conflict Serializable Schedules
	- 트랜잭션의 직렬 실행이 가능한 순서를 말한다. 즉, 동시에 실행되는 여러 트랜잭션이 있을 때, 이 트랜잭션들이 서로 간섭 없이 순차적으로 실행될 수 있는 순서를 찾는 것이다. 2PL은 이러한 스케줄을 생성하는데 도움을 준다. 
- Cascading Rollback
	- 하나의 트랜잭션이 실패했을 때 다른 트랜잭션들이 줄줄이 롤백 되야하는 암울한 상황....
- Strict Two-Phasing Locking
	- 트랜잭션이 커밋하거나 중단할 때 까지 모든 **독점적 잠금**을 유지한다. (심각한 성능저하가 예상이 됩니다.....)
- Rigorous Two-phasing Locking
	- stric 보다 더 빡센거다 트랜잭션이 **모든 잠금**을 커밋하거나 중단할 때까지 유지해야한다. 

### Lock Conversions

- First phase(잠금 획득 단계)
	- 트랜잭션이 데이터를 읽은 후에 수정해야 할 필요가 생겼을 때 공유 잠금을 독점 잠금으로 변환할 수 있다. (upgrade)
- Second phase(잠금 해제 단계)
	- 데이터 항목에 대한 변경을 완료한 후, 다른 트랜잭션이 해당 항목을 읽을 수 있도록 독점 잠금을 공유 잠금으로 변환할 수 있다. (이미 독점적으로 가지고 있었기 때문에 추가 승인이 필요하지 않다.)

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
T1이 가지고 있는 lock은 s락이다. 

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
	- 필요 없는 데이터 항목에 대한 록을 그래프 기반 규약상의 이유로 획득하여야 한다는 것이다. (부분순서가 있기 때문에 )

#### Tree-based Protocol
- 규약
	- 록 해제는 제약 없이 가능하다. 해제 후 다시 잡을 수도 있다. 
	- 한번 락을 획득하고 해제한 데이터 항목에 대해서는 다시 록을 잡을 수 없다. 
- 2PL과 차이점
	- 록을 해제 한 다음에 록을 획득할 수가 있다는 