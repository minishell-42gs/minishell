# 수동 통합 체크리스트

`make -C Tests integration`은 표준 입력으로 명령을 넣어 검증하므로 터미널에서만 확인할 수 있는
동작은 다루지 못한다. 그런 항목은 여기서 직접 확인하고, `story`에서 `main`으로 보내는 병합 요청 본문에
이 표를 복사해 결과를 남긴다.

실행 명령: `make -C Assignments && ./Assignments/minishell`

## v1: 외부 명령 한 개 실행

| 입력 | 기대 | 확인 |
|---|---|---|
| `ls -a` | 목록이 출력되고 프롬프트 `minishell$ `가 다시 뜬다 | [ ] |
| `no_such_cmd` | `minishell: no_such_cmd: command not found`가 뜨고 프롬프트가 다시 뜬다 (셸이 죽지 않는다) | [ ] |
| 빈 줄 Enter, 공백만 Enter | 아무 출력 없이 프롬프트가 다시 뜬다 | [ ] |
| ↑ 화살표 | 직전에 입력한 명령이 나타난다 | [ ] |
| ↑ 를 여러 번 | 빈 줄·공백 줄은 히스토리에 없고, 실제 명령만 거슬러 올라간다 | [ ] |
| `/bin/false` 입력 후 Ctrl+D | 한 번에 종료된다. 바깥 셸에서 `echo $?`가 `1` | [ ] |
| `ls -a` 입력 후 Ctrl+D | 종료 후 바깥 셸에서 `echo $?`가 `0` | [ ] |

## v2: 시그널과 히스토리 (평가표 Signals / Go Crazy and history)

평가표 기준. `$?`는 다음 줄에 `echo $?`로 확인한다.

| 상황 | 키 | 기대 (bash 기준) | 확인 |
|---|---|---|---|
| 빈 프롬프트 | Ctrl+C | 새 줄에 새 프롬프트, `$?`=130 | [ ] |
| 빈 프롬프트 | Ctrl+\ | 아무 일 없음 | [ ] |
| 빈 프롬프트 | Ctrl+D | 셸 종료 (다시 실행) | [ ] |
| `abc` 입력 중 | Ctrl+C | 새 줄에 새 프롬프트. 이어서 Enter → 아무것도 실행되지 않음 (버퍼 비움) | [ ] |
| `abc` 입력 중 | Ctrl+D | 아무 일 없음 | [ ] |
| `abc` 입력 중 | Ctrl+\ | 아무 일 없음 | [ ] |
| `cat` 실행 중 | Ctrl+C | cat 종료, 새 프롬프트, `$?`=130 | [ ] |
| `cat` 실행 중 | Ctrl+\ | `Quit (core dumped)` 출력, `$?`=131 | [ ] |
| `cat` 실행 중 | Ctrl+D | cat 이 EOF 를 받고 정상 종료, 셸은 계속, `$?`=0 | [ ] |
| `grep something` 실행 중 | Ctrl+C / Ctrl+\ / Ctrl+D | 위 `cat` 과 동일 | [ ] |
| `cat \| cat \| ls` | Enter 후 Ctrl+D | ls 출력, 교착 없이 프롬프트 복귀 | [ ] |
| `cat << EOF` 입력 중 | Ctrl+C | heredoc 취소, 새 프롬프트, 명령 실행 안 됨, `$?`=130 | [ ] |
| `cat << EOF` 입력 중 | Ctrl+D | 경고 출력 후 수집된 본문으로 실행 | [ ] |
| heredoc 본문 입력 후 | ↑ | heredoc 본문 줄이 히스토리에 **없음** | [ ] |
| 명령 몇 개 실행 후 | ↑ ↓ | 이전 명령 탐색, Enter 로 재실행 가능 | [ ] |
| `dsbksdgbksdghsd` | Enter | `command not found`, 셸은 계속 | [ ] |

## 평가 전 메모리 확인

제출 바이너리를 valgrind 로 돌린다. readline 내부 누수는 과제가 허용하므로
`readline.supp` 로 걸러내고, **우리 코드의 `definitely lost` 가 0** 인지 본다.

```sh
printf 'ls -a\nexport A=1\necho $A | cat\ncat << EOF\nhi\nEOF\ncd /tmp\npwd\nexit 3\n' | \
  valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes \
           --suppressions=Tests/integration/readline.supp ./Assignments/minishell
```

- 부모와 자식 모두 `definitely lost: 0 bytes`, `indirectly lost: 0 bytes`
- `--track-fds=yes` 출력에서 열린 fd 가 0/1/2 뿐
- 파이프라인과 heredoc 경로를 꼭 포함한다 (fd 누수가 가장 흔한 곳)

## 추가 확인 규칙

- 자동화할 수 있는 항목은 이 표가 아니라 `run_integration.sh`에 추가한다.
- 스토리를 story 브랜치에 병합할 때 해당 스토리의 터미널 의존 동작을 새 절(`## v2: ...`)로 추가한다.
- 이전 절은 지우지 않는다. `main`에 통합할 때마다 모두 다시 확인한다.
