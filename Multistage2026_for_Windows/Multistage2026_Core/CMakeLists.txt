cmake_minimum_required(VERSION 3.16)

project(Multistage2026 LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------- ORBITER ROOT -----------
if(WIN32)
  if(NOT DEFINED ENV{ORBITER_ROOT})
    message(FATAL_ERROR "Define ORBITER_ROOT en el entorno")
  endif()
  set(ORBITER_ROOT $ENV{ORBITER_ROOT})
else()
  set(ORBITER_ROOT "$ENV{HOME}/orbiter_test/Orbiter")
endif()

set(ORBITER_SDK_INCLUDE "${ORBITER_ROOT}/Orbitersdk/include")
set(ORBITER_SDK_LIB "${ORBITER_ROOT}/Orbitersdk/lib")

# ----------- LIBS -----------
if(WIN32)
  file(GLOB ORBITER_LIBS "${ORBITER_SDK_LIB}/*.lib")
else()
  file(GLOB ORBITER_LIBS "${ORBITER_SDK_LIB}/*.a")
endif()

# ----------- SOURCES -----------
set(SOURCES
    Guidance.cpp
    Multistage2026.cpp
    parser.cpp
    Particles.cpp
    peg.cpp
    WriteIni.cpp
    Resources.rc
)

set(HEADERS
    Multistage2026.hpp
    resource.h
    WriteIni.hpp
)

add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ORBITER_SDK_INCLUDE}
)

target_link_libraries(${PROJECT_NAME}
    ${ORBITER_LIBS}
)

if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()