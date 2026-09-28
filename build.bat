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
 
 
rem ---------------------------------------------------------------------------
rem Find Visual Studio
rem ---------------------------------------------------------------------------
 
set "COURSE_VS="
set "COURSE_VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
 
if not exist "%COURSE_VSWHERE%" set "COURSE_VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%COURSE_VSWHERE%" goto detect_git
 
set "COURSE_QUERY=%TEMP%\course-vs-%RANDOM%-%RANDOM%.txt"
 
"%COURSE_VSWHERE%" ^
    -latest ^
    -products * ^
    -version "[17.0,18.0)" ^
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
    -property installationPath > "%COURSE_QUERY%"
 
for /f "usebackq delims=" %%I in ("%COURSE_QUERY%") do set "COURSE_VS=%%I"
 
del /q "%COURSE_QUERY%" >nul 2>&1
 
 
rem ---------------------------------------------------------------------------
rem Find Git
rem ---------------------------------------------------------------------------
 
:detect_git
 
set "COURSE_GIT="
 
for /f "delims=" %%I in ('where git.exe 2^>nul') do if not defined COURSE_GIT set "COURSE_GIT=%%I"
 
if not defined COURSE_GIT if exist "%ProgramFiles%\Git\cmd\git.exe" ^
    set "COURSE_GIT=%ProgramFiles%\Git\cmd\git.exe"
 
if not defined COURSE_GIT if exist "%ProgramFiles(x86)%\Git\cmd\git.exe" ^
    set "COURSE_GIT=%ProgramFiles(x86)%\Git\cmd\git.exe"
 
if not defined COURSE_GIT if defined COURSE_VS if exist "%COURSE_VS%\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer\Git\cmd\git.exe" ^
    set "COURSE_GIT=%COURSE_VS%\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer\Git\cmd\git.exe"
 
 
rem ---------------------------------------------------------------------------
rem Find standalone vcpkg
rem
rem Visual Studio's bundled VC\vcpkg is intentionally NOT used.
rem ---------------------------------------------------------------------------
 
set "COURSE_VCPKG="
set "COURSE_AUTO_VCPKG=%LOCALAPPDATA%\graphics-course\vcpkg"
 
if defined VCPKG_ROOT call :probe_vcpkg "%VCPKG_ROOT%"
if not defined COURSE_VCPKG call :probe_vcpkg "%COURSE_AUTO_VCPKG%"
if not defined COURSE_VCPKG call :probe_vcpkg "%~dp0vcpkg"
if not defined COURSE_VCPKG call :probe_vcpkg "C:\vcpkg"
 
if not defined COURSE_VCPKG goto install_vcpkg
goto vcpkg_ready
 
 
rem ---------------------------------------------------------------------------
rem Install standalone vcpkg
rem ---------------------------------------------------------------------------
 
:install_vcpkg
 
if not defined COURSE_GIT goto missing_git
 
echo [vcpkg] Standalone vcpkg was not found.
echo [vcpkg] Installing to "%COURSE_AUTO_VCPKG%"
 
if not exist "%LOCALAPPDATA%\graphics-course" mkdir "%LOCALAPPDATA%\graphics-course"
 
rem Remove only an incomplete auto-generated installation.
if exist "%COURSE_AUTO_VCPKG%" rmdir /s /q "%COURSE_AUTO_VCPKG%"
 
echo [vcpkg] Downloading vcpkg...
 
"%COURSE_GIT%" clone https://github.com/microsoft/vcpkg.git "%COURSE_AUTO_VCPKG%"
 
if errorlevel 1 goto vcpkg_clone_failed
 
echo [vcpkg] Bootstrapping...
 
call "%COURSE_AUTO_VCPKG%\bootstrap-vcpkg.bat" -disableMetrics
 
if errorlevel 1 goto vcpkg_bootstrap_failed
 
call :probe_vcpkg "%COURSE_AUTO_VCPKG%"
 
if not defined COURSE_VCPKG goto vcpkg_bootstrap_failed
 
 
rem ---------------------------------------------------------------------------
rem vcpkg ready
rem ---------------------------------------------------------------------------
 
:vcpkg_ready
 
set "VCPKG_ROOT=%COURSE_VCPKG%"
 
 
rem ---------------------------------------------------------------------------
rem Find CMake
rem ---------------------------------------------------------------------------
 
set "COURSE_CMAKE="
 
