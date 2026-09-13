# Minishell Error Handling

## 목표

오류는 lexer, parser, expansion, executor, builtin, heredoc 등 여러 모듈에서
발견된다. 그러나 사용자가 보는 메시지 형식과 종료 상태 규칙이 모듈마다 다르면
쉘의 동작이 불안정해지고 테스트도 어려워진다.

따라서 하위 모듈은 오류를 **발견하고 필요한 정보**를 반환한다. 경계 역할을 하는
facade 또는 호출자는 이를 `t_error_req`로 구성한다. `error_report()`는 다음 두 가지만
수행한다.

1. `STDERR_FILENO`에 정해진 형식의 메시지를 쓴다.
2. 요청에 포함된 `exit_code`를 `*status`에 기록한다.

메모리 해제, fd 정리, `exit()`, 다음 프롬프트로 진행, `running` 값 변경은
**호출자의 책임**이다. 이 분리는 부모 프로세스에서 복구할 오류와 자식 프로세스가
종료해야 하는 오류를 같은 출력 API로 다룰 수 있게 한다.

```text
lexer ── syntax token ──> parsing_facade ──> t_parse_outcome
                                                │
executor / builtin / redirection / heredoc ─────┤
                                                │ t_error_req
                                                ▼
                                         error_report()
                                         ├─ stderr 출력
                                         └─ *status 갱신
                                                │
                                                ▼
                        호출자가 cleanup, return, continue, exit를 결정
```

## 공개 API

외부 모듈이 사용하는 함수는 하나다.

```c
void    error_report(int *status, const t_error_req *req);
```

`status`는 실제 종료 상태를 가진 정수의 주소다. 부모에서는 보통
`&app->last_status`를 전달한다. 자식에서는 자식 전용 지역 변수의 주소를 전달한 뒤,
정리 후 그 값을 `exit()`에 사용한다.

```c
int         child_status;
t_error_req req;

child_status = 1;
req.type = ERR_ERRNO;
req.exit_code = 126;
req.u_data.s_sys.name = cmd->argv[0];
req.u_data.s_sys.saved_errno = saved_errno;
error_report(&child_status, &req);
free_child_resources();
exit(child_status);
```

`error_report()`는 `req == NULL`이면 아무 일도 하지 않는다. `status == NULL`이면
메시지만 출력한다. 정상적인 호출에서는 둘 다 유효해야 한다.

## 파싱 결과 객체: t_parse_outcome

파싱은 단순 성공/실패 외에, 셸을 계속 실행할 수 있는 문법 오류와 종료해야 하는
내부 실패를 구분해야 한다. 그래서 `t_status`를 확장하지 않고 파싱 전용 값 객체를
사용한다.

~~~c
typedef struct s_parse_outcome
{
    t_parse_result result;
    bool           has_error_req;
    t_error_req    error;
}   t_parse_outcome;
~~~

`has_error_req`는 **오류 발생 여부**가 아니라 `error`가 `error_report()`에 넘길 수
있는 유효한 요청인지를 뜻한다.

~~~c
outcome = parsing_facade_parse(&app->parsing_facade, line, &cmd_list);
if (outcome.has_error_req)
    error_report(&app->last_status, &outcome.error);
if (outcome.result == PARSE_SYNTAX_ERROR)
    continue_loop();
if (outcome.result == PARSE_FATAL_ERROR)
    stop_shell();
~~~

현재 문법 오류는 `PARSE_SYNTAX_ERROR`와 `ERR_SYNTAX`, 상태 `2`를 함께 반환한다.
내부 실패는 `PARSE_FATAL_ERROR`와 `ERR_INTERNAL`, 상태 `1`을 반환한다. app은
fatal outcome에서만 루프를 끝낸다.

## error 모듈의 내부 구성

외부 공개 API는 `error_report()` 하나로 유지한다. 출력 구현은 의미별 파일로
분리한다. 구현 파일 사이에서 필요한 출력 함수 선언은 `include/error.h`의
`/* internal */` 영역에 둔다. 다른 모듈은 그 함수를 직접 호출하지 않고
`error_report()`만 사용한다.

~~~text
src/error/
├── error.c             # dispatch, status 갱신
├── error_syntax.c      # parser/lexer syntax
├── error_command.c     # command not found
├── error_system.c      # open, dup2, execve 등의 errno 형식
├── error_builtin.c     # builtin 의미·인자 오류
├── error_redirection.c # ambiguous redirect, heredoc EOF warning
└── error_internal.c    # 복구 불가능한 내부 실패
~~~

