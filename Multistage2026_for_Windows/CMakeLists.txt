cmake_minimum_required(VERSION 3.16)
project(Multistage2026)

cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Tipo de build (solo relevante para generadores single-config como Ninja)
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug or Release)" FORCE)
endif()

# ----------- Ruta base del SDK -----------
# En Windows no existe HOME como en Linux → usar variable de entorno configurable
if(WIN32)
  if(NOT DEFINED ENV{ORBITER_ROOT})
    message(FATAL_ERROR "Debes definir la variable de entorno ORBITER_ROOT en Windows")
  endif()
  set(ORBITER_ROOT $ENV{ORBITER_ROOT})
else()
  set(ORBITER_ROOT "$ENV{HOME}/orbiter_test/Orbiter")
endif()

# Submódulos
set(MULTISTAGE2015_MFD_PATH ${ORBITER_ROOT}/Orbitersdk/samples/Multistage2015_MFD)
set(STAGE2010_PATH ${ORBITER_ROOT}/Orbitersdk/samples/Stage_dll_2010)

# ----------- Salidas -----------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Multistage2026_BIN)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Multi-config (Visual Studio)
if(CMAKE_CONFIGURATION_TYPES)
  foreach(output_config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${output_config} OUTPUTCONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  endforeach()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ----------- Subdirectorios internos -----------
add_subdirectory(Multistage2026_Core)
add_subdirectory(EmptyModule)
add_subdirectory(Crawler)
add_subdirectory(MS_Hangar)
add_subdirectory(MS_Camera)
add_subdirectory(boom)

# ----------- Subdirectorios externos -----------
#add_subdirectory(${MULTISTAGE2015_MFD_PATH} "${CMAKE_BINARY_DIR}/build_MFD")
add_subdirectory(${STAGE2010_PATH} "${CMAKE_BINARY_DIR}/build_stage2010")

# ----------- Flags específicos por plataforma -----------

# Evitar flags de Linux en Windows
if(NOT WIN32)
  add_compile_options(-fPIC)
  add_compile_options(-g)
endif()