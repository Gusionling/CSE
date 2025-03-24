# Assignment 4: HTTP 및 웹 프로그래밍

HTTP 프로토콜과 웹 프로그래밍을 활용한 다양한 웹 애플리케이션을 구현

## 1. 네트워크 용어 사전 (HTTP 서버/클라이언트)

### 파일
- [solution1_server.py](./solution1_server.py): HTTP 서버
- [solution1_client.py](./solution1_client.py): HTTP 클라이언트
- [solution1_csv.csv](./solution1_csv.csv): 네트워크 용어와 정의 데이터

### 개요
- WSGI를 활용한 HTTP 서버 구현
- HTTP 프로토콜 기반 키워드-정의 사전 서비스
- CSV 파일에서 네트워크 용어 및 정의 읽기
- GET 요청 처리 및 에러 핸들링

### 서버 기능
- Python의 wsgiref 모듈을 사용한 간단한 HTTP 서버 구현
- CSV 파일에서 네트워크 용어와 정의 데이터 로드
- 클라이언트 요청 파싱 및 적절한 응답 생성
- HTTP 오류 처리 (404 Not Found, 501 Not Implemented)
- 쿼리된 키워드에 대한 정의 제공

### 클라이언트 기능
- requests 라이브러리를 사용한 HTTP 요청
- 사용자로부터 키워드 입력 받기
- 서버로부터 받은 응답 출력

### 실행 방법
```bash
# 서버 실행
python solution1_server.py

# 클라이언트 실행
python solution1_client.py
```

## 2. DILab 멤버 정보 시스템 (웹 스크래핑 및 Flask)

### 파일
- [solution2_server.py](./solution2_server.py): Flask 서버 및 웹 스크래핑
- [solution2_client.py](./solution2_client.py): HTTP 클라이언트
- [solution2_csv.csv](./solution2_csv.csv): 스크래핑된 멤버 정보 데이터

### 개요
- Flask 웹 프레임워크를 사용한 RESTful API 개발
- BeautifulSoup을 활용한 웹 스크래핑
- 외부 사이트에서 DILab 멤버 정보 추출 및 가공
- 사용자 요청에 따른 멤버 정보 제공

### 서버 기능
- 웹 스크래핑:
  - 외부 사이트(DILab 멤버 페이지)에서 데이터 추출
  - BeautifulSoup을 사용한 HTML 파싱
  - 멤버 이름, 직책, 근무 기간, 연구 분야 등 정보 수집
  - 추출된 데이터를 CSV 파일로 저장

- Flask API:
  - RESTful API 엔드포인트 구현
  - 사용자 요청에 대한 JSON 응답 생성
  - 오류 처리 및 적절한 HTTP 상태 코드 반환
  - 멤버 이름을 기준으로 정보 검색 기능

### 클라이언트 기능
- HTTP GET 요청을 통한 멤버 정보 조회
- 사용자로부터 멤버 이름 입력 받기
- 서버 응답 출력 및 오류 처리

### 실행 방법
```bash
# 서버 실행
python solution2_server.py

# 클라이언트 실행
python solution2_client.py
```

### 데이터 형식
스크래핑된 멤버 정보는 다음 형식으로 CSV 파일에 저장됩니다:
- name: 멤버 이름
- job_role: 직책
- start_year: 시작 연도
- end_year: 종료 연도
- research_interest: 연구 분야 (현재 멤버의 경우)
- current_job_role: 현재 직책 (졸업생의 경우)
- pic_url: 프로필 사진 URL

## 학습 내용
- HTTP 프로토콜 및 WSGI 표준
- Flask 웹 프레임워크 활용
- RESTful API 설계 및 구현
- 웹 스크래핑 기법
- CSV 데이터 처리
- 오류 처리 및 예외 관리
- JSON 데이터 직렬화/역직렬화
- HTTP 클라이언트 요청 생성