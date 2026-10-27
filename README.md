# 2D OpenGL 과제 — DSA · 모델 변환 · 카메라 · 텍스처

과제는 **4가지**로 이루어져 있으며 A4(Assignment 4) 완료본이 최종 과제이다. A2는 학생이 작성한 A1의 Mesh를, A3는 A1의 Mesh와 A2의 Object를, A4는 앞의 세 모듈을 실제로 include해서 사용한다.

## 수업과 과제 진행

| 주차 | 강의 자료 | 과제 진행 |
|---|---|---|
| 1주차 | 1~2: 그래픽스 시스템, 렌더링 파이프라인 | 개발 환경 설치와 스타터 실행 확인 |
| 2주차 | 3: OpenGL 소개 | 버퍼·shader 구조 확인, A1 준비 |
| 3주차 | 4~5: viewport, primitives | A1: 정점과 인덱스로 집 그리기 |
| 4주차 | 6~7: affine 변환, 행렬과 GLSL | A2: 여러 집의 모델 변환 |
| 5주차 | 8~9: 모델·카메라 변환, 텍스처 | A3 구현 후 A4 시작 |
| 6주차 | 복습 | 카메라·텍스처 연결 확인, 최종 제출 |

제출 날짜는 수업 공지를 따른다. A4에서는 정적인 텍스처 장면을 만들며 애니메이션은 요구하지 않는다.

## 프로젝트 셋업
프로젝트 루트에서 다음처럼 실행한다.

```bat
build.bat student 1 run
build.bat student 2 run
build.bat student 3 
build.bat student 4 
```

마지막 `run`을 빼면 빌드만 한다. `build.bat`은 vcpkg 경로와 CMake를 자동 탐색한다. Visual Studio Installer의 `vswhere`를 사용하므로 Visual Studio 2022를 다른 드라이브에 설치한 경우도 찾는다. VS2022 C++ 개발 도구, vcpkg, CMake는 설치되어 있어야 한다. 도구를 못 찾으면 이유를 출력하고 멈추며, 설정/컴파일 실패 후 오래된 실행 파일을 실행하지 않는다.

## 작성할 파일

| 단계 | 작성할 파일 | 앞 과제에서 그대로 사용하는 파일 |
|---|---|---|
| A1 | `student/a1/mesh.hpp` | 없음 |
| A2 | `student/a2/objects.hpp`, `scene.vert` | `student/a1/mesh.hpp` |
| A3 | `student/a3/camera.hpp`, `main.cpp`, `scene.vert` | A1 Mesh + A2 Object |
| A4 | `student/a4/texture.hpp`, `main.cpp`, `scene.vert`, `scene.frag` | A1 Mesh + A2 Object + A3 Camera |

- 각 스타터의 `TODO A1.1` 같은 번호를 해당 `docs/A1.md`와 대조한다.
- 뒤 과제의 실행 파일은 앞 과제의 수정 내용을 함께 빌드한다. A1 미완성이면 A2~A4도 집을 그리지 못하는 것이 정상이다.
- 모든 창은 왼쪽 위에 **노란 시작 표시**를 그린다. 이 표시는 학생의 정점/행렬/텍스처 구현과 독립적으로 제공되며, 제출 결과물로 점수를 받는 도형은 아니다.
- `support/course.hpp`는 창, 입력 조회, 파일 읽기, shader 컴파일, 행렬 곱, 수명 관리를 제공한다. `Mesh` 생성/그리기, 변환 행렬 구성·전달, 카메라, 텍스처 GPU 설정은 학생이 구현한다.
- 코드 골격의 구조체 필드와 함수 인터페이스는 제공된다. 이를 채우는 GPU 호출과 동작 구현이 과제다. u/v 필드는 처음부터 예약되지만 A4 전에는 사용하지 않는다.

## 누적 작업과 제출

A1 완료 후 같은 프로젝트에서 A2를 시작한다. 앞 과제 파일을 새 폴더에 복사할 필요는 없다. 제출에는 해당 단계까지의 `student/a1`~`student/aN`, 결과 캡처, 과제의 설명 답안을 포함한다. 공통 제공 파일을 변경했다면 그 파일도 함께 제출한다. 빌드 폴더, 실행 파일, `vcpkg_installed`는 제출에서 제외한다.

앞 단계가 동작하지 않으면 다음 단계에서도 같은 문제가 나타날 수 있다. 이때는 마지막으로 정상 동작한 단계, 오류 메시지, 수정한 파일을 정리해 질문한다. 제출 답안에는 완성한 항목과 아직 동작하지 않는 항목을 구분해 적는다.

## 조작

| 단계 | 조작 |
|---|---|
| A2 | 1/2/3 선택, WASD 이동, Q/E 회전, Z/X 크기, R 선택 오브젝트 초기화, O TRS/RTS 전환 |
| A3 | 화살표 카메라 이동, U/J 카메라 회전, =/− 줌, C 카메라 초기화, 마우스 클릭 배치(선택 실습) |
| A4 | A3 카메라 조작 + F NEAREST/LINEAR, V 체크무늬 REPEAT/CLAMP |
| 공통 | Escape 종료 |


## 참고 자료

공식 API 참고: https://registry.khronos.org/OpenGL-Refpages/gl4/html/index.php

이미지는 `assets/atlas.png`, `assets/checker.png`로 제공한다. PNG 읽기는 `support/image.cpp`에서 Windows 기본 이미지 디코더(WIC)로 처리하므로 별도 이미지 라이브러리 설치나 학생 구현은 필요하지 않다. 이미지 로더는 Windows용이다.
