cmake_minimum_required(VERSION 3.16)

project(MS_Hangar LANGUAGES CXX)  # ← cambiar nombre según módulo

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

# ----------- LIBRERÍAS -----------
if(WIN32)
  file(GLOB ORBITER_LIBS "${ORBITER_SDK_LIB}/*.lib")
else()
  file(GLOB ORBITER_LIBS "${ORBITER_SDK_LIB}/*.a")
endif()

# ----------- SOURCES -----------
set(SOURCES
    MS_Hangar.cpp   # ← cambiar
)

set(HEADERS
    MS_Hangar.hpp   # ← cambiar
)

# ----------- TARGET -----------
add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${HEADERS}
)

# ----------- INCLUDE -----------
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ORBITER_SDK_INCLUDE}
)

# ----------- LINK -----------
target_link_libraries(${PROJECT_NAME}
    ${ORBITER_LIBS}
)

# ----------- WINDOWS FIXES -----------
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()