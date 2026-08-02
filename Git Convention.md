# Branch Convention
## 규칙

- 영어 소문자를 사용한다.
- 단어 구분은 언더바(`_`) 대신 하이픈(`-`)을 사용한다.
- `prefix/작업내용` 형태로 작성한다.
- 작업 내용은 최대한 짧고 명확하게 작성한다.
- 가능하면 명사가 아닌 동사 형태로 시작한다.

## Prefix

| Prefix   | 설명                    | 예시                              |
| -------- | --------------------- | ------------------------------- |
| story  | 새로운 스토리 추가             | story/v2-walking_skeleton     |
| feature  | 새로운 기능 추가             | feature/user-profile-upload     |
| fix      | 버그 수정                 | fix/null-pointer-crash          |
| docs     | 문서 작성 및 수정            | docs/api-specification          |
| refactor | 기능 변경 없는 코드 개선        | refactor/remove-deprecated-code |
| test     | 테스트 코드 추가 및 수정        | test/unit-test-api              |
| chore    | 빌드, 설정, 의존성 등 기타 작업   | chore/add-gitignore             |
| style    | 코드 스타일 수정 (포맷팅, 주석 등) | style/format-code               |
| perf     | 성능 개선                 | perf/improve-search-speed       |
| build    | 빌드 시스템 변경             | build/update-makefile           |


# Commit Convention
## 규칙

- `type: 작업내용` 형태로 작성한다.
- `type`은 영어 소문자를 사용한다.
- `:` 뒤에는 한 칸의 공백을 둔다.
- 작업 내용은 영어로 간결하게 작성한다.
- 마침표(`.`)는 사용하지 않는다.

## Type

|Type|설명|예시|
|---|---|---|
|feat|새로운 기능 추가|feat: add login API|
|fix|버그 수정|fix: fix null pointer exception|
|docs|문서 작성 및 수정|docs: update README|
|refactor|기능 변경 없는 코드 개선|refactor: split parser logic|
|test|테스트 코드 추가 및 수정|test: add parser tests|
|chore|설정, 의존성 등 기타 작업|chore: update gitignore|
|style|코드 스타일 수정|style: format code|
|perf|성능 개선|perf: improve search speed|
|build|빌드 시스템 변경|build: update Makefile|
|revert|이전 커밋 되돌리기|revert: revert login feature|
|wip|작업 중간 상태 임시 저장 (PR 전 정리 필수)|wip: draft executor skeleton|