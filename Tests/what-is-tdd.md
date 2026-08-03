# TDD와 Unity를 처음 시작하기

TDD(Test-Driven Development)는 구현을 다 만든 뒤 확인하는 방법이 아니라,
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

Unity가 assertion, 실패 위치 출력, 테스트 수 집계를 제공하므로 프로젝트에서
별도의 테스트 러너를 직접 유지할 필요가 없다.

## 첫 TDD 연습: 새 token list

### 1. Red — 가장 작은 기대를 쓴다

먼저 새 list가 비어 있어야 한다는 행동을 테스트로 표현한다.

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

### 2. Green — 통과하는 최소 코드만 쓴다

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

### 3. Refactor — 초록색을 유지하며 정리한다

테스트가 통과하는 상태에서 중복, 함수 이름, 책임 분리를 개선한다. 코드가 동작하는
방식을 바꾸지 않더라도 작은 변경마다 테스트를 다시 실행한다.

메모리를 다루는 이 모듈에서는 다음 검사도 함께 실행하면 좋다.

```sh
make -C Tests sanitize
```

## 좋은 단위 테스트의 모양

하나의 테스트는 보통 Arrange–Act–Assert로 읽힌다.

```c
void test_list_appends_one_token_as_head(void)
{
    t_token *token;

    token = token_factory_create(&g_factory, "echo"); /* Arrange */
    TEST_ASSERT_NOT_NULL(token);
    TEST_ASSERT_EQUAL_INT(OK,
        token_list_add_token(&g_tokens, token));       /* Act */

    TEST_ASSERT_EQUAL_PTR(token, g_tokens.head);       /* Assert */
}
```

실제 `test_token.c`는 heap token의 소유권과 정리를 안전하게 다루기 위해
`setUp()`과 `tearDown()` fixture를 사용하지만, 읽는 순서는 같다.

좋은 출발점은 다음과 같다.

- 테스트 이름만 읽어도 입력과 기대 행동을 알 수 있다.
- 한 테스트는 한 가지 실패 이유에 집중한다.
- private 함수가 아니라 `token.h`의 공개 동작을 호출한다.
- 테스트끼리 상태를 공유하지 않고 어떤 순서로 실행해도 같은 결과가 난다.
- 성공 경로뿐 아니라 경계값과 잘못된 입력도 하나씩 추가한다.

## 테스트가 실패했을 때 읽는 순서

Unity 실패 메시지를 보면 먼저 다음 세 가지를 확인한다.

1. 어떤 `test_...` 함수가 실패했는가?
2. 어느 assertion 줄에서 실패했는가?
3. expected와 actual이 각각 무엇인가?

그다음 바로 구현부터 고치지 말고 테스트의 기대가 정말 요구사항과 맞는지 확인한다.
테스트가 틀렸다면 테스트를 고치고, 요구사항이 맞다면 구현을 최소한으로 고친다.

## 버그를 발견했을 때

버그 수정도 같은 주기를 쓴다.

1. 버그를 가장 작은 입력으로 재현하는 테스트를 추가한다.
2. 수정 전 테스트가 실패하는지 확인한다.
3. 구현을 수정한다.
4. 새 테스트와 전체 회귀 테스트가 통과하는지 확인한다.

이렇게 남은 테스트는 같은 버그가 다시 들어오는 것을 막는 회귀 테스트가 된다.
