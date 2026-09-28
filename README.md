# 2D OpenGL 과제 — DSA · 모델 변환 · 카메라 · 텍스처

과제는 **4가지**로 이루어져 있으며 A4(Assignment 4) 완료본이 최종 과제이다. A2는 학생이 작성한 A1의 Mesh를, A3는 A1의 Mesh와 A2의 Object를, A4는 앞의 세 모듈을 실제로 include해서 사용한다.

## CMD에서 바로 사용하기 (권장)

학생용/교사용 프로젝트 루트에서 다음처럼 실행한다.

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
| A2 | `student/a2/objects.hpp`, `main.cpp`, `scene.vert` | `student/a1/mesh.hpp` |
| A3 | `student/a3/camera.hpp`, `main.cpp`, `scene.vert` | A1 Mesh + A2 Object |
| A4 | `student/a4/texture.hpp`, `main.cpp`, `scene.vert`, `scene.frag` | A1 Mesh + A2 Object + A3 Camera |

- 각 스타터의 `TODO A1.1` 같은 번호를 해당 `docs/A1.md`와 대조한다.
- 뒤 과제의 실행 파일은 앞 과제의 수정 내용을 함께 빌드한다. A1 미완성이면 A2~A4도 집을 그리지 못하는 것이 정상이다.
- 모든 창은 왼쪽 위에 **노란 시작 표시**를 그린다. 이 표시는 학생의 정점/행렬/텍스처 구현과 독립적으로 제공되며, 제출 결과물로 점수를 받는 도형은 아니다.
- `support/course.hpp`는 창, 입력 조회, 파일 읽기, shader 컴파일, 행렬 곱, 수명 관리를 제공한다. `Mesh` 생성/그리기, 변환 행렬 구성·전달, 카메라, 텍스처 GPU 설정은 학생이 구현한다.
- 코드 골격의 구조체 필드와 함수 인터페이스는 제공된다. 이를 채우는 GPU 호출과 동작 구현이 과제다. u/v 필드는 처음부터 예약되지만 A4 전에는 사용하지 않는다.

## 누적 작업과 제출

A1 완료 후 같은 프로젝트에서 A2를 시작한다. 앞 과제 파일을 새 폴더에 복사할 필요는 없다. 제출에는 해당 단계까지의 `student/a1`~`student/aN`, 결과 캡처, 과제의 설명 답안을 포함한다. 공통 제공 파일을 변경했다면 그 파일도 함께 제출한다. 빌드 폴더, 실행 파일, `vcpkg_installed`는 제출에서 제외한다.

## 조작

| 단계 | 조작 |
|---|---|
| A2 | 1/2/3 선택, WASD 이동, Q/E 회전, Z/X 크기, R 선택 오브젝트 초기화, O TRS/RTS 전환 |
| A3 | 화살표 카메라 이동, U/J 카메라 회전, =/− 줌, C 카메라 초기화, 마우스 클릭 배치(보너스) |
| A4 | A3 카메라 조작 + F NEAREST/LINEAR, V 체크무늬 REPEAT/CLAMP, Space 재생/정지, N 정지 중 한 프레임 |
| 공통 | Escape 종료 |

A3/A4의 main은 카메라 조작에 집중한다. A2의 Object 조작 함수와 행렬 순서 비교는 A2 실행 파일에서 계속 확인할 수 있다.


##
공식 API 참고: https://registry.khronos.org/OpenGL-Refpages/gl4/html/index.php