`ERR_AMBIGUOUS_REDIR`와 `ERR_HEREDOC_EOF`는 둘 다 redirection 계열이므로 같은
구현 파일에 둔다. 다만 전자는 명령 실행을 막는 오류이고 후자는 delimiter 없이 EOF를
만난 경고이므로, 서로 다른 출력 함수와 상태 정책을 유지한다. 반대로 `ERR_ERRNO`는
redirection뿐 아니라 `pipe`, `fork`, `execve`에도 쓰이는 시스템 콜 형식이라 범용
`error_system.c`에 둔다.

`error_report()`는 정리, fd 복구, `exit()`를 수행하지 않는다. 예를 들어
`exec_child()`는 보고 후 path를 해제하고 자식만 `exit()`한다.

## 오류 종류와 사용 위치

| 종류 | 사용 위치 | 출력 예시 | 일반적인 상태 |
|---|---|---|---:|
| `ERR_SYNTAX` | lexer/parser | `minishell: syntax error near unexpected token \`|'` | `2` |
| `ERR_CMD_NOT_FOUND` | 명령 경로 탐색 | `minishell: abc: command not found` | `127` |
| `ERR_ERRNO` | `open`, `pipe`, `fork`, `dup2`, `execve` 등 | `minishell: file: Permission denied` | `1`, `126`, 또는 `127` |
| `ERR_BUILTIN` | builtin의 인자·의미 검사 | `minishell: cd: No such file or directory` | `1` 또는 `2` |
| `ERR_AMBIGUOUS_REDIR` | 확장 뒤 리다이렉션 target 검사 | `minishell: $FILE: ambiguous redirect` | `1` |
| `ERR_HEREDOC_EOF` | delimiter 전에 입력 EOF | heredoc EOF warning | `0` (임시값) |
| `ERR_INTERNAL` | allocation·parser 등 복구 불가능한 내부 실패 | `minishell: internal error: parser failed` | `1` |

### `ERR_SYNTAX`

인식 가능한 minishell 문법이 잘못되었을 때 사용한다. 예를 들면 `| ls`, `echo |`,
`cat >`, `cat <<`다. 오류 토큰을 전달하고 상태는 항상 `2`로 지정한다.

```c
req.type = ERR_SYNTAX;
req.exit_code = 2;
req.u_data.s_syntax.token = "newline";
error_report(&app->last_status, &req);
```

현재 lexer는 닫히지 않은 quote, 선행/연속/마지막 단일 pipe를 문법 오류로 보고한다.
`;`, `\\`, `&&`, `||`는 과제 필수 범위에서 해석하지 않으므로 일반 문자열로 둔다.

### `ERR_CMD_NOT_FOUND`

`PATH`에서 명령을 찾지 못했을 때만 사용한다. 상태는 `127`이다. 직접 지정한 경로는
실행할 수 없거나 존재하지 않아도 `execve()`까지 전달해 실제 errno를 보존한다.
`Permission denied`, `Is a directory`, `Exec format error`처럼 실행 대상을 찾았지만
실행하지 못한 경우는 `ERR_ERRNO`와 상태 `126`을 사용한다. 직접 경로의 `ENOENT` 또는
`ENOTDIR`은 `ERR_ERRNO` 메시지와 상태 `127`을 사용한다.

```c
req.type = ERR_CMD_NOT_FOUND;
req.exit_code = 127;
req.u_data.s_cmd_not_found.cmd = cmd->argv[0];
error_report(&child_status, &req);
```

### `ERR_ERRNO`

시스템 콜 실패에 사용한다. `errno`는 이후 호출로 바뀔 수 있으므로 **실패한 즉시**
저장한 값을 전달해야 한다. `error_report()` 내부는 전역 `errno`를 읽지 않는다.

```c
saved_errno = errno;
req.type = ERR_ERRNO;
req.exit_code = 1;
req.u_data.s_sys.name = redir->target;
req.u_data.s_sys.saved_errno = saved_errno;
error_report(&app->last_status, &req);
```

리다이렉션 `open()` 실패, pipeline을 위한 `pipe()`/`fork()`/`dup2()` 실패, 그리고
`execve()` 실패가 이 경로를 공유한다. `execve()`는 보통 `126`을 전달하지만, 직접
경로의 `ENOENT` 또는 `ENOTDIR`은 `127`을 전달한다.

### `ERR_BUILTIN`

시스템 콜 자체의 실패보다 builtin 명령의 인자나 의미가 잘못된 경우에 사용한다.
`name`은 builtin 이름, `detail`은 오류 설명이다.

```c
req.type = ERR_BUILTIN;
req.exit_code = 1;
req.u_data.s_builtin.name = "export";
req.u_data.s_builtin.detail = "not a valid identifier";
error_report(&app->last_status, &req);
```

