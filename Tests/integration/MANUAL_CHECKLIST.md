# 수동 통합 체크리스트

`make -C Tests integration`은 stdin 으로 입력을 넣어 검증하므로 tty 가 있어야만 보이는
동작은 다루지 못한다. 그런 항목은 여기서 사람이 직접 확인하고, story → main PR 본문에
이 표를 복사해 결과를 남긴다.

실행: `make -C Assignments && ./Assignments/minishell`

## v1: 외부 명령 한 개 실행

| 입력 | 기대 | 확인 |
|---|---|---|
| `ls -a` | 목록이 출력되고 프롬프트 `minishell$ `가 다시 뜬다 | [ ] |
| `no_such_cmd` | `minishell: no_such_cmd: command not found`가 뜨고 프롬프트가 다시 뜬다 (셸이 죽지 않는다) | [ ] |
| 빈 줄 Enter, 공백만 Enter | 아무 출력 없이 프롬프트가 다시 뜬다 | [ ] |
| ↑ 화살표 | 직전에 입력한 명령이 나타난다 | [ ] |
| ↑ 를 여러 번 | 빈 줄·공백 줄은 히스토리에 없고, 실제 명령만 거슬러 올라간다 | [ ] |
| `/bin/false` 입력 후 Ctrl+D | 한 번에 종료된다. 바깥 셸에서 `echo $?` 가 `1` | [ ] |
| `ls -a` 입력 후 Ctrl+D | 종료 후 바깥 셸에서 `echo $?` 가 `0` | [ ] |

## 추가 규칙

- 자동화할 수 있는 항목은 이 표가 아니라 `run_integration.sh` 에 넣는다.
- 스토리가 story 브랜치에 머지될 때 그 스토리의 tty 의존 동작을 새 절(`## v2: ...`)로 추가한다.
- 이전 절은 지우지 않는다. main 통합 때마다 전부 다시 확인한다.
