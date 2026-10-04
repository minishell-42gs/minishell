# Minishell Executor Design

## 현재 구현 범위

실행기는 단독 builtin을 부모에서 실행하고, 외부 명령과 pipeline은 자식 프로세스로
실행한다. pipeline FD 연결 후 각 명령의 redirection을 입력 순서로 적용한다.
따옴표·변수 확장, 7개 builtin, 입력/출력/append redirection, heredoc, signal 처리는
각각 [파싱·확장](Parsing-Expansion-Design.md), [builtin·환경](Builtins-Environment-Design.md),
[redirection·heredoc](Redirection-Heredoc-Design.md), [signal](Signals-Design.md)
설계 문서에 정리했다. io_mgr는 N-1개 파이프를 미리 만들므로 사용 가능한 FD 수의
제한을 받는다. 검증 명령은 [필수 검증](Mandatory-Validation.md)을 참조한다.

## 1. 개요 및 설계 목표

실행부(`executor`)의 핵심 목표는 파싱 결과물인 명령어 목록(`t_cmd_list`)을 전달받아,
쉘 내장 명령어(Builtin) 및 외부 명령어(External Command), 그리고 파이프라인(`|`)을 
**단일 책임 원칙(SRP)**과 **객체지향적 위계(OOP)**에 따라 안전하고 일관되게 실행하는 것이다.

### 주요 설계 원칙
1. **단일 책임 원칙 (Single Responsibility Principle)**:
   * 프로세스 생명주기 관리(`proc_mgr`), 명령별 입출력 연결과 파이프 FD 자원 관리(`io_mgr`), 빌트인 실행(`built_in`)의 책임을 분리한다. 외부 명령 실행은 `proc_mgr` 내부 자식 처리 메서드가 담당한다.
2. **부모 쉘과 자식 프로세스의 명확한 분리**:
   * 부모 쉘의 상태를 변경해야 하는 **단독 빌트인(`cd`, `exit`, `export` 등)**은 부모 프로세스에서 `fork()` 없이 즉시 실행한다.
   * 출력을 스트리밍해야 하는 **파이프라인 및 외부 명령어**는 메인 쉘이 직접 자식 프로세스를 `fork()`하여 관리한다.
3. **자원 누수 및 좀비 프로세스 방지**:
   * 파이프 FDs는 부모와 자식 모두에서 책임지고 닫혀야 EOF가 정상 감지되며 누수가 발생하지 않는다.
   * 생성된 모든 자식 프로세스는 `waitpid()`를 통해 빠짐없이 회수되며, 파이프라인의 최종 종료 코드는 **마지막 명령어의 종료 상태**를 따른다.

---

## 2. 아키텍처 및 클래스 다이어그램

```mermaid
classDiagram
    class t_executor {
        +env_list
        +built_in
        +init()
        +run()
        +destroy()
    }

    class t_built_in {
        +init()
        +is_built_in()
        +run()
        +destroy()
    }

    class t_proc_mgr {
        -cmd_list
        -cmd_count
        -pids
        -env_list
        -built_in
        -io_mgr
        +init()
        +run()
        -execute_child()
        +destroy()
    }

    class t_io_mgr {
        -pipes
        -pipe_count
        +init()
        +get_fds()
        +close_all()
        +destroy()
    }

    class t_cmd {
        +argv
        +redirs
        +apply_redirs()
        +destroy()
    }

    t_executor *-- t_built_in : owns
    t_executor ..> t_proc_mgr : creates per execution

    t_proc_mgr *-- t_io_mgr : owns
    t_proc_mgr ..> t_built_in : borrows in child
    t_proc_mgr ..> t_cmd : borrows from cmd_list
```

### 위계 구조 (Facade Pattern)

```text
                      [ t_executor ] (최상위 실행 Facade)
                            │
             ├──────────────────────┐
             ▼                      ▼
       [ t_built_in ]          [ t_proc_mgr ]
   (executor가 소유)       (실행마다 일시 생성)
   - env_list를 실행 시 주입  - env 스냅샷, pid, pipe 자원 소유
                                      │
                                      ▼
                               [ t_io_mgr ]
                        (입출력 연결·파이프 자원 관리자)
                        - pipe 0개도 포함해 항상 존재
                        - pipes fd 배열은 필요할 때만 소유
```

---

## 3. 컴포넌트별 상세 역할 및 책임

