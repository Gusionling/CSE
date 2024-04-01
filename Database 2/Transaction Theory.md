
# 1차시 (Orientation)

#### database 1과 뭐가 다른가?
- User 입장에서 DB를 바라보기
- SQL, normalization
- 트랜잭션, 시스템 내부 구조, 객체 관계형 데이터베이스( ver 2 )


# 2차시 (TRANSACTION THEORY)

## 1.1 Transaction Concept

app은 query를 계속 만든다. 
query의 묶음을 transaction이라고 볼 수 있다. 

>[!SQL과 Transaction의 관계?]
>트랜잭션은 DB의 상태를 변화시키는 하나 이상의 SQL 명령어들의 집합으로, 이러한 명령어들이 모두 성공적으로 실행되거나, 하나라도 실패하면 실행되지 않도록 하는 기능을 한다. 

**개념**(주저리~ 주저리~)
- 하나의 논리적 작업을 수행하는 데이터베이스 연산의 순서이다. 
- 데이터를 접근하고 갱신하는 데이터베이스 프로그램의 논리적 수행 단위이다. 
- 데이터베이스에 적용되는 모든 연산은 read/write로 대치할 수 있다. 

![[Pasted image 20240308122339.png]]


**Transaction Management**
- **이슈**
	- 다수개의 트랜젝션을 동시에 수행하였을 때 발생하는 문제점을 해결하는 **동시성 제어 기능**(Concurrent)  - 자원 활용성이 증가한다. (처리량과 관련이 있다)
	- 시스템 장애를 극복하는 **회복 기능**
- **ACID**
	- **Atomicity** : 원자성 - all or nothing : 다 수행이 되거나 실패하면 아예 실행이 안된 상태로...!
	- **Consistency** : 일치성 - 단일 트랜잭션의 수행은 데이터 무결성을 유지한다. 트랜잭션이 실행되기 전과 후의 DB의 상태가 일관성을 유지해야한다. A,B의 계좌의 총합은 항상 같아야한다~
	- **Isolation** : 독립성 - 여러 트랜잭션이 동시에 실행될 때, 각 트랜잭션이 독립적으로 실행되어야 하며, 다른 트랜잭션의 중간 상태를 볼 수 없다. 
	- **Durability** : 지속성 - 트랜잭션의 수행결과는 반드시 DB에 반영이 되어 있어야한다.  
- **DB의 개입**
	- 같은 자원에 접근할 때 DB는 어떤 식으로 transaction의 순서를 결정할까? 
	![[Pasted image 20240308124707.png]]
	- Consistency를 보장하기 때문에 T1의 Transaction이 끝나고 수행을 시키면 된다. 

**Transaction State**
- Active
- Partially committed
- Aborted
- Committed

![[Pasted image 20240308125214.png]]
- **Begin Transaction**: 트랜잭션이 시작되는 지점을 나타냅니다.
- **Active**: 트랜잭션이 활성 상태로, 여기서는 SQL 명령어들을 통해 데이터베이스에 대한 읽기/쓰기 작업이 수행됩니다.
- **Partially Committed**: 트랜잭션이 실행을 마치고 종료 조건을 기다리는 단계입니다. 모든 명령어가 성공적으로 실행되었지만, 아직 데이터베이스에 반영(commit)되지는 않은 상태를 말합니다.
- **Committed**: 트랜잭션이 성공적으로 완료되었으며, 모든 변경 사항이 데이터베이스에 영구적으로 반영되었습니다.
- **Aborted**: 트랜잭션이 실패했거나 사용자 혹은 데이터베이스 관리 시스템에 의해 중단되었습니다. 이 상태에서 트랜잭션은 롤백되어 트랜잭션 이전 상태로 되돌아갑니다.
- **Terminated**: 트랜잭션 처리가 완전히 끝나고, 시스템에서는 트랜잭션에 대한 정보를 제거할 수 있는 상태입니다.

## 1.2 Serializability

concurrent하게 수행을 할 때 Serializability를 만족시키면 consistent가 만족이 된다. 

**Correct Execution**
- Conflict serializable
- View serializable

**NonSerializable Execution** 
트랜잭션을 올바르게 수행하지 못하는 경우 발생할 수 있는 현상들이다. 
- dirty read : 완료되지 않은 값을 읽는 것이다. 완료되지 않은 값을 읽었으니까 완료되지 않은 값이 rollback되었을 때 얘를 읽는 애도 rollback되어야 한다. 
- Lost update
- unrepeatable read : 한 트랜잭션 사이에 같은 값을 읽었을 때 그 값은 같아야한다. 

