@echo off
setlocal EnableExtensions DisableDelayedExpansion
rem Usage: build.bat [student|solutions] [1..4] [run|build]
if /i "%~1"=="--help" goto usage
set "COURSE_TRACK=%~1"
set "COURSE_ASSIGNMENT=%~2"
set "COURSE_ACTION=%~3"
if not defined COURSE_TRACK set "COURSE_TRACK=student"
if not defined COURSE_ASSIGNMENT set "COURSE_ASSIGNMENT=1"
if not defined COURSE_ACTION set "COURSE_ACTION=build"
if /i "%COURSE_TRACK%"=="student" goto track_ok
if /i "%COURSE_TRACK%"=="solutions" goto track_ok
goto usage
:track_ok
if /i "%COURSE_TRACK%"=="student" set "COURSE_TRACK=student"
if /i "%COURSE_TRACK%"=="solutions" set "COURSE_TRACK=solutions"
set "COURSE_VALID="
for %%N in (1 2 3 4) do if "%COURSE_ASSIGNMENT%"=="%%N" set "COURSE_VALID=1"
if not defined COURSE_VALID goto usage
if /i "%COURSE_ACTION%"=="build" goto action_ok
if /i "%COURSE_ACTION%"=="run" goto action_ok
goto usage
:action_ok
pushd "%~dp0"
if errorlevel 1 exit /b 1
if not exist "%COURSE_TRACK%\a%COURSE_ASSIGNMENT%\main.cpp" goto missing_source

rem vswhere locates VS2022 even when installed outside Program Files.
set "COURSE_VS="
set "COURSE_VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%COURSE_VSWHERE%" set "COURSE_VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%COURSE_VSWHERE%" goto detect_vcpkg
set "COURSE_QUERY=%TEMP%\course-vs-%RANDOM%-%RANDOM%.txt"
"%COURSE_VSWHERE%" -latest -products * -version "[17.0,18.0)" -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath > "%COURSE_QUERY%"
for /f "usebackq delims=" %%I in ("%COURSE_QUERY%") do set "COURSE_VS=%%I"
del /q "%COURSE_QUERY%" >nul 2>&1

:detect_vcpkg
set "COURSE_VCPKG="
if defined VCPKG_ROOT call :probe_vcpkg "%VCPKG_ROOT%"
if not defined COURSE_VCPKG if defined COURSE_VS call :probe_vcpkg "%COURSE_VS%\VC\vcpkg"
if not defined COURSE_VCPKG call :probe_vcpkg "%~dp0vcpkg"
if not defined COURSE_VCPKG call :probe_vcpkg "C:\vcpkg"
if not defined COURSE_VCPKG call :probe_vcpkg "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\vcpkg"
if not defined COURSE_VCPKG call :probe_vcpkg "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\vcpkg"
if not defined COURSE_VCPKG call :probe_vcpkg "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\vcpkg"
if not defined COURSE_VCPKG call :probe_vcpkg "%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\VC\vcpkg"
if not defined COURSE_VCPKG goto missing_vcpkg

set "COURSE_CMAKE="
for /f "delims=" %%I in ('where cmake.exe 2^>nul') do if not defined COURSE_CMAKE set "COURSE_CMAKE=%%I"
if not defined COURSE_CMAKE if defined COURSE_VS if exist "%COURSE_VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" set "COURSE_CMAKE=%COURSE_VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if not defined COURSE_CMAKE goto missing_cmake

echo [Build] %COURSE_TRACK% / A%COURSE_ASSIGNMENT%
echo [vcpkg] "%COURSE_VCPKG%"
echo [CMake] "%COURSE_CMAKE%"
"%COURSE_CMAKE%" -S . -B "build-%COURSE_TRACK%" -G "Visual Studio 17 2022" -A x64 "-DCMAKE_TOOLCHAIN_FILE=%COURSE_VCPKG%\scripts\buildsystems\vcpkg.cmake" "-DTRACK=%COURSE_TRACK%"
if errorlevel 1 goto configure_failed
"%COURSE_CMAKE%" --build "build-%COURSE_TRACK%" --config Debug --target "a%COURSE_ASSIGNMENT%"
if errorlevel 1 goto compile_failed
if /i not "%COURSE_ACTION%"=="run" goto success
"build-%COURSE_TRACK%\Debug\a%COURSE_ASSIGNMENT%.exe"
if errorlevel 1 goto run_failed
:success
popd
exit /b 0

:probe_vcpkg
if exist "%~1\scripts\buildsystems\vcpkg.cmake" set "COURSE_VCPKG=%~1"
exit /b 0
:missing_source
echo ERROR: Source not found: %COURSE_TRACK%/a%COURSE_ASSIGNMENT%
echo The student package does not include solutions.
goto failed
:missing_vcpkg
echo ERROR: vcpkg was not found. Enable vcpkg in Visual Studio Installer,
echo or set VCPKG_ROOT to your existing vcpkg directory and retry.
goto failed
:missing_cmake
echo ERROR: CMake was not found. Install CMake or enable C++ CMake tools in Visual Studio.
goto failed
:configure_failed
echo ERROR: CMake configuration failed. Compilation was not attempted.
goto failed
:compile_failed
echo ERROR: Compilation failed. An old executable was not started.
goto failed
:run_failed
echo ERROR: Program returned an error. Read its message above.
:failed
popd
exit /b 1
:usage
echo Usage: build.bat [student^|solutions] [1^|2^|3^|4] [build^|run]
echo Example: build.bat student 1 run
exit /b 2