예시 상태는 `cd` 실패 `1`, `exit`의 숫자가 아닌 인수 `2`, `exit`의 인수가 너무 많은
경우 `1`이다. `exit`의 숫자가 아닌 인수는 보고 뒤 실제로 셸을 종료해야 하지만,
그 종료 결정은 Facade가 아닌 builtin 호출자가 맡는다.

### `ERR_AMBIGUOUS_REDIR`

리다이렉션 target을 확장한 결과가 유효한 단일 파일명이 아닐 때 사용한다. 이 오류는
`open()` 전에 발생하므로 `ERR_ERRNO`가 아니다.

```c
req.type = ERR_AMBIGUOUS_REDIR;
req.exit_code = 1;
req.u_data.s_ambiguous_redir.target = original_target;
error_report(status, &req);
```

### `ERR_HEREDOC_EOF`

heredoc 입력 중 `Ctrl-D`로 EOF가 들어왔지만 delimiter를 만나지 못한 경우의
**경고**다. 일반 오류와 달리 heredoc은 현재까지 받은 입력을 EOF로 끝내고 명령을
계속 실행한다. 따라서 `exit_code`에는 임시로 `0`을 넣고, 최종 pipeline 종료 상태가
나중에 이를 덮어쓴다.

```c
req.type = ERR_HEREDOC_EOF;
req.exit_code = 0;
req.u_data.s_heredoc_eof.delimiter = delimiter;
error_report(status, &req);
```

`Ctrl-C`로 heredoc을 취소한 상황은 EOF 경고가 아니다. heredoc 수집을 중단하고
상태 `130`을 남기며 명령 실행 자체를 건너뛴다.

### `ERR_INTERNAL`

사용자 입력 자체가 아니라 할당 실패 또는 parser/lexer 내부 실패처럼 현재 명령을
계속 처리할 수 없는 상황에 사용한다. app은 이 요청을 보고한 뒤 실행 루프를 종료한다.

~~~c
outcome.result = PARSE_FATAL_ERROR;
outcome.has_error_req = true;
outcome.error = (t_error_req){ERR_INTERNAL, 1,
    {.s_internal = {"parser failed"}}};
~~~

`ERR_INTERNAL`의 message는 디버깅 보조용이다. 사용자 입력의 문법 오류처럼 가장하면
안 된다.

## 프로세스별 상태 소유권

| 상황 | `status`로 넘길 주소 | 이후 책임 |
|---|---|---|
| parser/expansion/redirection 준비 실패 | 부모의 `&app->last_status` | fd·명령 자료구조 정리 후 다음 프롬프트 |
| 단독 builtin | 부모의 `&app->last_status` | 필요한 경우 환경 변경 후 다음 프롬프트 또는 종료 |
| pipeline 안의 external command | 자식의 `child_status` | 자식 fd/메모리 정리 후 `exit(child_status)` |
| pipeline 안의 builtin | 자식의 `child_status` | 자식에서만 실행하고 `exit(child_status)` |
| heredoc 수집 child | 수집 결과를 부모가 해석 | parent가 `130` 또는 이후 pipeline 상태를 결정 |

pipeline의 최종 `app->last_status`는 마지막 명령의 wait 상태여야 한다. 중간 자식의
`error_report()` 호출은 해당 자식의 종료 코드만 정하고, 부모의 최종 상태를 직접
결정하지 않는다.

## 환경 데이터 의존성

parsing facade는 초기 `char **envp` 복사본을 보관하지 않고 app이 소유한
`t_env_list *`를 참조한다. executor도 같은 `env_list`에서 실행 시점의 `envp`를
생성한다.

이렇게 하면 이후 `export` 또는 `unset`으로 환경이 바뀐 뒤에도 expansion 단계가
최신 환경을 조회할 수 있다. facade는 `env_list`를 소유하거나 해제하지 않는다.

## Signal과 Facade의 경계

signal handler 안에서는 `error_report()`를 호출하지 않는다. 이 함수는 `strerror()`,
`ft_strlen()` 등 async-signal-safe가 아닌 경로를 사용할 수 있다. handler는 허용된
전역 `sig_atomic_t` 하나에 signal 번호만 저장한다.

일반 프롬프트, 실행 중인 자식, heredoc 수집 중의 signal 처리는 signal 모듈이
상태별로 담당한다.

- interactive `Ctrl-C`: 새 줄과 새 프롬프트, 상태 `130`
- interactive `Ctrl-\\`: 아무 출력·동작 없음
- heredoc `Ctrl-C`: 수집 취소, 상태 `130`, pipeline 미실행
- 자식의 signal 종료: 부모가 `waitpid()` 결과를 `128 + signal`로 변환

필요한 signal 안내 문구는 signal/wait 처리 코드에서 출력한다. 이는 일반 명령 오류가
아니며 `error_report()`의 책임으로 섞지 않는다.
