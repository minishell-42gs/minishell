# TDD와 Unity를 처음 시작하기

TDD(테스트 주도 개발)는 구현을 다 만든 뒤 확인하는 방법이 아니라,
원하는 동작을 작은 테스트로 먼저 표현하며 설계를 진행하는 개발 방식이다.

```text
Red                 Green                    Refactor
실패 테스트 작성 → 통과하는 최소 구현 작성 → 동작을 유지하며 구조 개선
        ↑                                          │
        └──────────── 전체 테스트 재실행 ───────────┘
```

중요한 것은 테스트 코드의 양이 아니라 이 짧은 피드백 주기를 반복하는 것이다.

## 이 저장소에서 Unity가 하는 일

Unity는 TDD 자체가 아니라 TDD 주기를 빠르게 돌도록 돕는 C 테스트 프레임워크다.
`make -C Tests test`의 흐름은 다음과 같다.

```text
test_token.c + Unity + Assignments/src/token/*.c + libft.a
                            ↓ 컴파일
                    build/test_token
                            ↓ 실행
              각 RUN_TEST와 assertion 실행
                            ↓
                 성공은 0, 실패는 0이 아닌 값
```

Unity가 검증문 처리, 실패 위치 출력, 테스트 수 집계를 제공하므로 저장소에서
별도의 테스트 러너를 직접 유지할 필요가 없다.

## 첫 TDD 연습: 새 토큰 목록

### 1. Red(실패) — 가장 작은 기대를 쓴다

먼저 새 토큰 목록이 비어 있어야 한다는 동작을 테스트로 표현한다.

```c
void test_new_token_list_is_empty(void)
{
    t_token_list list;

    TEST_ASSERT_EQUAL_INT(OK, token_list_init(&list));
    TEST_ASSERT_NULL(list.head);
}
```

그리고 `main()`에 등록한다.

```c
RUN_TEST(test_new_token_list_is_empty);
```

이 시점에는 반드시 테스트를 실행한다.

```sh
make -C Tests test
```

컴파일 실패도 Red다. 다만 오타나 빠진 include 때문에 실패한 것인지, 정말 아직
동작이 없어서 실패한 것인지 구분해야 한다. 기대한 이유로 실패하는 것을 확인해야
테스트가 실제로 새 동작을 검증한다고 믿을 수 있다.

### 2. Green(통과) — 통과하는 최소 코드만 쓴다

테스트를 통과시키는 데 필요한 구현만 작성한다.

```c
t_status token_list_init(t_token_list *this)
{
    this->head = NULL;
    return (OK);
}
```

다시 실행해서 새 테스트뿐 아니라 기존 테스트도 모두 통과하는지 본다. Green에서
미래 기능까지 미리 구현하지 않는다. 다음 행동은 다음 실패 테스트가 이끈다.

### 3. Refactor(구조 개선) — 통과 상태를 유지하며 정리한다

테스트가 통과하는 상태에서 중복, 함수 이름, 책임 분리를 개선한다. 코드가 동작하는
방식을 바꾸지 않더라도 작은 변경마다 테스트를 다시 실행한다.

메모리를 다루는 이 모듈에서는 다음 검사도 함께 실행하면 좋다.

```sh
make -C Tests sanitize
```

## 좋은 단위 테스트의 구성

하나의 테스트는 보통 준비–실행–검증(Arrange–Act–Assert) 순서로 읽힌다.

```c
void test_lexer_creates_a_word_token(void)
{
    t_lexer lexer;
    t_token_list tokens;
    t_token *token;

    TEST_ASSERT_EQUAL_INT(OK, lexer_init(&lexer));
    TEST_ASSERT_EQUAL_INT(OK, token_list_init(&tokens)); /* Arrange */
    TEST_ASSERT_EQUAL_INT(OK,
        lexer_run(&lexer, "echo", &tokens));            /* Act */

    token = tokens.head;
    TEST_ASSERT_NOT_NULL(token);                         /* Assert */
    TEST_ASSERT_EQUAL_STRING("echo", token->value);

    tokens.destroy(&tokens);
    lexer.destroy(&lexer);
}
```

