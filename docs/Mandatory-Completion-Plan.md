# Minishell 필수 기능 완성 계획과 결과

## 목표와 범위

이 브랜치는 기존 파이프라인 실행기를 바탕으로 42 Minishell 과제의 필수 범위를 완성합니다. 기준은 과제 원문이며, Bash 비교와 [42 EvalHub 평가표](https://www.42evalhub.com/common/minishell)는 기능 조합을 빠뜨리지 않았는지 확인하는 데 사용합니다.

- 작업 브랜치: `feature/complete-mandatory`
- 기반 브랜치: `feature/executor-with-pipe`의 기존 파이프라인 구현
- 검토 대상 브랜치: `story`
- 과제 원문: [Minishell](../Materials/Subjects/minishell_kr.md)
- 기존 구조 문서: [실행기 설계](Executor-Design.md), [오류 처리](Error-Handling.md)
- 기능 설계 문서: [파싱과 확장](Parsing-Expansion-Design.md), [내장 명령과 환경 변수](Builtins-Environment-Design.md), [리다이렉션과 heredoc](Redirection-Heredoc-Design.md), [시그널 처리](Signals-Design.md)

보너스 연산자, 와일드카드 확장, 작업 제어, 명령 치환은 구현 범위에서 제외합니다.

## 구현 완료 항목

| 영역 | 구현 내용 |
|---|---|
| 입력 루프 | Readline 프롬프트와 히스토리, EOF 종료, 빈 줄 처리, 마지막 종료 상태 |
| 어휘 분석기와 구문 분석기 | 단어, 파이프, 입력·출력·추가·heredoc 연산자, 따옴표를 고려한 문법 검사, 순서가 유지되는 리다이렉션 목록, 명령 없는 리다이렉션 |
| 확장 | 작은따옴표와 큰따옴표, 문자열 연결, 빈 인자, 환경 변수와 `$?`, 따옴표 없는 필드 분리, 모호한 리다이렉션 검사 |
| 환경 변수 | 내보내기 및 비내보내기 선언, 빈 값, `envp` 생성, `export`와 `unset` 갱신 |
| 내장 명령 | `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| 실행 | `PATH`와 직접 경로 명령, 파이프라인, 부모에서 실행하는 단독 내장 명령, 자식에서 실행하는 파이프라인 내장 명령 |
| 리다이렉션 | 파이프 연결 이후 `<`, `>`, `>>`를 입력 순서로 적용, 부모 표준 FD 복구 |
| heredoc | 실행 전 복수 본문 수집, 따옴표로 둘러싼 구분자 처리, 본문 확장, EOF 경고, Ctrl-C 취소, 생성 직후 경로를 `unlink()`로 삭제하는 임시 파일 |
| 시그널 | 프롬프트 Ctrl-C/Ctrl-\, 자식 종료 상태, heredoc 중단, PTY 회귀 테스트 |
| 자원 정리 | 부모와 자식의 파이프 FD 닫기, 자식 회수, heredoc FD 정리, 실패 경로 처리 |

## 설계 결정

1. 어휘 분석기와 구문 분석기가 원문 단어를 보존합니다. 문법 분석을 마친 뒤 확장하므로 변수 값의 파이프 문자 등이 연산자로 다시 해석되지 않습니다.
2. 각 명령은 원문 argv와 확장된 argv를 따로 보관합니다. 확장기는 따옴표 상태에 따라 필드를 구성해 빈 인자를 유지하고, 따옴표 없는 확장 결과만 분리합니다.
3. 리다이렉션은 입력 순서를 보존합니다. 먼저 파이프 FD를 연결하고, 그 뒤 명령별 리다이렉션을 왼쪽부터 적용합니다.
4. 단독 내장 명령이나 리다이렉션만 있는 명령은 부모에서 실행합니다. 표준 입력과 출력을 저장한 뒤 리다이렉션을 적용하고, 내장 명령이 있으면 실행한 후 FD를 복구합니다.
5. 파이프라인 명령은 자식에서 실행합니다. 자식 내장 명령의 상태 변경은 부모 셸에 전파되지 않습니다.
6. heredoc 본문은 실행 전에 부모에서 수집합니다. 배타적으로 만든 임시 파일에 쓰고 별도 읽기 FD를 연 뒤 경로를 `unlink()`로 바로 삭제합니다.
7. 자식 실행 중 부모는 시그널을 무시하고, 자식은 기본 동작을 복원합니다. 파일 범위의 `volatile sig_atomic_t` 변수 하나에 시그널을 기록합니다.

자세한 데이터 흐름과 FD 소유권은 위에 연결한 설계 문서에서 설명합니다.

## 검증

Bash 비교 통합 테스트는 명령 탐색, 따옴표, 확장, 내장 명령, 파이프라인, 리다이렉션, heredoc, 문법 오류, 종료 상태를 검사합니다. PTY 테스트는 대화형 시그널 동작을 확인합니다.

검토 전에 다음 명령을 실행합니다.

~~~sh
make -C Assignments
make -C Tests test
make -C Tests integration
make -C Tests integration-signals
make -C Tests sanitize
make -C Tests memory
norminette Assignments/src Assignments/include
~~~

세부 범위는 [필수 기능 검증](Mandatory-Validation.md)을 참고하세요. 이 문서는 필수 범위 구현을 기록하며, 전체 셸 문법을 지원한다고 주장하지 않습니다.
