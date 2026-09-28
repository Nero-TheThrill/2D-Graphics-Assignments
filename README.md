# 누적 2D OpenGL 과제 — DSA · 모델 변환 · 카메라 · 텍스처

과제는 **4개**다. A4 완료본이 최종 장면이다. A2는 학생이 작성한 A1의 Mesh를, A3는 A1의 Mesh와 A2의 Object를, A4는 앞의 세 모듈을 실제로 include해서 사용한다. 앞 과제의 정답을 뒤 스타터에 숨겨 넣지 않았다.

## CMD에서 바로 사용하기 (권장)

학생용/교사용 프로젝트 루트에서 다음처럼 실행한다. **PowerShell을 사용하지 않는다.**

```bat
build.bat student 1 run
build.bat solutions 1 run
build.bat solutions 4 run
```

마지막 `run`을 빼면 빌드만 한다. `build.bat`은 vcpkg 경로와 CMake를 자동 탐색한다. Visual Studio Installer의 `vswhere`를 사용하므로 Visual Studio 2022를 다른 드라이브에 설치한 경우도 찾는다. VS2022 C++ 개발 도구, vcpkg, CMake는 설치되어 있어야 한다. 도구를 못 찾으면 이유를 출력하고 멈추며, 설정/컴파일 실패 후 오래된 실행 파일을 실행하지 않는다.

## GitHub에서 한 파일로 셋업하기

교사는 **학생용 압축을 푼 내용**을 공개 GitHub 저장소 루트에 올린다. `CMakeLists.txt`, `build.bat`, `student/`, `support/`, `assets/` 등이 저장소 바로 아래에 있어야 한다. 압축 파일 자체만 올리거나 교사용 정답/체크포인트를 학생 저장소에 함께 올리지 않는다.

1. `setup.bat` 상단의 `COURSE_REPO=YOUR_USERNAME/YOUR_REPOSITORY`를 실제 `계정/저장소`로 바꾼다.
2. 브랜치가 main이 아니면 `COURSE_BRANCH`도 바꾼다.
3. 학생에게 수정한 `setup.bat` 하나를 전달한다. 학생은 CMD에서 실행하거나 더블클릭한다.
4. BAT 옆의 새 `graphics-course` 폴더에 소스가 내려받아지고 학생용 A1이 빌드·실행된다.

주소를 파일에 고정하지 않고 실행할 때 전달해도 된다.

```bat
setup.bat https://github.com/ACCOUNT/REPOSITORY
setup.bat ACCOUNT/REPOSITORY main "C:\Class\Graphics"
```

Git 설치는 필요 없다. Windows의 `curl.exe`와 `tar.exe`를 사용하며, 해당 도구가 없으면 안내 후 중단한다. 다운로드는 공개 저장소용이고 로그인 정보를 요구하거나 저장하지 않는다. Visual Studio/CMake 자체를 설치하는 스크립트는 아니다. 첫 빌드에서 vcpkg가 GLFW/GLEW를 준비한다.

이미 대상 폴더가 있으면 학생 작업 보호를 위해 중단한다. 이후 재빌드는 기존 프로젝트에서 `build.bat student 1 run`으로 한다. 새로운 저장소 버전을 다시 받으려면 별도의 새 폴더를 지정한다. GitHub 주소가 아직 정해지지 않았다면 setup 실행 시 `OWNER/REPO`를 입력할 수 있다.

기존 `build.ps1`도 남아 있지만 CMD 사용 시 필요 없다. BAT 자체는 이 제작 환경에 Windows CMD가 없어 실제 Windows 실행은 별도 확인이 필요하다. 과제 C++/GLSL과 기존 렌더링 결과는 변경하지 않았다.

## 먼저 실행하기

Windows + Visual Studio 2022 C++ 개발 도구 + CMake + vcpkg를 사용한다. OpenGL **4.5 Core**를 지원하는 GPU 드라이버가 필요하다. GLFW/GLEW는 포함된 manifest로 설치된다. 순수 OpenGL 3.3 또는 macOS 기본 OpenGL로는 이 DSA 과제를 실행할 수 없다.

압축은 이전 버전과 섞지 말고 새 폴더에 푼다. 프로젝트 루트에서 PowerShell 명령을 실행한다.

```powershell
# 학생용 A1
.\build.ps1 -Track student -Assignment 1 -Run

# 교사용 A1 정답 (교사용 압축본에서)
.\build.ps1 -Track solutions -Assignment 1 -Run

# 교사용 A4 정답
.\build.ps1 -Track solutions -Assignment 4 -Run
```

스크립트는 `VCPKG_ROOT`, `C:\vcpkg`, Visual Studio 2022의 Community/Professional/Enterprise 설치 경로를 확인한다. 자동으로 못 찾으면 다음처럼 실제 경로를 지정한다.

```powershell
.\build.ps1 -Track solutions -Assignment 1 -Run -VcpkgRoot 'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg'
```

PowerShell 정책으로 스크립트 실행이 막혀 있으면 아래 CMake 명령을 직접 사용한다. `C:/vcpkg`는 실제 설치 경로로 바꾼다. 공백이 있는 경로도 인자 전체를 따옴표로 묶는다.

