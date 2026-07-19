*This project has been created as part of the 42 curriculum by taegokim.*

# libft

## Description

**libft**는 C 표준 라이브러리의 필수 함수들을 재구현한 포괄적인 C 라이브러리입니다.
이 프로젝트의 목표는 문자열 처리, 메모리 관리, 포인터 조작 등 C 프로그래밍의 핵심 개념을 이해하고 구현하는 것입니다.

- **Part 1**: 표준 C 라이브러리 함수 - 문자 분류, 문자열 처리, 메모리 조작, 형변환 등
- **Part 2**: 추가 문자열 조작 함수 - 고급 문자열 처리 및 파일 디스크립터 기반 출력
- **Part 3**: 연결 리스트 유틸리티 - 동적 자료구조 구현 및 조작

## Instructions

### Compilation & Build

라이브러리를 컴파일하는 방법:

```bash
make 혹은 make all	 # 라이브러리 컴파일하여 libft.a 생성
make clean			# 생성된 오브젝트 파일 제거
make fclean  		# 생성된 모든 파일 제거
make re      		# 전체 재컴파일
```
빌드 프로세스는 다른 프로젝트에 링크할 수 있는 정적 라이브러리 `libft.a`를 생성합니다.

### Usage

다른 C 프로그램에서 libft를 사용하는 방법:

**방법 1: 오브젝트 파일과 라이브러리를 분리 컴파일**
```bash
gcc -c myprogram.c -I.
gcc myprogram.o libft.a -o myprogram
```

**방법 2: 라이브러리와 함께 직접 컴파일**
```bash
gcc myprogram.c libft.a -I. -o myprogram
```

**헤더 파일 포함:**
```c
#include "libft.h"
```

### Project Structure

- `libft.h` - 모든 함수 선언이 포함된 헤더 파일
- `ft_*.c` - 개별 함수 구현 파일들
- `MakeFile` - 빌드 설정 파일


## Library Details

### Part 1 - Libc Functions

C 표준 라이브러리 함수 구현.

#### Character Classification
- `ft_isalpha()` - 문자가 알파벳인지 확인
- `ft_isdigit()` - 문자가 숫자인지 확인
- `ft_isalnum()` - 문자가 영숫자인지 확인
- `ft_isascii()` - 문자가 ASCII 범위인지 확인
- `ft_isprint()` - 문자가 출력 가능한지 확인

#### String Functions
- `ft_strlen()` - 문자열의 길이 계산
- `ft_strchr()` - 문자의 첫 번째 위치 찾기
- `ft_strrchr()` - 문자의 마지막 위치 찾기
- `ft_strncmp()` - 문자열의 처음 n개 문자 비교
- `ft_strlcpy()` - 고정 크기 버퍼에 문자열 복사 (오버플로우 방지)
- `ft_strlcat()` - 고정 크기 버퍼에 문자열 연결 (오버플로우 방지)
- `ft_strnstr()` - 문자열에서 부분 문자열 찾기

#### Memory Functions
- `ft_memset()` - 메모리를 특정 바이트로 채우기
- `ft_bzero()` - 메모리 초기화
- `ft_memcpy()` - 메모리 영역 복사 (겹침 발생 시 정의되지 않은 동작)
- `ft_memmove()` - 메모리 영역 복사 (겹침 안전)
- `ft_memchr()` - 메모리에서 바이트 검색
- `ft_memcmp()` - 메모리 영역 비교

#### Conversion & Utility
- `ft_toupper()` - 문자를 대문자로 변환
- `ft_tolower()` - 문자를 소문자로 변환
- `ft_atoi()` - 문자열을 정수로 변환
- `ft_calloc()` - 0으로 초기화된 메모리 할당
- `ft_strdup()` - 문자열 복제 (새로운 메모리 할당)

### Part 2 - Additional Functions

고급 문자열 조작 및 유틸리티 함수:

#### String Manipulation
- `ft_substr()` - 문자열에서 시작 위치와 길이로 부분 문자열 추출
- `ft_strjoin()` - 두 문자열을 연결하여 새 문자열 생성
- `ft_strtrim()` - 문자열의 앞뒤에서 지정된 문자 제거
- `ft_split()` - 구분자로 문자열을 분할하여 문자열 배열 생성
- `ft_itoa()` - 정수를 문자열로 변환

#### String Iteration
- `ft_strmapi()` - 각 문자에 함수를 적용하여 새 문자열 생성
- `ft_striteri()` - 인덱스와 함께 각 문자에 함수 적용

#### File Descriptor Output
- `ft_putchar_fd()` - 파일 디스크립터에 단일 문자 출력
- `ft_putstr_fd()` - 파일 디스크립터에 문자열 출력
- `ft_putendl_fd()` - 파일 디스크립터에 문자열과 개행 출력
- `ft_putnbr_fd()` - 파일 디스크립터에 정수 출력

### Part 3 - Linked List Functions

동적 자료구조인 연결 리스트 구현 및 조작:

```c
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}   t_list;
```

#### List Operations
- `ft_lstnew()` - 새로운 리스트 노드 생성
- `ft_lstadd_front()` - 리스트 앞에 노드 추가
- `ft_lstadd_back()` - 리스트 뒤에 노드 추가
- `ft_lstsize()` - 리스트의 노드 개수 반환
- `ft_lstlast()` - 리스트의 마지막 노드 반환

#### List Manipulation
- `ft_lstdelone()` - 단일 노드 삭제 (메모리 해제 함수 사용)
- `ft_lstclear()` - 전체 리스트 삭제 (메모리 해제 함수 사용)
- `ft_lstiter()` - 각 노드에 함수 적용
- `ft_lstmap()` - 함수를 적용하여 새로운 리스트 생성

## Resources

이 프로젝트는 C 언어의 기본 개념들을 다루고 있습니다:

### Classic References
- **C 표준 라이브러리 문서** - man 페이지 등

### AI Usage
- **사용 도구**: copilot, gemini
- **사용 범위**:
  - README.MD 문서작성
  - 각 기능별 단위 테스트 코드
  - 버그 디버깅(ft_strlcat 등)
