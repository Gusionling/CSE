# 과제 2 - 커스텀 시스템콜 구현

리눅스 커널 5.15.120 버전에서 커널을 수정하여 새로운 시스템콜을 구현하고, 이를 테스트하는 사용자 프로그램을 개발
자릿수 역순 변경과 부호 반대로 계산을 수행하는 두 가지 유형의 시스템콜을 구현했습니다.

## 구현 사항

다음 기능을 수행하는 시스템콜과 테스트 프로그램 구현:

1. **자릿수 역순 변경 시스템콜**
   - 자연수 입력 시 자릿수를 거꾸로 변경한 숫자 출력
   - 예: 123 → 321, 348957 → 759843

2. **부호 바꾸어 덧셈, 뺄셈 연산 시스템콜**
   - 자연수, 부호(+ 또는 -), 자연수 순서로 입력 시
   - 입력된 부호와 반대되는 연산 결과 출력
   - 예: 123+111 → 12 (빼기 연산 수행), 12350-13253 → 25603 (더하기 연산 수행)

3. **입력 오류 처리**
   - 두 가지 경우 외 입력 시 "Wrong Input!" 출력
   - 아무것도 입력되지 않으면 프로그램 종료

## 파일 구조

### 커널 수정 파일
- `syscall_64.tbl`: 시스템콜 테이블에 새 시스템콜 등록
- `syscalls.h`: 시스템콜 함수 프로토타입 선언
- `reverse_digits.c`: 자릿수 역순 변경 시스템콜 구현
- `opposite_calc.c`: 부호 바꾸어 연산 시스템콜 구현
- `Makefile`: 커널 컴파일 시 새 파일 포함

### 사용자 프로그램
- `assignment2.c`: 테스트용 사용자 프로그램
- `Makefile`: 컴파일 스크립트

## 시스템콜 구현 방법

### 1. 자릿수 역순 변경 시스템콜

```c
SYSCALL_DEFINE1(alpha_reverse, char *, str)
{
    int length = 0;
    int i;
    char ret[100];
    memset(ret, '\0', 100);
    
    while(str[length] != '\0'){
        length++;
    }
    
    for(i = 0; i<length; i++){
        ret[length - i - 1] = str[i];
    }
    copy_to_user(str, ret, sizeof(ret));
    
    return 0;
}
```

### 2. 부호 바꾸어 덧셈, 뺄셈 연산 시스템콜

```c
SYSCALL_DEFINE3(oper_add, long, a, long, b, long *, to_user){
    long result = 0;
    result = a + b;
    printk("[kernel Message] result = %ld\n",result);
    put_user(result, to_user);
    return 0;
}

SYSCALL_DEFINE3(oper_sub, long, a, long, b, long *, to_user){
    long result = 0;
    result = a - b;
    printk("[kernel Message] result = %ld\n",result);
    put_user(result, to_user);
    return 0;
}
```

## 컴파일 및 설치 방법

### 커널 수정 파일 목록

다음 파일들을 수정하거나 추가하여 시스템콜을 구현했습니다:

1. `syscall_64.tbl` (위치: /usr/src/linux/linux-5.15.120/arch/x86/entry/syscalls)
   - 새 시스템콜 등록:
     ```
     449 common alpha_reverse sys_alpha_reverse
     450 common oper_add sys_oper_add
     451 common oper_sub sys_oper_sub
     ```

2. `syscalls.h` (위치: /usr/src/linux/linux-5.15.120/include/linux)
   - 시스템콜 함수 프로토타입 추가:
     ```c
     asmlinkage long sys_alpha_reverse(char *str);
     asmlinkage long sys_oper_add(long a, long b, long *to_user);
     asmlinkage long sys_oper_sub(long a, long b, long *to_user);
     ```

3. `sys_alpha_reverse.c` (위치: /usr/src/linux/linux-5.15.120/kernel)
   - 문자열 역순 변경 시스템콜 구현

4. `sys_oper.c` (위치: /usr/src/linux/linux-5.15.120/kernel)
   - 연산 관련 시스템콜 구현

5. Makefile 수정 (위치: /usr/src/linux/linux-5.15.120/kernel)
   - 커널 빌드 시 새 파일 포함

### 커널 컴파일 및 설치

```bash
make -j$(nproc)
sudo make modules_install
sudo make install
sudo update-grub
```

시스템 재부팅 후 `uname -r` 명령어로 커널 버전이 5.15.120으로 변경되었는지 확인합니다.

### 테스트 프로그램 컴파일

```bash
gcc -o syscall assignment2.c
```

## 실행 방법

```bash
./syscall
```

## 실행 결과 예시

```
input: 123456
output : 654321
input: 12345732098
output : 89023754321
input: 12 + 6
output : 6
input: 12 - 6
output : 18
input: 123456 + 123455
output : 1
input: 
# 프로그램 종료
```

## 구현 세부 사항

테스트 프로그램은 다음 두 가지 유형의 입력을 처리합니다:

1. 자연수 입력 시 자릿수 역순 변경
   - 숫자만 입력되면 `alpha_reverse` 시스템콜을 호출하여 문자열을 역순으로 변경
   - 예: 123456 → 654321

2. 자연수, 부호, 자연수 입력 시 부호 반대로 계산
   - 입력된 부호와 반대되는 연산 결과 출력
   - '+' 입력 시 뺄셈 연산 수행 (sys_oper_sub 호출)
   - '-' 입력 시 덧셈 연산 수행 (sys_oper_add 호출)
   - 예: 12 + 6 → 6 (뺄셈 결과), 12 - 6 → 18 (덧셈 결과)

3. 잘못된 입력 시 "Wrong Input!" 출력

프로그램은 사용자 입력을 구문 분석하여 적절한 시스템콜을 호출하고 결과를 출력합니다.

## 참고 자료

- Linux 커널 소스 코드: https://github.com/torvalds/linux
- 시스템콜 구현 가이드: https://www.kernel.org/doc/html/latest/process/adding-syscalls.html