실제 `test_token.c`는 동적 할당한 토큰의 소유권과 정리를 안전하게 다루기 위해
`setUp()`과 `tearDown()`으로 테스트 상태를 준비하고 정리하지만, 읽는 순서는 같다.

좋은 출발점은 다음과 같다.

- 테스트 이름만 읽어도 입력과 기대 행동을 알 수 있다.
- 한 테스트는 한 가지 실패 이유에 집중한다.
- 비공개 함수가 아니라 `token.h`의 공개 동작을 호출한다.
- 테스트끼리 상태를 공유하지 않고 어떤 순서로 실행해도 같은 결과가 난다.
- 성공 경로뿐 아니라 경계값과 잘못된 입력도 하나씩 추가한다.

## 테스트가 실패했을 때 읽는 순서

Unity가 실패를 보고하면 먼저 다음 세 가지를 확인한다.

1. 어떤 `test_...` 함수가 실패했는가?
2. 어느 검증식에서 실패했는가?
3. 기대값과 실제값은 각각 무엇인가?

그다음 바로 구현부터 고치지 말고 테스트의 기대가 정말 요구사항과 맞는지 확인한다.
테스트가 틀렸다면 테스트를 고치고, 요구사항이 맞다면 구현을 최소한으로 고친다.

## 버그를 발견했을 때

버그 수정도 같은 주기를 쓴다.

1. 버그를 가장 작은 입력으로 재현하는 테스트를 추가한다.
2. 수정 전 테스트가 실패하는지 확인한다.
3. 구현을 수정한다.
4. 새 테스트와 전체 회귀 테스트가 통과하는지 확인한다.

이렇게 남은 테스트는 같은 버그가 다시 들어오는 것을 막는 회귀 테스트가 된다.

## 테스트의 층: 단위, 통합, 수동

앞서 다룬 Unity 검사는 운영 코드를 직접 호출하는 단위 테스트다. 이 저장소에는
그 위에 두 층이 더 있다.

| 층 | 명령 | 검증 대상 | 잡는 것 |
|---|---|---|---|
| 단위 | `make -C Tests test` | 함수 하나의 계약 | 검증식 실패 |
| 단위 + 계측 | `make -C Tests sanitize` | 같은 테스트를 ASan/UBSan으로 | 버퍼 오버런, 해제 후 사용, 미정의 동작 |
| 단위 + Valgrind | `make -C Tests memory` | 같은 테스트를 Valgrind로 | 누수, 초기화 안 된 값 |
| 통합 | `make -C Tests integration` | 제출용 `Assignments/minishell` 실행 파일 | Readline 입력 루프부터 종료 코드까지, Bash와 비교 |
| 수동 | `Tests/integration/MANUAL_CHECKLIST.md` | 터미널에서만 볼 수 있는 동작 | 히스토리, 프롬프트 복귀, Ctrl+D |

단위 테스트는 `main.c`와 `app.c`의 대화형 입력 루프(REPL)를 실행하지 않는다. `executor.run`이
127을 돌려주는 것과 셸이 그 뒤에도 프롬프트를 다시 띄우는 것은 서로 다른 계층의 약속이다.
따라서 세 계층의 검사가 모두 통과해야 "사용자에게 약속한 동작이 지켜진다"고 말할 수 있다.

## 오류 출력 요구사항을 테스트로 나누기

오류 문구는 모두 `minishell`이 출력하는 것처럼 보이지만, 실제 출력 주체는 다르다.
이 구분을 놓치면 외부 프로그램의 오류까지 셸이 한 번 더 출력하거나, 셸이
출력해야 할 오류를 자식 프로그램에 기대하는 테스트가 생긴다.

아래의 `bash:`는 동작을 비교하는 기준일 뿐이다. minishell 자체 메시지는
프로젝트 이름에 맞춰 `minishell:`로 검증한다. `strerror()`가 만드는 시스템 오류
세부 문구는 운영체제 로캘에 따라 달라질 수 있으므로, 테스트에서는 고정 접두사·상태와
동일 환경의 `strerror()` 결과를 함께 사용한다.