**Schedules**
- 동시적으로 수행되는 다수 트랜잭션에 속하는 연산이 수행된 시간적 순서를 의미한다. 
- 동시적으로 수행된 트랜잭션의 모든 연산이 스케줄에 나와있어야한다. 
- 스케줄에 있는 특정 트랜잭션에 속하는 연산 순서는 또한 해당 트랜잭션 내의 연산 순서와 동일 하여야한다. 

- Serial Schedule : 그냥 하나의 트랜잭션이 끝나고 다음게 온다. 

![[Pasted image 20240312152502.png]]
- 와리가리 하고 있긴하지만 접근하는 자원에 있어서는 serial하다. 
- 결론적으로 얘도 Serializable하다고 한다. (근데 책에서는 직렬 스케줄은 아니라고 함 Serializable하다는 것과 serial 스케줄은 다른 것 같다. )

>[!질문]
>시간 순이라고 했는데 그림만 보고는 뭔 이득을 보는지 모르겟음 (1,2 보다)
>T1의 B에 대한 연산을 T2 A연산할 때 같이 해야하는 것 아닌가?
>한 트랜잭션이 실행이 되는 동안 다른 트랜잭션이 해당 데이터를 변경할 수 없도록 함으로써, 병렬 처리 시 발생할 수 있는 문제들을 방지한다. 


![[Pasted image 20240312152754.png]]
- 공유 자원에 대한 동기화 작업이 잘못됐다. => NonSerializable하다 

#### Conflicting Instructions
- 동일한 데이터에 대한 두 개 연산 중에서 최소한 ==한 개 연산이 쓰기 연산==이면, 두 개 연산은 충돌한다고 한다. (실행 순서를 바꾸면 결과가 다르게 나온)

Conflict Serializable
- 위치를 바꿀 수 있는 연산(비충돌 연산)들의 순서를 바꿔서 serial schedules 이 나오는 경우이다. 
![[Pasted image 20240312160055.png]]
<T3, T4> 는 T3다음 T4가 와도 되는지 => 안된다. 

#### View Serializability 
- 정의

![[Pasted image 20240312161351.png]]
- Conflict Serializable하지는 않지만 view Serializable 하다. 
- 스케줄 s에 속한 트랜잭션의 읽기 연산에 대하여 s' 스케줄에서도 동일한 값을 읽고 s의 마지막 쓰기 연산이 s'스케줄의 마지막 쓰기 연산과 동일하면 스케줄 s'은 뷰 직렬가능 스케줄이라고 한다. 
- 얘네들은 이런 특징이 있다 : blind write(Read하지 않고 Write)
- 뭔가....write(Q)끼리의 순서는 상관이 없어보인다. 해설지에서 <T5, T6, T7>의 T7쓰기 연산과 스케줄 8의 데이터 Q에 대한 마지막 쓰기이 동일하다고 하기 때문이다. 

![[Pasted image 20240315141634.png]]
- conflict serializable이 아닌건 뭐 바로 알거고
- view serializable이 아닌 이유는 <T1, T2> , <T2, T1>을 해보았을 때 읽기 연산에 대해서 기존과 다른 값을 읽게 된다. 

## How to Test Serializability

- **Precedence graph**를 그려본다. 
- cycle이 있는지 확인한다. 

![[Pasted image 20240315142130.png]]
- 당연하겠지만 하나의 트랜잭션에만 사용되는 자원은 그래프에 그리지 않는다. 
- Precedence graph를 그렸을 때 장점?
	- cycle이 생기는 지 확인함으로써 conflict Serializable한지 확인한다. 
	- 트랜잭션끼리의 순서를 바꾸는데 가이드라인을 준다. 
	- 위를 보면 크게 T1 -> (T2, T4) -> T4 와 어디로든 가도되는 T5가 있다.  총 몇개? 2 x 5로 10개의 위상 정렬이 가능하다. 


# Recoverability

**Cascading rollback**
![[Pasted image 20240319151135.png]]
T10 을 commit하고 T11에서 A를 읽는 식으로 하면 Cascaing rollback을 방지할 수 있다. 


![[Pasted image 20240319151344.png]]
ACA는 W-R관계만 본다. ST는 W-W도 본다. 

### Concurrency Control
- Serializability 테스트하는 거슨 너무 오래걸려
- 그래서 등장한게 Concurrency Control protocols이다. 