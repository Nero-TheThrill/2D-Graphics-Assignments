cmake_minimum_required(VERSION 3.20)
project(Cumulative2D LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
set(TRACK student CACHE STRING "student or solutions")
set_property(CACHE TRACK PROPERTY STRINGS student solutions)
if(NOT TRACK STREQUAL "student" AND NOT TRACK STREQUAL "solutions")
  message(FATAL_ERROR "TRACK must be student or solutions")
endif()
message(STATUS "Cumulative source track: ${TRACK}")
foreach(n RANGE 1 4)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TRACK}/a${n}/main.cpp")
    add_executable(a${n} ${TRACK}/a${n}/main.cpp support/image.cpp)
    # List editable files explicitly so generated IDE projects show them too.
    file(GLOB assignment_shaders CONFIGURE_DEPENDS
      "${CMAKE_CURRENT_SOURCE_DIR}/${TRACK}/a${n}/*.vert"
      "${CMAKE_CURRENT_SOURCE_DIR}/${TRACK}/a${n}/*.frag")
    set_source_files_properties(${assignment_shaders} PROPERTIES HEADER_FILE_ONLY TRUE)
    target_sources(a${n} PRIVATE ${assignment_shaders} support/course.hpp)
    source_group("Shaders" FILES ${assignment_shaders})
    source_group("Support" FILES support/course.hpp support/image.cpp)
    target_include_directories(a${n} PRIVATE support)
    # Later assignments actually compile earlier work, rather than copying its answer.
    foreach(previous RANGE 1 ${n})
      target_include_directories(a${n} PRIVATE ${TRACK}/a${previous})
      file(GLOB assignment_headers CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/${TRACK}/a${previous}/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${TRACK}/a${previous}/*.h")
      target_sources(a${n} PRIVATE ${assignment_headers})
      source_group("Assignments\\a${previous}" FILES ${assignment_headers})
    endforeach()
    target_compile_definitions(a${n} PRIVATE
      COURSE_TRACK="${TRACK}"
      COURSE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
      SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/${TRACK}/a${n}")
    target_link_libraries(a${n} PRIVATE glfw GLEW::GLEW OpenGL::GL windowscodecs ole32)
    set_target_properties(a${n} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endif()
endforeach()

# Teacher-only checks; the student package does not need teacher/ or solutions/.
if(TRACK STREQUAL "solutions" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/teacher/verify.cpp")
  add_executable(verify_solutions teacher/verify.cpp support/image.cpp)
  target_include_directories(verify_solutions PRIVATE support solutions/a1 solutions/a2 solutions/a3 solutions/a4)
  target_compile_definitions(verify_solutions PRIVATE
    COURSE_TRACK="solutions"
    COURSE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")
  target_link_libraries(verify_solutions PRIVATE glfw GLEW::GLEW OpenGL::GL windowscodecs ole32)
endif()