| 종류 | 예시 | 실제 출력 주체 | 상태 | 테스트 층 | 현재 상태 |
|---|---|---|---:|---|---|
| 구문 오류 | `minishell: syntax error near unexpected token ...` | minishell 구문 분석기 | 2 | 파사드 단위 + 구문 분석기 통합 | 적용됨 |
| 모호한 리다이렉션 | `minishell: $FILE: ambiguous redirect` | minishell 확장기 | 1 | 통합 테스트 | 구현됨 |
| 명령 탐색 실패 | `minishell: foo: command not found` | minishell 실행기 | 127 | 파사드 단위 + 실행기 통합 | 적용됨 |
| 내장 명령 오류 | `minishell: cd: /none: No such file or directory` | minishell 내장 명령 | 보통 1 | 내장 명령 통합 테스트 | 구현됨 |
| 리다이렉션 오류 | `minishell: /protected/a: Permission denied` | minishell 리다이렉션 처리 | 보통 1 | 통합 테스트 | 구현됨 |
| `execve()` 실패 | `minishell: ./program: ...` | minishell 실행기의 자식 프로세스 | 보통 126, ENOENT는 127 | 파사드 단위 + 실행기 통합 | 적용됨 |
| 외부 프로그램 오류 | `ls: cannot access ...` | 실행된 `ls`/`grep` 등 | 프로그램이 정함 | 실행 통합 테스트 | 셸은 관여하지 않음 |
| 시그널 종료 상태 | `Segmentation fault`, `Killed` | 부모 셸의 자식 회수와 시그널 처리 | `128 + signal` | PTY 통합 테스트 | 구현됨 |
| heredoc EOF 경고 | `minishell: warning: here-document ...` | minishell heredoc 처리 | 명령의 최종 상태 유지 | 통합 테스트 | 구현됨 |

`Tests/error/test_error.c`는 현재 파사드가 책임지는 여섯 가지 출력 형식과 상태를
고정한다. `Tests/integration/run_integration.sh`는 실제 REPL 경계에서 마지막 파이프 명령의
구문 오류(상태 2)와 오류 뒤 프롬프트 복귀를 검증한다. 없는 명령의 127, 직접 경로의
ENOENT(127)·권한 오류(126), PATH의 권한 오류도 동일한 통합 스크립트에 있다.

반대로 `ls /없는경로`처럼 프로그램이 이미 실행된 뒤의 오류는 `error_report()`의
테스트 대상이 아니다. minishell은 표준 오류를 가공하거나 `minishell:`을 덧붙이지
않고, 자식의 종료 상태만 `waitpid()`로 받아야 한다. 외부 프로그램의 정확한 문구는
프로그램 버전·로캘마다 달라질 수 있어, 통합 테스트에서는 셸이 출력에 개입하지
않는지와 종료 상태 전달을 중심으로 확인한다.

필수 실행 흐름의 파서, 확장, 내장 명령, 리다이렉션, 시그널, heredoc 기능은 구현되어 있다.
새로운 동작을 추가할 때는 해당 요구를 가장 작은 실패 테스트로 먼저 표현하고 구현한다.

## 스토리를 시작할 때

하나의 스토리는 사용자에게 보이는 동작 하나를 약속한다. 그 약속을 먼저
`run_integration.sh`에 사례로 적고 Red 상태를 확인한 뒤, 단위 테스트와 구현으로
이어간다.

1. `run_integration.sh` 끝에 `# vN: ...` 블록을 추가하고 새 케이스를 적는다.
   `make -C Tests integration`에서 해당 사례가 실패하는지 확인한다.
2. 필요한 함수마다 위의 Red–Green–Refactor 주기를 돈다.
3. 통합 사례가 통과하면 스토리가 끝난다. 터미널이 필요한 동작이 있으면
   `MANUAL_CHECKLIST.md`에 절을 추가한다.
4. `story` 브랜치에 병합할 때 이전 케이스는 지우지 않는다. 다음 스토리가 이전 약속을
   깨면 여기서 잡힌다.
