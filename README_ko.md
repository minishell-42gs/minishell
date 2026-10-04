*이 프로젝트는 42 교육과정의 일부로 ringo와 taegokim이 작성했습니다.*

[영문 README](README.md)

# Minishell

## 설명

Minishell은 42 교육과정의 필수 과제를 위해 C로 작성한 간단한 대화형 셸입니다. 명령을 입력받아 단어와 연산자를 해석하고, 환경 변수를 확장한 뒤 내장 명령이나 외부 프로그램을 실행합니다. 파이프, 따옴표, 리다이렉션, heredoc과 필수 내장 명령 7종을 지원합니다.

명령의 원문 단어를 확장 단계까지 보존하고, 리다이렉션을 입력 순서대로 적용합니다. 상태를 바꾸는 단독 내장 명령은 부모 셸에서 실행하고, 파이프라인 명령은 자식 프로세스에서 실행합니다.

## 실행 방법

저장소 루트에서 빌드합니다.

~~~sh
make -C Assignments
~~~

셸을 실행합니다.

~~~sh
./Assignments/minishell
~~~

GNU Readline을 사용합니다. 테스트는 다음 명령으로 실행할 수 있습니다.

~~~sh
make -C Tests test
make -C Tests integration
make -C Tests integration-signals
make -C Tests sanitize
make -C Tests memory
~~~

보너스 연산자(`&&`, `||`, 괄호 그룹화, 와일드카드 확장)는 구현 범위에 포함하지 않습니다.

## 참고 자료

- [42 Minishell 과제](Materials/Subjects/minishell_kr.md)
- [42 EvalHub Minishell 평가표](https://www.42evalhub.com/common/minishell)
- [Bash 설명서](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline 설명서](https://tiswww.case.edu/php/chet/readline/readline.html)

AI는 기존 실행기와 오류 처리 설계를 검토하고, 필수 파서·확장·내장 명령·리다이렉션·heredoc·시그널 기능을 구현하고 디버깅하는 데 활용했습니다. 회귀 테스트와 설계 문서 작성에도 사용했습니다. 동작은 과제 원문과 Bash 비교 통합 테스트로 확인하고, Norminette 및 sanitizer·메모리 검사로 검증했습니다.
