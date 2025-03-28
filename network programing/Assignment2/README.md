# Assignment 2: SSL/TLS 보안 통신

SSL/TLS를 활용한 안전한 클라이언트-서버 통신

## 파일: [solution.py](./solution.py)

## 개요
- TLS/SSL을 활용한 클라이언트-서버 보안 통신 구현
- 데이터 압축을 통한 네트워크 트래픽 최적화
- 다양한 네트워크 에러 처리 구현
- JSON 기반 쿼리 및 응답 처리

## 구현 기능

### 1. 보안 통신
- SSL/TLS를 사용한 클라이언트-서버 간 암호화 통신
- 인증서를 통한 서버 인증
- 단방향 인증 (서버 인증) 구현

### 2. 데이터 압축
- zlib 라이브러리를 활용한 데이터 압축
- 네트워크 대역폭 사용 최적화
- 압축/압축해제 프로세스 구현

### 3. 오류 처리
- 다양한 네트워크 오류 상황에 대한 예외 처리:
  - 연결 오류 (Connection Error)
  - 소켓 오류 (Socket Error)
  - 타임아웃 오류 (Timeout Error)
  - SSL 인증서 오류 (SSL Certificate Error)
  - 잘못된 클라이언트 요청 오류

### 4. 로깅
- 모든 중요 이벤트에 대한 로깅 구현 (연결, 연결 해제, 오류)
- 시간, 로그 레벨, 메시지 포맷 지정

### 5. 지원 작업

#### Ping 작업
- 도메인 이름을 IP 주소로 변환
- 사용자로부터 도메인 이름 입력 받기
- DNS 조회를 통한 IP 주소 반환

#### Toggle 작업
- 문자열의 대소문자 변환
- 사용자로부터 문자열 입력 받기
- 영문자 대/소문자 전환 후 결과 반환

## 통신 프로세스
1. **클라이언트**:
   - JSON 형식의 쿼리 문자열 생성
   - zlib으로 데이터 압축
   - SSL/TLS를 통한 암호화 및 서버로 전송

2. **서버**:
   - 암호화된 데이터 수신 및 복호화
   - 압축된 데이터 해제
   - JSON 쿼리 파싱 및 요청된 작업 수행
   - 결과를 JSON 형식으로 생성, 압축, 암호화하여 반환

## 실행 방법

### 필요 파일
- `solution.py`: 메인 스크립트
- `ssu.pem`: 서버 인증서 및 개인 키
- `ssu.crt`: CA 인증서

### 서버 실행
```bash
python solution.py localhost 8080 -s ssu.pem
```

### 클라이언트 실행
```bash
python solution.py localhost 8080 -a ssu.crt
```

### 명령행 인자
- `host`: 서버 호스트 이름 또는 IP 주소
- `port`: 서버 포트 번호
- `-s certfile`: 서버 인증서 파일 경로 (서버 모드)
- `-a cafile`: CA 인증서 파일 경로 (클라이언트 모드)

## 학습 내용
- SSL/TLS 통신 원리 및 구현
- 인증서 기반 인증 메커니즘
- 데이터 압축 기법
- JSON 데이터 직렬화/역직렬화
- 멀티스레딩을 통한 다중 클라이언트 처리
- 네트워크 예외 처리 및 로깅