| 컴포넌트 | 책임 (Single Responsibility) | 실행 컨텍스트 |
| :--- | :--- | :--- |
| **`t_executor`** | 최상위 실행 파사드. 명령어를 분석하여 **단독 빌트인** vs **프로세스 관리자(`t_proc_mgr`)**로 실행 분기 위임 | 메인 쉘 (부모) |
| **`t_built_in`** | `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`의 내장 로직 실행. `run()` 호출 시 받은 `env_list`만 읽거나 갱신하며, 환경을 소유하거나 보관하지 않음 | 부모 쉘 or 자식 프로세스 |
| **`t_proc_mgr`** | 환경 스냅샷(`t_env_list`)·PID 배열·`t_io_mgr`를 소유하고, `cmd_list`와 `built_in`은 빌려서 자식 프로세스 `fork()`, 파이프 입출력 연결, 외부 명령 실행, `waitpid()` 및 종료 코드 회수를 담당 | 메인 쉘 (부모) |
| **`t_io_mgr`** | 모든 명령 개수에서 명령별 입출력 FD를 배정한다. 다중 명령어($N > 1$)면 $N-1$개의 파이프를 생성·회수하고, 단일 명령어면 pipe 0개와 `(-1, -1)` FD 배정을 제공한다 | `t_proc_mgr` 내부 |

---

### 환경 컨텍스트와 소유권

- `app`이 원본 `t_env_list`를 소유하고, `executor`는 이를 빌린다.
- `executor`는 `t_built_in`을 값 멤버로 소유하지만, `t_built_in`은 `env_list`를 멤버로 보관하지 않는다.
- 단독 빌트인은 `executor.env_list`를 `run()` 인자로 받아 부모 쉘의 환경을 변경한다.
- 외부 명령 또는 파이프라인에서는 `proc_mgr`가 실행 시작 시점의 환경을 자신의 값 멤버 `t_env_list`로 복제하고, `io_mgr`도 값 멤버로 소유한다. 각 child의 빌트인은 이 스냅샷을 인자로 받고, 변경은 해당 child에만 남는다.

따라서 pipeline 실행 중 `t_built_in` 객체의 멤버를 다른 환경으로 교체할 필요가 없으며,
어떤 환경을 읽거나 변경하는지 각 `run()` 호출부에서 드러난다.

---

## 4. $N+1$ 스트림 채널 (Stream Boundaries)

$N$개의 명령어가 파이프로 연결되어 있을 때, 입출력 경계는 개념적으로 항상 **$N+1$개**가 존재한다:

```text
[Channel 0]  ───> [ Cmd 0 ] ───> [Channel 1] ───> [ Cmd 1 ] ───> ... ───> [Channel N]
(STDIN / HD)                      (Pipe 0)                                  (STDOUT)
```

* **채널 연결 규칙**: $i$번째 명령어($0 \le i < N$)는 항상 **`Channel i`에서 읽고, `Channel i + 1`로 쓴다.**
  * $i = 0$: `in_fd = STDIN_FILENO`, `out_fd = pipe[0][1]`
  * $0 < i < N - 1$: `in_fd = pipe[i-1][0]`, `out_fd = pipe[i][1]`
  * $i = N - 1$: `in_fd = pipe[N-2][0]`, `out_fd = STDOUT_FILENO`
  * $N = 1$ (단일 명령): `in_fd = STDIN_FILENO`, `out_fd = STDOUT_FILENO` (파이프 0개)
* **파이프와 redirection 결합**:
  * Pipeline FD 연결 이후 redirection을 입력 순서대로 적용한다. 명령별 redirection이 기본 파이프 채널을 대체하며, 앞선 redirection의 파일 부수 효과는 유지된다.

---

## 5. 실행 시나리오 흐름

### 시나리오 1: 단독 빌트인 또는 redirection-only 명령 ($N = 1$)
1. t_executor가 단독 명령인지 확인한다. argv가 비어 있고 redirection이 있는 명령도 부모 경로로 보낸다.
2. 부모 경로가 stdin/stdout을 저장하고 redirection을 입력 순서대로 적용한다.
3. argv가 있으면 builtin을 실행해 환경/작업 디렉터리 변경을 보존한다. > file처럼 argv가 없으면 파일 작업만 수행한다.
4. 실행이나 redirection이 실패해도 저장해둔 stdin/stdout을 복원하고 종료 상태를 반환한다.

### 시나리오 2: 단일 외부 명령어 (`ls -la`, $N = 1$)
1. `t_executor`가 `t_proc_mgr`에 실행 위임.
2. `t_proc_mgr`가 항상 소유하는 `t_io_mgr`는 pipe 0개로 초기화되고, `get_fds(0)`은 `in_fd = -1`, `out_fd = -1`을 반환한다.
3. 원래 표준 입출력을 유지한 채 1회 `fork()`.
4. 자식 프로세스: `exec_external_child()`를 통해 `execve` 실행.
5. 부모 프로세스: 자식 1개를 `waitpid()`하고 종료 코드 반환.

