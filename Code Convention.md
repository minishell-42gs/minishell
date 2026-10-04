
# 요약

| 요소           | 규칙                       | 예시               |
| ------------- | ------------------------- | ----------------- |
| `types.h`     | 공용 타입을 정의한다.                                                           | typedef long long t_ll;  <br> typedef enum e_status { OK, FAIL } t_status; |
| `struct` = 객체 | 데이터 필드 + 함수 포인터 (메서드)                                       <br> 단, Norminette 규칙 위반이 발생하면 `반환형 객체이름_메서드(인자)` 형태로 선언하고 선언 위에 주석으로 이유를 남긴다.    | `t_status (*run)(t_cmd_mgr *this); ` <br> t_error app_run(int argc, char **argv)                                             |
| `this` 파라미터   | 모든 메서드의 첫 번째 인자는 `this`(자기 자신)                                 | `run_impl(t_cmd *this)`                                                         |
| 생성자           | `xxx_init(this, ...)` 형태. 함수 포인터 연결 및 필드 초기화, `t_status` 반환   | `cmd_init(...)`                                                                 |
| 소멸자           | `destroy` 함수 포인터를 통해 객체의 생명주기 관리                              | `this->destroy(this);`                                                          |
| 구현 함수         | `static` + `_impl` 접미사 사용, 생성자에서 함수 포인터에 할당                   | `run_impl`, `app_run_impl`                                                      |
| 에러 처리         | 중앙 `report_error()` + `ERR_*` enum 사용, 필요 시 콤마 연산자로 정리        | `return (free_split(x), report_error(...));`                                    |
| 파일 배치         | `include/`(헤더), `src/`(구현), `util/`(헬퍼). 큰 모듈은 `_impl` 파일로 분리 <br> 모듈(객체)은 src 아래의 폴더로 구분한다. | include/ <br>- fd_factory.h <br> - parser.h<br>...<br>src/<br>- fd_factory/ <br>-- fd_factory_calc.c <br>-- fd_factory_impl.c <br> -- update_fd.c<br>-- ...<br> - parser/<br>-- parser.c <br> -- pre_parse.c                                     |
| 리턴 규약         | 성공/실패는 `t_status`(`OK`/`FAIL`)로 통일                            | `return (OK);` / `return (FAIL);`                                               |

# 세부 규칙

## 0. 전체 구조 — 매니저 패턴

`main`에는 로직을 두지 않는다. `t_app`을 만들고 `init → run → destroy`만 호출한다.
`t_app`이 하위 매니저(parser, executor, env_mgr, signal_mgr...)를 **소유 관계로 포함**하고,
하위 관리자도 같은 구조를 반복한다.

```c
int	main(int argc, char **argv, char **envp)
{
	t_app	app;

	ft_memset(&app, 0, sizeof(t_app));
	if (app_init(&app, argc, argv, envp) != OK)
		return (app.destroy(&app), 1);
	if (app.run(&app) != OK)
		return (app.destroy(&app), 1);
	app.destroy(&app);
	return (app.exit_code);
}
```

호출 관계는 항상 위에서 아래로 흐른다: `app.run` → `cmd_mgr.run` → `cmd.run`.
각 단계는 하위 객체의 메서드를 호출할 때 해당 객체의 `this`를 넘긴다.

---

## 1. `types.h` — 공용 타입 모음

모든 공용 열거형을 **이 파일 한 곳에만** 둔다.
각 헤더는 `struct s_xxx { ... }` **정의**만 갖고, `t_xxx` 별칭은 이 파일이 책임진다.
→ 헤더끼리 서로 참조해도 순환 포함이 발생하지 않는다.

```c
#ifndef TYPES_H
# define TYPES_H

typedef enum e_status
{
	OK,
	FAIL,
}				t_status;

#endif
```

---

## 2. `struct`와 객체 (데이터와 메서드)

구조체는 **데이터 필드와 함수 포인터(메서드)**로 구성한다.
구조체 정의는 헤더에, 별칭 `typedef`는 `types.h`에 둔다.

```c
struct			s_cmd_mgr
{
	t_cmd		*cmds;          /* --- 데이터 --- */
	int			cmd_num;
	int			exit_code;
	t_pipe_mgr	*pm;
	char		**envp;

	t_status	(*run)(t_cmd_mgr *this);     /* --- 메서드 --- */
	void		(*destroy)(t_cmd_mgr *this);
};

t_status	cmd_mgr_init(t_cmd_mgr *this, t_parsed *parsed,
				t_pipe_mgr *_pm, char **envp);
```

