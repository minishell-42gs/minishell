# Minishell Error Handling

## 목표

오류는 lexer, parser, expansion, executor, builtin, heredoc 등 여러 모듈에서
발견된다. 그러나 사용자가 보는 메시지 형식과 종료 상태 규칙이 모듈마다 다르면
쉘의 동작이 불안정해지고 테스트도 어려워진다.

따라서 각 모듈은 오류를 **발견하고 요청을 구성**하며, `error_report()`는 다음 두
가지만 수행한다.

1. `STDERR_FILENO`에 정해진 형식의 메시지를 쓴다.
2. 요청에 포함된 `exit_code`를 `*status`에 기록한다.

메모리 해제, fd 정리, `exit()`, 다음 프롬프트로 진행, `running` 값 변경은
**호출자의 책임**이다. 이 분리는 부모 프로세스에서 복구할 오류와 자식 프로세스가
종료해야 하는 오류를 같은 출력 API로 다룰 수 있게 한다.

```text
parser / expansion / executor / builtin / heredoc
                    │
                    │ t_error_req를 만든다
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

## 오류 종류와 사용 위치

| 종류 | 사용 위치 | 출력 예시 | 일반적인 상태 |
|---|---|---|---:|
| `ERR_SYNTAX` | lexer/parser | `minishell: syntax error near unexpected token \`|'` | `2` |
| `ERR_CMD_NOT_FOUND` | 명령 경로 탐색 | `minishell: abc: command not found` | `127` |
| `ERR_ERRNO` | `open`, `pipe`, `fork`, `dup2`, `execve` 등 | `minishell: file: Permission denied` | `1` 또는 `126` |
| `ERR_BUILTIN` | builtin의 인자·의미 검사 | `minishell: cd: No such file or directory` | `1` 또는 `2` |
| `ERR_AMBIGUOUS_REDIR` | 확장 뒤 리다이렉션 target 검사 | `minishell: $FILE: ambiguous redirect` | `1` |
| `ERR_HEREDOC_EOF` | delimiter 전에 입력 EOF | heredoc EOF warning | `0` (임시값) |

### `ERR_SYNTAX`

인식 가능한 minishell 문법이 잘못되었을 때 사용한다. 예를 들면 `| ls`, `echo |`,
`cat >`, `cat <<`다. 오류 토큰을 전달하고 상태는 항상 `2`로 지정한다.

```c
req.type = ERR_SYNTAX;
req.exit_code = 2;
req.u_data.s_syntax.token = "newline";
error_report(&app->last_status, &req);
```

닫히지 않은 quote, `;`, `\\`, `&&`, `||`는 과제 필수 범위에서 해석하지 않으므로,
그 자체로 `ERR_SYNTAX`를 보고하지 않는다.

### `ERR_CMD_NOT_FOUND`

명령명이 `PATH`와 상대/절대 경로에서 존재하지 않을 때만 사용한다. 상태는 `127`이다.
`Permission denied`, `Is a directory`, `Exec format error`처럼 실행 대상을 찾았지만
실행하지 못한 경우는 `ERR_ERRNO`와 상태 `126`을 사용한다.

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
`execve()` 실패가 이 경로를 공유한다. `execve()` 실패만 보통 `126`을 전달한다.

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

## 구현 체크리스트

- [ ] lexer가 `<`, `>`, `<<`, `>>`, `|`를 quote 밖에서 분리한다.
- [ ] parser가 누락된 명령 또는 redirection target에 `ERR_SYNTAX`와 `2`를 사용한다.
- [ ] expansion이 quote 문맥을 보존하고 `$VAR`, `$?`를 처리한다.
- [ ] redirection의 `open`/`dup2` 실패는 `saved_errno`를 보존해 `ERR_ERRNO`로 보고한다.
- [ ] command resolver가 찾지 못함(`127`)과 실행 불가(`126`)를 구분한다.
- [ ] 부모 builtin과 pipeline 내부 자식 builtin의 상태·환경 변경 범위를 구분한다.
- [ ] heredoc의 EOF warning과 `Ctrl-C` 취소를 구분한다.
- [ ] 오류 출력 뒤에도 모든 pipe fd, redirection fd, 임시 heredoc fd, 할당 메모리를 정리한다.

## Wiki 등록

이 파일은 Wiki 페이지명 규칙에 맞춘 `Error-Handling.md`다. Wiki 저장소를 clone한
뒤 루트에 같은 이름으로 복사해 commit/push하면 `Error Handling` 페이지가 생성된다.

```sh
git clone https://github.com/minishell-42gs/minishell.wiki.git
cp docs/wiki/Error-Handling.md minishell.wiki/Error-Handling.md
cd minishell.wiki
git add Error-Handling.md
git commit -m "docs: add error handling design"
git push
```