for /f "delims=" %%I in ('where cmake.exe 2^>nul') do if not defined COURSE_CMAKE set "COURSE_CMAKE=%%I"
 
if not defined COURSE_CMAKE if defined COURSE_VS if exist "%COURSE_VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" ^
    set "COURSE_CMAKE=%COURSE_VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
 
if not defined COURSE_CMAKE goto missing_cmake
 
 
rem ---------------------------------------------------------------------------
rem Configure
rem ---------------------------------------------------------------------------
 
set "COURSE_BUILD=build-%COURSE_TRACK%"
 
echo.
echo [Build] %COURSE_TRACK% / A%COURSE_ASSIGNMENT%
echo [vcpkg] "%COURSE_VCPKG%"
echo [CMake] "%COURSE_CMAKE%"
echo.
 
rem Remove the old CMake cache so a previously selected vcpkg toolchain
rem does not remain cached. vcpkg_installed is kept.
if exist "%COURSE_BUILD%\CMakeCache.txt" del /q "%COURSE_BUILD%\CMakeCache.txt"
 
if exist "%COURSE_BUILD%\CMakeFiles" rmdir /s /q "%COURSE_BUILD%\CMakeFiles"
 
"%COURSE_CMAKE%" ^
    -S . ^
    -B "%COURSE_BUILD%" ^
    -G "Visual Studio 17 2022" ^
    -A x64 ^
    "-DCMAKE_TOOLCHAIN_FILE=%COURSE_VCPKG%\scripts\buildsystems\vcpkg.cmake" ^
    "-DTRACK=%COURSE_TRACK%"
 
if errorlevel 1 goto configure_failed
 
 
rem ---------------------------------------------------------------------------
rem Build
rem ---------------------------------------------------------------------------
 
"%COURSE_CMAKE%" ^
    --build "%COURSE_BUILD%" ^
    --config Debug ^
    --target "a%COURSE_ASSIGNMENT%"
 
if errorlevel 1 goto compile_failed
 
if /i not "%COURSE_ACTION%"=="run" goto success
 
 
rem ---------------------------------------------------------------------------
rem Run
rem ---------------------------------------------------------------------------
 
"%COURSE_BUILD%\Debug\a%COURSE_ASSIGNMENT%.exe"
 
if errorlevel 1 goto run_failed
 
 
:success
echo.
echo [Success] %COURSE_TRACK% / A%COURSE_ASSIGNMENT%
popd
exit /b 0
 
 
rem ---------------------------------------------------------------------------
rem Helpers
rem ---------------------------------------------------------------------------
 
:probe_vcpkg
 
if not exist "%~1\scripts\buildsystems\vcpkg.cmake" exit /b 0
if not exist "%~1\vcpkg.exe" exit /b 0
 
set "COURSE_VCPKG=%~1"
 
exit /b 0
 
 
rem ---------------------------------------------------------------------------
rem Errors
rem ---------------------------------------------------------------------------
 
:missing_source
echo ERROR: Source not found: %COURSE_TRACK%/a%COURSE_ASSIGNMENT%
echo The student package does not include solutions.
goto failed
 
 
:missing_git
echo ERROR: Git was not found.
echo Git is required the first time this course installs standalone vcpkg.
echo Install Git for Windows and run build.bat again.
goto failed
 
 
:vcpkg_clone_failed
echo ERROR: Failed to download vcpkg.
echo Check the network connection and try again.
goto failed
 
 
:vcpkg_bootstrap_failed
echo ERROR: Failed to bootstrap vcpkg.
goto failed
 
 
:missing_cmake
echo ERROR: CMake was not found.
echo Install CMake or enable C++ CMake tools in Visual Studio.
goto failed
 
 
:configure_failed
echo ERROR: CMake configuration failed.
echo Compilation was not attempted.
goto failed
 
 
:compile_failed
echo ERROR: Compilation failed.
echo An old executable was not started.
goto failed
 
 
:run_failed
echo ERROR: Program returned an error.
echo Read its message above.
 
 
:failed
popd
exit /b 1
 
 
rem ---------------------------------------------------------------------------
rem Usage
rem ---------------------------------------------------------------------------
 
:usage
echo Usage: build.bat [student^|solutions] [1^|2^|3^|4] [build^|run]
echo.
echo Examples:
echo   build.bat
echo   build.bat student 1
echo   build.bat student 1 run
echo   build.bat student 2 run
echo   build.bat solutions 3 run
exit /b 2