- 데이터 필드를 위에, 함수 포인터를 아래에 두고 빈 줄로 구분한다.
- 소유하지 않고 참조만 하는 포인터(`t_pipe_mgr *pm`)는 `destroy`에서 건드리지 않는다.

---

## 3. `this` 매개변수

모든 메서드의 첫 번째 인자는 `this`(자기 자신)다. 다른 객체를 호출할 때도 해당 객체를 넘긴다.

```c
static t_status	run_impl(t_cmd *this)
{
	this->pid = fork();
	if (this->pid == 0)
	{
		dup2(this->fd_in, STDIN_FILENO);
		this->pm->close_all_pipes(this->pm);   /* 다른 객체 호출: self 전달 */
		execve(this->path, this->argv, this->envp);
		...
	}
	return (OK);
}
```

---

## 4. 생성자·소멸자·구현 함수

**생성자 `xxx_init(this, ...)`**: 순서는 (1) 함수 포인터 배선 → (2) 필드 초기화 → (3) `t_status` 반환.

```c
t_status	cmd_init(t_cmd *this, int idx, const t_parsed *parsed, t_cmd_mgr *_mgr)
{
	this->run = run_impl;         /* 1) 배선 먼저 */
	this->destroy = destroy_impl;
	this->pm = _mgr->pm;          /* 2) 필드 초기화 */
	this->argv = ft_split(parsed->commands[idx], ' ');
	if (!this->argv)
		return (report_error("argv_split_failed", ERR_ARGV_SPLIT_FAILED));
	...
	return (OK);                  /* 3) 상태 반환 */
}
```

**구현 함수는 `static ..._impl`로 선언**해 파일 내부에 두고 함수 포인터를 통해서만 호출한다.
이름 충돌을 피하려면 접두사를 붙인다 (`app_run_impl`, `cmd_run_impl`).

**소멸자 `destroy`**: 자기가 만든 자원만 해제. 하위 객체는 NULL 가드 후 destroy 호출.

```c
static void	destroy_impl(t_app *this)
{
	if (this->cmd_mgr.destroy)
		this->cmd_mgr.destroy(&this->cmd_mgr);
	if (this->pipe_mgr.destroy)
		this->pipe_mgr.destroy(&this->pipe_mgr);
	if (this->parser.destroy)
		this->parser.destroy(&this->parser);
}
```

> `main`이 `ft_memset(&app, 0, ...)`로 0 초기화하므로, init 도중 실패해도
> destroy가 NULL 가드로 안전하게 부분 정리할 수 있다.

---

## 5. 오류 처리 — 중앙화와 콤마 연산자 정리

성공/실패는 `t_status`(OK/FAIL)로 반환. 에러는 중앙 `report_error()` + `ERR_*` enum으로.
정리(free) + 에러 반환을 **콤마 연산자 한 줄**로 묶는다.

```c
if (!this->argv[0])
	return (free_split(this->argv), report_error("", ERR_FIND_PATH_FAILED));
```

`report_error`는 메시지 출력 후 `FAIL`(또는 종료코드)을 반환하도록 설계한다.

> minishell은 종료상태(`$?`)가 핵심이다. pipex처럼 단순 FAIL이 아니라,
> `report_error`가 **종료코드까지 세팅/반환**하도록 확장할 것 (예: 명령 not found → 127).

---

## 6. 파일과 디렉터리 배치

```
include/     헤더 (구조체 정의와 xxx_init 원형 선언)
  types.h    전방 선언 별칭과 공용 열거형 모음
  parser.h
  lexer.h
src/         구현 (.c, 모듈마다 한 파일, 큰 모듈은 _impl 파일로 분리)
  parser/
	parser.c
	parse_impl.c
	...
  lexer/
	lexer.c
	lexer_impl.c
	...
util/        범용 헬퍼 (free_split, create_cmd_path 등)
libft/       libft (+ 필요 시 ft_printf, get_next_line)
```


---

## 7. Norminette 주의사항 (이 스타일의 함정)

함수 포인터 OOP는 코드가 늘어나 Norm과 부딪힌다.

- 함수 **25줄** / 파일당 **함수 5개** → `_impl` 파일 분리로 대응.
- `init`이 길어지기 쉬움 → 필드 초기화를 `setup_xxx` 헬퍼로 분리.
- 42 헤더 주석 필수, 4칸 탭, 80열.

> pipex보다 minishell이 훨씬 크다. 처음부터 모듈·함수를 잘게 쪼개는 습관으로 시작할 것.
