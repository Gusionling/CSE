# 과제 1 - 리눅스 커널 컴파일 및 PS 명령어 구현

이 과제는 Ubuntu 리눅스에서 커널을 직접 컴파일하고, `/proc` 파일 시스템을 활용하여 리눅스 시스템의 프로세스 정보를 출력하는 `ps` 명령어와 유사한 `newps` 명령어를 직접 구현하는 것입니다.

## 구현 요구사항

1. **가상 머신 및 우분투 리눅스 설치**
   - VMware 또는 Virtual Box를 이용하여 Ubuntu 20.04.6 LTS 설치
   - 사용자 이름 또는 컴퓨터 이름에 학번 포함

2. **리눅스 커널(linux-5.15.120) 컴파일 및 설치**
   - 커널 소스 다운로드
   - 커널 설정, 컴파일 및 설치
   - 설치 전후 커널 버전 확인

3. **리눅스 PS 명령어 구현(`newps`)**
   - `system` 함수 또는 `exec` 계열 함수를 사용하지 않고 구현
   - `/proc` 파일 시스템 활용
   - 실행 중인 프로세스 정보 출력

## 파일 구조

- `newps.c`: PS 명령어 구현 소스 코드
- `Makefile`: 컴파일 스크립트

## 컴파일 및 실행 방법

1. 컴파일:
   ```bash
   make
   ```

2. 실행:
   ```bash
   ./newps
   ```

실행 결과는 기존의 `ps` 명령어와 유사한 형태로 프로세스 정보가 출력됩니다:
```
PID TTY       TIME CMD
3568 pts/0    00:00:00 bash
3577 pts/0    00:00:00 ./newps
```

## 구현 세부 내용

### 1. 커널 컴파일 과정

커널 컴파일을 위해 다음과 같은 과정을 수행하였습니다:

1. 커널 소스 다운로드:
   ```bash
   wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.15.120.tar.xz
   tar -xvf linux-5.15.120.tar.xz
   cd linux-5.15.120
   ```

2. 커널 설정:
   ```bash
   make menuconfig
   ```

3. 커널 컴파일:
   ```bash
   make -j$(nproc)
   ```

4. 커널 모듈 컴파일 및 설치:
   ```bash
   sudo make modules_install
   sudo make install
   ```

5. 부트로더 업데이트:
   ```bash
   sudo update-grub
   ```

커널 설치 결과는 아래와 같이 확인할 수 있습니다:
```
$ uname -r
5.15.0-83-generic  # 설치 전

$ uname -r
5.15.120  # 설치 후
```

### 2. PS 명령어 구현

`newps` 프로그램은 다음과 같은 기능을 구현합니다:

1. `/proc` 디렉토리 스캔하여 숫자로 된 디렉토리(PID) 식별
2. 각 프로세스 디렉토리에서 `/proc/[pid]/stat`, `/proc/[pid]/cmdline`, `/proc/[pid]/status` 파일 분석
3. 프로세스 정보(PID, 상태, 명령어, CPU 사용량, 메모리 사용량) 추출 및 출력

주요 함수:
- `getTTY`: 프로세스의 터미널 정보를 가져오는 함수
- `getCMD`: 프로세스의 명령어 정보를 가져오는 함수
- `getTIME`: 프로세스의 CPU 사용 시간을 계산하는 함수

`/proc` 파일 시스템에서 정보를 읽어와 표준 출력으로 보여주며, `system` 함수나 `exec` 계열 함수를 사용하지 않고 직접 구현했습니다.

## 실행 결과 예시

```
  PID   STATE   CMD                  CPU%   MEM%
    1   S       /sbin/init           0.0    0.1
    2   S       [kthreadd]           0.0    0.0
    3   I       [rcu_gp]             0.0    0.0
  ...
 1234   R       ./newps              0.1    0.2
  ...
```

## 참고 자료

- Linux 커널 문서: https://www.kernel.org/doc/html/latest/
- /proc 파일시스템 문서: https://man7.org/linux/man-pages/man5/proc.5.html