### 시나리오 3: 다중 파이프라인 (`cat /etc/passwd | grep a | wc -l`, $N = 3$)
1. `t_executor`가 `t_proc_mgr`에 실행 위임.
2. `t_proc_mgr`가 소유한 `t_io_mgr`가 파이프 2개를 연다.
3. 3개 명령어에 대해 순차적으로 `io_mgr.get_fds(i, &in_fd, &out_fd)` 호출 후 `fork()`:
   * 자식 0: `in_fd = STDIN`, `out_fd = pipe[0][1]`
   * 자식 1: `in_fd = pipe[0][0]`, `out_fd = pipe[1][1]`
   * 자식 2: `in_fd = pipe[1][0]`, `out_fd = STDOUT`
   * 각 자식 프로세스는 `dup2`로 입출력을 연결하고, `io_mgr.close_all()` 호출 후 명령 실행.
4. 부모 프로세스: 모든 자식이 fork된 후 `io_mgr.close_all()`을 호출하여 파이프 FDs 회수 (EOF 전파).
5. 부모 프로세스: 모든 자식을 `waitpid()`하고, 마지막 명령어(`wc -l`)의 종료 코드를 최종 상태로 반환.
6. `io_mgr.destroy()` 및 `proc_mgr.destroy()`로 일회성 자원 메모리 해제.

### 시나리오 4: 파이프라인 속 빌트인 (`echo alpha | cat | wc -c`)
1. pipeline의 모든 명령을 자식 프로세스로 생성하고 stdin/stdout을 `dup2`로 연결한다.
2. 첫 자식에서 `echo` builtin을 감지하고, 해당 명령의 확장 argv를 실행한다.
3. `echo`의 stdout은 앞서 연결한 pipe이므로 텍스트가 `cat`을 거쳐 `wc`에 전달된다.
4. 각 자식은 자신의 종료 상태로 종료하고, 부모는 마지막 명령의 상태를 pipeline 결과로 반환한다.

---

## 6. 소스 코드 파일 구성

| 파일 경로 | 소속 컴포넌트 | 주요 역할 |
| :--- | :--- | :--- |
| `include/executor.h` | Public Header | `t_executor` 및 외부 실행 API 선언 |
| `include/built_in.h` | Public Header | `t_built_in` 및 빌트인 실행 API 선언 |
| `include/proc_mgr.h` | Public Header | `t_proc_mgr` 및 프로세스 실행 API 선언 |
| `include/io_mgr.h` | Public Header | `t_io_mgr` 및 명령별 입출력 연결 API 선언 |
| `src/executor/executor.c` | `t_executor` | 최상위 실행 엔트리포인트 및 실행 분기 |
| `src/built_in/built_in.c` | `t_built_in` | 빌트인 판별·실행 인터페이스와 생명주기 |
| `src/proc_mgr/proc_mgr.c` | `t_proc_mgr` | 프로세스 매니저 생성·소멸 및 `fork()` 흐름 |
| `src/proc_mgr/proc_mgr_impl.c` | `t_proc_mgr` 내부 구현 | fork, 자식 FD 연결 및 시그널 초기화 |
| `src/proc_mgr/proc_mgr_wait.c` | `t_proc_mgr` 내부 구현 | waitpid 회수, 종료 코드 변환 및 실패 복구 |
| `src/proc_mgr/proc_mgr_exec_external.c` | `t_proc_mgr` 내부 구현 | 외부 명령 경로 탐색 및 `execve()` 실행 |
| `src/io_mgr/io_mgr.c` | `t_io_mgr` | 명령별 FD 배분, 파이프 생성 및 전체 FD 회수 |


## 7. 실패 처리와 검증

- 모든 명령을 fork한 뒤에 대기하여 파이프 버퍼보다 큰 출력도 전달한다.
- 자식은 SIGINT/SIGQUIT/SIGPIPE를 기본 동작으로 되돌리고, dup2 후 원본 파이프
  FD를 `close_all()`에서 한 번씩 닫는다. 부모도 대기 전에 모든 파이프 FD를 닫는다.
- waitpid가 EINTR이면 같은 PID로 재시도한다. 모든 자식을 회수한 뒤 마지막
  명령의 종료 코드(시그널 종료는 128 + signal)를 반환한다.
- fork 도중 실패하면 열린 파이프를 닫고 생성된 자식에 SIGKILL을 보내 회수한다.
  자식이 입력이나 긴 작업을 기다리는 경우에도 정리가 멈추지 않도록 한다.
- pipe/fork/dup2/waitpid 오류는 공통 error facade로 보고한다. 부모 측 실패는
  FAIL을 반환하며 호출자의 종료 상태 out 파라미터를 보존한다.

`make -C Tests proc_mgr`는 GNU linker의 `--wrap`으로 시스템 호출 실패와 EINTR을
주입한다. 부분 fork/pipe 실패, dup2 실패, 시그널 종료, 반복 실행의 FD 수와
자식 회수를 검증한다. 이 옵션은 테스트 바이너리에만 적용된다.
`make -C Tests integration`은 실제 셸의 출력과 종료 코드를 Bash와 비교하며,
대용량 출력과 조기 종료를 포함한다. 각 셸 실행은 GNU `timeout`으로 제한한다.
