# 필수 기능 검증

## 자동 검사

| 명령 | 검증 내용 |
|---|---|
| `make -C Assignments` | 제출용 실행 파일 빌드 |
| `make -C Tests test` | Unity 단위 테스트와 프로세스 수명주기 테스트 |
| `make -C Tests integration` | 실제 실행 파일의 표준 출력과 종료 상태를 Bash와 비교 |
| `make -C Tests integration-signals` | PTY를 이용한 대화형 시그널 및 EOF 동작 |
| `make -C Tests sanitize` | AddressSanitizer와 UndefinedBehaviorSanitizer |
| `make -C Tests memory` | Valgrind 누수 및 오류 검사 |
| `norminette Assignments/src Assignments/include` | C 소스와 헤더의 Norminette 검사 |

통합 테스트는 실제 바이너리를 실행하고 비대화형 프롬프트 반향을 제거한 뒤 표준 출력과 마지막 종료 상태를 Bash와 비교합니다. 오류 문구와 상태가 동작의 일부인 경우에는 해당 진단도 직접 확인합니다. 모든 명령에는 제한 시간이 적용됩니다.

## 기능 검증 범위

통합 사례는 직접 경로와 `PATH` 탐색, 없는 명령과 실행 권한 오류, 단일·다단 파이프라인, 대용량 스트림과 조기 종료, 파이프 문법, 따옴표와 문자열 연결, 필드 분리, 환경 변경, 필수 내장 명령 7종, 리다이렉션, heredoc의 확장·따옴표·복수 입력·EOF, 종료 상태 전달을 다룹니다.

PTY 테스트는 프롬프트에서 Ctrl-C와 Ctrl-\, 자식 실행 중 Ctrl-C와 Ctrl-\, heredoc 수집 중 Ctrl-C, Ctrl-D를 확인합니다.

## 평가 전 수동 확인

1. 단독 `cd` 뒤에 `pwd`를 실행해 부모의 작업 디렉터리가 바뀌었는지 확인합니다.
2. `export TEST=value` 뒤에 `env | grep TEST`를 실행해 자식에 변수가 전달되는지 확인합니다.
3. `echo hello > out | cat`을 실행해 명시적 리다이렉션이 파이프 출력보다 우선하는지 확인합니다.
4. 따옴표가 있는 구분자와 없는 구분자로 heredoc을 입력해 후자만 변수를 확장하는지 확인합니다.
5. 프롬프트 입력, 포그라운드 명령, heredoc 입력 중 Ctrl-C를 눌러 각각 다시 사용할 수 있는 프롬프트로 복귀하는지 확인합니다.

필수 구현은 보너스 연산자, 와일드카드 확장, 작업 제어, 명령 치환, 전체 Bash 문법을 지원하지 않습니다.