```powershell
cmake -S . -B build-solutions -G "Visual Studio 17 2022" -A x64 "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DTRACK=solutions
cmake --build build-solutions --config Debug --target a1
.\build-solutions\Debug\a1.exe
```

학생용은 `TRACK=student`, 빌드 폴더는 `build-student`로 바꾼다. CMake 설정이 실패하면 다음 build 명령을 실행하지 않는다. 수정한 C++은 다시 빌드한다. GLSL 파일은 실행 시 읽으므로 저장 후 재실행하면 된다.

## 어떤 파일을 수정하나

| 단계 | 새로 수정할 파일 | 앞 과제에서 그대로 사용하는 파일 |
|---|---|---|
| A1 | `student/a1/mesh.hpp` | 없음 |
| A2 | `student/a2/objects.hpp`, `main.cpp`, `scene.vert` | `student/a1/mesh.hpp` |
| A3 | `student/a3/camera.hpp`, `main.cpp`, `scene.vert` | A1 Mesh + A2 Object |
| A4 | `student/a4/texture.hpp`, `main.cpp`, `scene.vert`, `scene.frag` | A1 Mesh + A2 Object + A3 Camera |

- 각 스타터의 `TODO A1.1` 같은 번호를 해당 `docs/A1.md`와 대조한다.
- 뒤 과제의 실행 파일은 앞 과제의 수정 내용을 함께 빌드한다. A1 미완성이면 A2~A4도 집을 그리지 못하는 것이 정상이다.
- 모든 창은 왼쪽 위에 **노란 시작 표시**를 그린다. 이 표시는 학생의 정점/행렬/텍스처 구현과 독립적으로 제공되며, 제출 결과물로 점수를 받는 도형은 아니다.
- 창 제목과 콘솔에서 `[student]` / `[solutions]`를 확인한다. 교사용 압축본도 기본 빌드는 student다.
- `support/course.hpp`는 창, 입력 조회, 파일 읽기, shader 컴파일, 행렬 곱, 수명 관리를 제공한다. `Mesh` 생성/그리기, 변환 행렬 구성·전달, 카메라, 텍스처 GPU 설정은 학생이 구현한다.
- 코드 골격의 구조체 필드와 함수 인터페이스는 제공된다. 이를 채우는 GPU 호출과 동작 구현이 과제다. u/v 필드는 처음부터 예약되지만 A4 전에는 사용하지 않는다.

## 누적 작업과 제출

A1 완료 후 같은 프로젝트에서 A2를 시작한다. 앞 과제 파일을 새 폴더에 복사할 필요 없다. 제출에는 해당 단계까지의 `student/a1`~`student/aN`, 결과 캡처/짧은 녹화, 과제의 설명 답안을 포함한다. 공통 제공 파일을 변경했다면 그 파일도 함께 낸다. 빌드 폴더, 실행 파일, `vcpkg_installed`는 제외한다.

교사용 압축본은 `solutions/a1`~`a4`를 포함한다. `checkpoints/after-a1.zip` 등은 직전 과제를 못 마친 학생에게 **선택적으로 배포하는 기준 완성본**이다. 교사가 배포한 파일만 사용한다. 체크포인트를 프로젝트 루트에 풀면 그 단계까지의 `student/` 모듈이 교체되므로 자신의 작업은 먼저 백업한다. 이후 단계의 TODO는 그대로 남는다. 학생용 일반 배포본에는 정답과 체크포인트가 없다.

## 조작

| 단계 | 조작 |
|---|---|
| A2 | 1/2/3 선택, WASD 이동, Q/E 회전, Z/X 크기, R 선택 오브젝트 초기화, O TRS/RTS 전환 |
| A3 | 화살표 카메라 이동, U/J 카메라 회전, =/− 줌, C 카메라 초기화, 마우스 클릭 배치(보너스) |
| A4 | A3 카메라 조작 + F NEAREST/LINEAR, V 체크무늬 REPEAT/CLAMP, Space 재생/정지, N 정지 중 한 프레임 |
| 공통 | Escape 종료 |

A3/A4의 main은 카메라 조작에 집중한다. A2의 Object 조작 함수와 행렬 순서 비교는 A2 실행 파일에서 계속 확인할 수 있다.

## 자료와 범위

관련 강의: L2 OpenGL Intro, L3 Primitives/Viewport, L5 2D Affine Transforms, L6 Vectors/Matrices/GLSL, L7 Model/Camera Transforms, L8 Texture Mapping. 과제 문서에 연결 개념을 명시했다. 원본 강의 PDF는 재배포하지 않는다.

이미지 로더는 작은 P6 PPM 형식을 읽는다. 포함된 atlas/checker 이미지는 이 과제용으로 생성했다. 이미지 디코딩 라이브러리 사용법은 평가 대상이 아니며, 로더가 반환한 RGB 픽셀을 GPU 텍스처로 만드는 과정이 평가 대상이다. 투명도·3D·조명·충돌은 필수가 아니다.

DSA의 attribute location과 buffer binding index는 서로 다른 번호다. A1에서 position=0, color=1 두 attribute가 같은 buffer binding=0을 사용하고 A4에서 UV=2를 추가한다.

공식 API 참고: https://registry.khronos.org/OpenGL-Refpages/gl4/html/index.php
