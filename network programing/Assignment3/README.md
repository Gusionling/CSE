# Assignment 3: 고급 네트워크 패턴

다중 클라이언트 처리 및 ZeroMQ 라이브러리를 활용한 다양한 메시징 패턴을 구현

## 파일: [solution.py](./solution.py)

## 개요
- Assignment 2의 SSL/TLS 서버-클라이언트 구조 확장
- 스레딩을 통한 다중 클라이언트 동시 처리
- ZeroMQ 라이브러리를 사용한 다양한 메시징 패턴 구현

## 구현 기능

### 1. 다중 클라이언트 처리
- 스레딩 라이브러리를 활용한 동시에 여러 클라이언트 처리
- 각 클라이언트 연결에 대한 별도 스레드 생성 및 관리
- 스레드 간 자원 공유 및 경쟁 조건 방지

### 2. ZeroMQ 메시징 패턴

#### PUB-SUB 패턴 (뉴스 발행-구독)
- 서버(발행자)와 클라이언트(구독자) 간 비동기 통신
- 클라이언트가 특정 주제(카테고리)를 구독하면 해당 주제의 뉴스만 수신
- `NewsGenerator` 클래스를 통한 다양한 주제의 뉴스 생성
- 사용자가 선택한 카테고리에 대한 실시간 뉴스 수신

### 3. 추가 작업: Subscribe

- 클라이언트가 관심 있는 뉴스 카테고리를 선택하여 구독
- 서버는 다양한 카테고리(business, entertainment, health, science, sports, technology)의 뉴스 생성
- 서버에서 생성된 뉴스 중 구독한 카테고리의 뉴스만 클라이언트로 전송
- ZeroMQ의 PUB-SUB 소켓 타입 활용

## 통신 프로세스

### 기본 SSL/TLS 통신 (Assignment 2와 동일)
1. 클라이언트는 SSL/TLS로 암호화된 JSON 쿼리를 서버로 전송
2. 서버는 요청을 복호화, 압축 해제, 파싱 후 적절한 작업 수행
3. 응답을 JSON으로 직렬화, 압축, 암호화하여 클라이언트로 반환

### PUB-SUB 패턴 (뉴스 구독)
1. 클라이언트가 'subscribe' 작업과 뉴스 카테고리를 서버로 전송
2. 서버는 요청을 처리하고 ZeroMQ PUB 소켓 생성
3. 클라이언트는 ZeroMQ SUB 소켓을 생성하고 선택한 카테고리를 구독
4. 서버는 주기적으로 다양한 카테고리의 뉴스를 생성하여 발행
5. 클라이언트는 구독한 카테고리의 뉴스만 지속적으로 수신 및 표시

## 클래스 및 주요 구성 요소

### NewsGenerator 클래스
```python
class NewsGenerator:
    def __init__(self):
        # 뉴스 주제, 이벤트, 회사 초기화
        self.topics = ["business", "entertainment", "health", "science", "sports", "technology"]
        self.events = ["new product launch", "merger", "acquisition", "lawsuit", "scandal", "government regulation"]
        self.companies = ["Apple", "Microsoft", "Google", "Amazon", "Facebook", "Tesla"]

    def get_news(self):
        # 랜덤 뉴스 헤드라인 생성
        topic = random.choice(self.topics)
        event = random.choice(self.events)
        company = random.choice(self.companies)
        headline = topic + " " + company + " " + event
        return headline
```

## 실행 방법

### 필요 파일
- `solution.py`: 메인 스크립트
- `ssu.pem`: 서버 인증서 및 개인 키
- `ssu.crt`: CA 인증서
- ZeroMQ 라이브러리 설치: `pip install pyzmq`

### 서버 실행
```bash
python solution.py localhost 8080 -s ssu.pem
```

### 클라이언트 실행
```bash
python solution.py localhost 8080 -a ssu.crt
```

### 사용법
1. 클라이언트에서 작업 선택 (ping, toggle, subscribe)
2. 'subscribe' 선택 시, 뉴스 주제 입력 (business, entertainment, health, science, sports, technology 중 하나)
3. 구독한 주제의 뉴스가 실시간으로 표시됨

## 학습 내용
- 멀티스레딩을 통한 동시성 처리
- ZeroMQ 라이브러리와 메시징 패턴
- 발행-구독(PUB-SUB) 패턴의 이해와 구현
- 비동기 네트워크 통신
- 메시지 필터링 및 라우팅