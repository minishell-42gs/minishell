# Unity 테스트 가이드

`Tests/`는 `Assignments/`의 운영 코드를 [Unity](Unity/README.md)로 검증하는
호스트 기반 단위 테스트 공간이다. Unity는 C용 테스트 프레임워크이며, assertion
결과를 모아 보여 주고 테스트가 하나라도 실패하면 0이 아닌 종료 코드를 반환한다.

현재 테스트 대상은 `Assignments/src/`의 `app`, `cmd`, `lexer`, `parser`,
`parsing_facade`, `token`, `util` 모듈이다. `main.c`는 테스트 실행 파일과
진입점이 충돌하므로 링크하지 않는다.

## 디렉터리 구조

```text
Tests/
├── app/test_app.c
├── cmd/
│   ├── test_cmd.c
│   ├── test_cmd_factory.c
│   └── test_cmd_list.c
├── lexer/test_lexer.c
├── parser/test_parser.c
├── parsing_facade/test_parsing_facade.c
├── token/test_token.c
├── util/test_free_split.c
├── scripts/run_tests.sh            # 테스트 실행과 전체 결과 집계
├── Makefile
├── README.md
├── what-is-tdd.md
└── Unity/                         # 외부 Unity 프레임워크
```

각 테스트 파일은 자체 `main()`을 가진 독립 실행 파일로 빌드된다. 운영 소스와
libft 소스는 그대로 `Assignments/`에서 읽되, 테스트용 오브젝트는
`Tests/build/obj/`에 생성한다. sanitizer용 오브젝트는
`Tests/build/sanitize/obj/`에 별도로 생성한다. 따라서 테스트 빌드는
`Assignments/` 아래에 `.o`나 `libft.a`를 만들지 않는다.

## 실행 방법

저장소 루트에서 다음 명령을 실행한다.

```sh
make -C Tests test
```

`make -C Tests`도 전체 테스트를 실행한다. 특정 모듈의 테스트만 실행하려면
모듈 이름을 타깃으로 사용한다.

```sh
// 예시
make -C Tests cmd
make -C Tests lexer
make -C Tests parser
make -C Tests token
...
```

테스트의 실행과 결과 집계는 `scripts/run_tests.sh`가
담당하고, Makefile은 테스트 빌드 및 실행기 호출만 담당한다.

AddressSanitizer와 UndefinedBehaviorSanitizer로 잘못된 메모리 접근과 정의되지
않은 동작을 확인할 수 있다. 운영 모듈과 libft도 sanitizer 옵션으로 다시
컴파일한다. 디버거나 샌드박스의 `ptrace` 환경에서도 실행되도록 이 타깃에서는
LeakSanitizer만 끈다.

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

1. 대상 모듈 디렉터리의 `test_...c`에 아직 없는 동작을 설명하는 함수를 쓴다.
2. `main()`에 같은 함수의 `RUN_TEST(...)`를 추가한다.
3. `make -C Tests test`로 의도한 이유 때문에 실패하는지 확인한다(Red).
4. `Assignments/src/token/`을 통과할 만큼만 수정한다(Green).
5. 이름과 중복을 정리한 뒤 `test`와 `sanitize`를 다시 실행한다(Refactor).

테스트 이름에는 내부 구현보다 관찰 가능한 행동을 적는다. 하나의 테스트에서
여러 규칙을 한꺼번에 검증하기보다, 실패 이유가 바로 드러나도록 행동 하나에
집중하는 편이 좋다.

새 테스트 파일을 `Tests/<module>/test_*.c` 형태로 추가하면 Makefile이 별도
실행 파일로 자동 빌드한다.
