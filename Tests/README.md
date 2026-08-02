# Unity 테스트 가이드

`Tests/`는 `Assignments/`의 운영 코드를 [Unity](Unity/README.md)로 검증하는
호스트 기반 단위 테스트 공간이다. Unity는 C용 테스트 프레임워크이며, assertion
결과를 모아 보여 주고 테스트가 하나라도 실패하면 0이 아닌 종료 코드를 반환한다.

현재 테스트 대상은 `Assignments/src/token/`이다.

## 디렉터리 구조

```text
Tests/
├── Makefile
├── README.md
├── what-is-tdd.md
├── Unity/                 # 외부 Unity 프레임워크
└── token/
    └── test_token.c       # token 모듈의 테스트와 테스트 main
```

빌드 결과는 `Tests/build/`에 생성된다. `Assignments/src/main.c`는 링크하지
않는다. 테스트 프로그램은 `test_token.c`의 `main()`에서 시작해 token 함수를
직접 호출한다.

## 실행 방법

저장소 루트에서 다음 명령을 실행한다.

```sh
make -C Tests test
```

`make -C Tests` 또는 `make -C Tests token`도 같은 테스트를 실행한다. 성공하면
Unity가 다음과 같은 요약을 출력하고 종료 코드 0을 반환한다.

```text
7 Tests 0 Failures 0 Ignored
OK
```

AddressSanitizer와 UndefinedBehaviorSanitizer로 잘못된 메모리 접근과 정의되지
않은 동작을 확인할 수 있다. 디버거나 샌드박스의 `ptrace` 환경에서도 실행되도록
이 타깃에서는 LeakSanitizer만 끈다.

```sh
make -C Tests sanitize
```

누수 검사는 Valgrind가 설치된 Linux 환경에서 별도로 실행한다.

```sh
make -C Tests memory
```

생성된 실행 파일을 지우려면 다음 명령을 실행한다.

```sh
make -C Tests clean
```

## 현재 token 테스트의 계약

`token/test_token.c`는 공개 헤더인 `token.h`만을 기준으로 다음 동작을
검증한다.

- `token_init()`이 타입과 값을 저장하고 `next`를 `NULL`로 초기화한다.
- factory가 입력 문자열을 복제해 독립적으로 소유한다.
- 정확히 `"|"`인 값만 `TOKEN_PIPE`이고 일반 문자열과 `"||"`는
  `TOKEN_WORD`이다.
- 새 token list는 비어 있다.
- list에 추가한 token은 삽입 순서를 유지한다.
- `token_list_add_token()`은 `NULL` list 또는 token을 거부하고 기존 상태를
  바꾸지 않는다.
- 각 테스트 뒤에 생성한 token을 정리한다. sanitizer가 이 경로도 검사한다.

여기서 `"||"`가 `TOKEN_WORD`라는 것은 현재 구현의 계약이다. minishell에서
`||`를 문법 오류나 별도 연산자로 다루기로 결정했다면, 원하는 결과로 테스트를
먼저 바꿔 실패(Red)를 확인한 뒤 구현을 수정한다.

## Unity 테스트 읽는 법

Unity 테스트 파일에는 세 부분이 있다.

```c
void setUp(void)
{
    /* 각 테스트 직전에 실행: 깨끗한 fixture 준비 */
}

void tearDown(void)
{
    /* 각 테스트 직후에 실행: 할당한 자원 정리 */
}

void test_new_token_list_is_empty(void)
{
    TEST_ASSERT_NULL(g_tokens.head);
}
```

마지막 `main()`의 `RUN_TEST()` 한 줄이 테스트 하나를 등록한다.

```c
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_new_token_list_is_empty);
    return (UNITY_END());
}
```

assertion이 실패하면 Unity는 파일, 줄 번호, 기대값과 실제값을 출력하고 다음
테스트를 계속 실행한다. 직접 만든 PASS/FAIL 전역 변수나 출력 코드는 더 이상
필요하지 않다.

자주 사용하는 assertion은 다음과 같다.

```c
TEST_ASSERT_EQUAL_INT(TOKEN_PIPE, token->type);
TEST_ASSERT_EQUAL_STRING("|", token->value);
TEST_ASSERT_EQUAL_PTR(first, list.head);
TEST_ASSERT_NULL(token->next);
TEST_ASSERT_NOT_NULL(token);
```

## 테스트 하나를 추가하는 순서

1. 아직 없는 동작을 설명하는 `test_...` 함수를 `token/test_token.c`에 쓴다.
2. `main()`에 같은 함수의 `RUN_TEST(...)`를 추가한다.
3. `make -C Tests test`로 의도한 이유 때문에 실패하는지 확인한다(Red).
4. `Assignments/src/token/`을 통과할 만큼만 수정한다(Green).
5. 이름과 중복을 정리한 뒤 `test`와 `sanitize`를 다시 실행한다(Refactor).

테스트 이름에는 내부 구현보다 관찰 가능한 행동을 적는다. 하나의 테스트에서
여러 규칙을 한꺼번에 검증하기보다, 실패 이유가 바로 드러나도록 행동 하나에
집중하는 편이 좋다.

다른 모듈을 추가할 때는 Makefile의 source 목록과 실행 파일 구성을 모듈별로
분리하면 된다. 지금은 token 학습에 필요한 최소 구성만 유지한다.
